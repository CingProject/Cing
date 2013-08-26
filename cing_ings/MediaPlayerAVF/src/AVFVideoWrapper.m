//
//  AVFVideoWrapper.m
//  Cing
//
//  Created by Julio Obelleiro on 8/23/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#import "AVFVideoWrapper.h"


/* Asset keys */
NSString * const kTracksKey         = @"tracks";
NSString * const kPlayableKey       = @"playable";

/* PlayerItem keys */
NSString * const kStatusKey         = @"status";

/* AVPlayer keys */
NSString * const kRateKey           = @"rate";
NSString * const kCurrentItemKey    = @"currentItem";


@implementation AVFVideoWrapper;

static void *AVPlayerItemStatusContext = &AVPlayerItemStatusContext;


@synthesize player = _player;
@synthesize asset = _asset;
@synthesize playerItem = _playerItem;
//@synthesize playerItem;
//@synthesize videoOutput = _videoOutput;
//playerItemOutput,
@synthesize videoOutput = _videoOutput;
@synthesize ready; //playerLayer, layerRenderer;

- (AVFVideoWrapper *)init {
    if (self = [super init]) {
        
        _format = AVF_RGB;
    }
    return self;
}

- (void) loadFile:(NSString *)filename {
   
    // File loading
    NSURL *fileURL = [NSURL fileURLWithPath:[filename stringByStandardizingPath]];
    NSLog(@"Trying to load %@", filename);
    
    // Init the asset to have access to the file
    AVURLAsset *asset = [[AVURLAsset alloc] initWithURL:fileURL options:nil];

    // Load it when it is playable and the tracks are ready (to be able to acces frames)
    NSArray *requestedKeys = [NSArray arrayWithObjects:kTracksKey, kPlayableKey, nil];
    [asset loadValuesAsynchronouslyForKeys:requestedKeys completionHandler:^{
        dispatch_async(dispatch_get_main_queue(), ^{
            [self setupPlaybackWithAsset:asset];
        });
    }];
}

- (void)setupPlaybackWithAsset:(AVAsset *)asset
{
    // Check to see if the file loaded
    NSError *error;
    AVKeyValueStatus status = [asset statusOfValueForKey:@"tracks" error:&error];
    if(status == AVKeyValueStatusLoaded) {
             
        _playerItem = [AVPlayerItem playerItemWithAsset:asset];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(itemDidPlayToEnd:) name:AVPlayerItemDidPlayToEndTimeNotification object:nil];
    
        // Set format for video output in Cocoa format
        int cocoaPixelFormat = 0;
        switch (_format) {
            case AVF_RGB:
                cocoaPixelFormat = kCVPixelFormatType_24RGB;
                break;
                
            case AVF_RGBA:
                cocoaPixelFormat = kCVPixelFormatType_32RGBA;
                break;
                
            case AVF_BGR:
                cocoaPixelFormat = kCVPixelFormatType_24BGR;
                break;
                
            case AVF_BGRA:
                cocoaPixelFormat = kCVPixelFormatType_32BGRA;
                break;
                
            default:
                break;
        }
        
        // Set the pixel format for the video output
        NSDictionary *options = @{ ( NSString *)kCVPixelBufferPixelFormatTypeKey : @(cocoaPixelFormat) };
        _videoOutput = [[AVPlayerItemVideoOutput alloc] initWithPixelBufferAttributes:options];

        // Configure video output
        _videoOutput.suppressesPlayerRendering = YES;
        [_videoOutput requestNotificationOfMediaDataChangeWithAdvanceInterval:1];
        [_videoOutput setDelegate:self queue:dispatch_get_main_queue()];
        [_playerItem addOutput:self.videoOutput];
        
        // Set player item and callback for end of stream
        self.player = [AVPlayer playerWithPlayerItem:_playerItem];
        self.player.actionAtItemEnd = AVPlayerActionAtItemEndNone;
        [self.player play];
        
        NSLog(@" Self: %p, %p, %p, %p", self, self.player, _playerItem, _videoOutput );
        
        // Callbacks
        [_playerItem addObserver:self forKeyPath:@"status" options:0 context:AVPlayerItemStatusContext];
        [_player addObserver:self forKeyPath:@"status" options:0 context:AVPlayerItemStatusContext];
        
    }
    
    //self.displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(displayLinkFired:)];
    //self.displayLink.paused = YES;
    //[self.displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
}


#pragma mark - Notifications

- (void)itemDidPlayToEnd:(NSNotification *)note
{
    [[note object] seekToTime:kCMTimeZero];
}

#pragma mark - AVPlayerItemOutputPullDelegate

- (void)outputMediaDataWillChange:(AVPlayerItemOutput *)sender
{
    //self.displayLink.paused = NO;
}

- (void)outputSequenceWasFlushed:(AVPlayerItemOutput *)output
{
    // discard queued samples
}
- (void) dealloc {
    if(loading) {
        deallocWhenReady = YES;
    }
    else {
        [self stop];
        
        // SK: Releasing the CARenderer is slow for some reason
        //     It will freeze the main thread for a few dozen mS.
        //     If you're swapping in and out videos a lot, the loadFile:
        //     method should be re-written to just re-use and re-size
        //     these layers/objects rather than releasing and reallocating
        //     them every time a new file is needed.
        
        //if(self.layerRenderer) [self.layerRenderer release];
        
        /*[[NSNotificationCenter defaultCenter] removeObserver:self];
        if(self.playerItem) [self.playerItem removeObserver:self forKeyPath:@"status"];
        
        if(self.player) [self.player release];
        if(self.playerItem) [self.playerItem release];
        //if(self.playerLayer) [self.playerLayer release];
        if(!deallocWhenReady) [super dealloc];
   */
        
        if ( _frameBuffer ) {
            free(_frameBuffer);
            _frameBuffer = nil;
        }
        [super dealloc];
    }
}

- (BOOL) isLoading { return loading; }
- (BOOL) isReady { return ready; }

- (CGSize) size {
    return videoSize;
}

- (CMTime) duration {
    return videoDuration;
}

- (float) time {
    return CMTimeGetSeconds([self.player currentTime]);
}

- (float) fps {
    return videoFps;
}

- (void) play {
    [self.player play];
}

- (void) stop {
    [self.player pause];
}

- (void) setPixelFormat:(AVFPixelFormat)format {
    _format = format;
    
    if ( (format == AVF_RGB) || (format == AVF_BGR) )
        _channelCount = 3;
    else if ( (format == AVF_RGBA) || (format == AVF_BGRA) )
        _channelCount = 4;
    // For now, nothing else supported
    else
        _channelCount = 3;
}

- (void) setFps:(float)desiredFps {
    
    // Set rate to achieve the desired fps (rate it is a multiplier over playback speed)
    float rate = desiredFps/self.fps;
    self.player.rate = rate;
    
    // Store the rate (just in case it didn't readh
    videoFps = self.player.rate * videoFps;
}
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary *)change context:(void *)context
{
    if (context == AVPlayerItemStatusContext) {
        
        if ( (object == _playerItem) && [keyPath isEqualToString:@"status"]) {
            AVPlayerStatus status = [_playerItem status];
            switch (status) {
                case AVPlayerItemStatusUnknown:
                    NSLog( @"-- AVPlayerItemStatusUnknown" );
                    break;
                case AVPlayerItemStatusReadyToPlay:
                    NSLog( @"-- player Item: AVPlayerItemStatusReadyToPlay" );
                    break;
                case AVPlayerItemStatusFailed:
                    NSLog( @"-- AVPlayerItemStatusFailed" );
                    //[self stopLoadingAnimationAndHandleError:[[_player currentItem] error]];
                    break;
            }
     
        }
        else if ( (object == _player) && [keyPath isEqualToString:@"status"]) {
            AVPlayerStatus status = [_player status];
            switch (status) {
                case AVPlayerItemStatusUnknown:
                    NSLog( @"-- AVPlayerItemStatusUnknown" );
                    break;
                case AVPlayerItemStatusReadyToPlay:
                    NSLog( @"-- AVPlayerItemStatusReadyToPlay" );
                    
                    NSArray *tracks = [[_player currentItem] tracks] ;
                    AVAssetTrack *videoTrack = [[tracks objectAtIndex:0] assetTrack];
                    videoSize       = [videoTrack naturalSize];
                    videoDuration   = [[videoTrack asset] duration];
                    videoFps        = [videoTrack nominalFrameRate];
                 
                    BOOL playable = _asset.playable;
                    if ( !playable )
                        NSLog(@"AVFPlayer warnign: track is not playable");
                    
                    ready = YES;
                    break;
                case AVPlayerItemStatusFailed:
                    NSLog( @"-- AVPlayerItemStatusFailed" );
                    //[self stopLoadingAnimationAndHandleError:[[_player currentItem] error]];
                    break;
            }
            
        }

    }
    else {
        [super observeValueForKeyPath:keyPath ofObject:object change:change context:context];
    }
}


- (void) playerItemDidReachEnd:(NSNotification *) notification {
    [self.player seekToTime:kCMTimeZero];
    [self.player play];
}

- (void) update {

    // Get the current time
    CMTime currentTime = [[self videoOutput] itemTimeForHostTime:CACurrentMediaTime()];
    
   // NSLog(@"Current Time")
    // Ger Frames
    if ([[self videoOutput] hasNewPixelBufferForItemTime:currentTime] == YES) {
        CVPixelBufferRef newFrame = [[self videoOutput] copyPixelBufferForItemTime:currentTime itemTimeForDisplay:NULL];
        
        if ( newFrame ) {
                  
            // Get buffer info
            CVReturn lock = CVPixelBufferLockBaseAddress(newFrame, kCVPixelBufferLock_ReadOnly);
            if ( lock == kCVReturnSuccess ) {
                
                uint8_t *pixelBuffer = (uint8_t*)CVPixelBufferGetBaseAddress(newFrame);
                if ( pixelBuffer ) {
                    
                    //NSLog( @" New Frame!!" );
                    
                    // Get buffer low level data
                    size_t bytesPerRow = CVPixelBufferGetBytesPerRow(newFrame);
                                        
                    _widthStep = bytesPerRow;
                    size_t w = CVPixelBufferGetWidth(newFrame);
                    size_t h = CVPixelBufferGetHeight(newFrame);
                    
                    //size_t totalSize = CVPixelBufferGetDataSize((CVPixelBufferRef)newFrame);
                    
                    // adn the pixel formar
                    //OSType pixelFormat = CVPixelBufferGetPixelFormatType (newFrame);
                    
                    int depth = _channelCount;
                    int extraBytesPaddingPerRow = bytesPerRow - w*depth;
                    int expectedBytesPerRow = w*depth;
                    
                    // TODO: optimize:
                    // If it is the first frame, init the internal buffer
                    size_t bufferSize = expectedBytesPerRow * h * sizeof(char);
                    if ( _frameBuffer == nil ) {
                        _frameBuffer = (uint8_t*)malloc(bufferSize);
                    }
                    
                    // Now copy the data
                    if ( bytesPerRow == expectedBytesPerRow )
                    {
                        memcpy(_frameBuffer, pixelBuffer, bytesPerRow * h);
                    }
                    // There is padding in this buffer, copy per row considering padding
                    else
                    {
                        // Copy buffer data considering that AVF adds padding per row to make it divisible by 16
                        for(int i = 0; i < h; ++i)
                        {
                            void* src = (uint8_t *) pixelBuffer + i * ((w * depth) + extraBytesPaddingPerRow);
                            void *dst = (uint8_t *) _frameBuffer + i * (w * depth);
                            memcpy(dst, src, w * depth);
                        }
                    }
                    
                    _newFrameReady = true;
                }
                CVPixelBufferUnlockBaseAddress(newFrame, kCVPixelBufferLock_ReadOnly);
                CVPixelBufferRelease(newFrame);
            }
            
        }

    }
}

- (bool) newFrameReady {
    return _newFrameReady;
}

- (unsigned int) widthStep {
    return _widthStep;
}

- (uint8_t*) lastFrameBuffer {
    
    // Clear the flag and return the pointer
    _newFrameReady = false;
    return _frameBuffer;
}


/*
- (void)addTimeObserverToPlayer
{
    
     //Adds a time observer to the player to periodically refresh the time label to reflect current time.
 
    if (_timeObserver)
        return;
    
    _timeObserver = [_player addPeriodicTimeObserverForInterval:CMTimeMakeWithSeconds(1, 10) queue:dispatch_get_main_queue() usingBlock:
                     ^(CMTime time) {
                         NSLog( @"--- addTimeObserverToPlayer called" );
                         
                         
                         CMTime currentTime = [[self videoOutput] itemTimeForHostTime:CACurrentMediaTime()];
                         
                         NSLog( @"CURRENT TIME = %f", CMTimeGetSeconds(currentTime));
                         
                         
                         if ([[self videoOutput] hasNewPixelBufferForItemTime:currentTime]) {
                             CVPixelBufferRef pixelBuffer = NULL;
                             pixelBuffer = [[self videoOutput] copyPixelBufferForItemTime:currentTime itemTimeForDisplay:NULL];
                             
                             NSLog( @"**NEW FRAME" );
                             
                         }
                         
                         //[self syncTimeLabel];
                     }];
}
*/
/*

- (void)removeTimeObserverFromPlayer
{
    if (_timeObserver)
    {
        [_player removeTimeObserver:_timeObserver];
        _timeObserver = nil;
    }
}
*/
/*
- (void)addDidPlayToEndTimeNotificationForPlayerItem:(AVPlayerItem *)item
{
    if (_notificationToken)
        _notificationToken = nil;
    
 
  //   Setting actionAtItemEnd to None prevents the movie from getting paused at item end. A very simplistic, and not gapless, looped playback.
     
    _player.actionAtItemEnd = AVPlayerActionAtItemEndNone;
    _notificationToken = [[NSNotificationCenter defaultCenter] addObserverForName:AVPlayerItemDidPlayToEndTimeNotification object:item queue:[NSOperationQueue mainQueue] usingBlock:^(NSNotification *note) {
        // Simple item playback rewind.
        [[_player currentItem] seekToTime:kCMTimeZero];
    }];
}
*/

#pragma mark - AVPlayerItemOutputPullDelegate
@end
