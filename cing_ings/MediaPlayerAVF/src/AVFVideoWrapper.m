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
@synthesize videoOutput = _videoOutput;
@synthesize ready; //playerLayer, layerRenderer;

- (AVFVideoWrapper *)init {
    if (self = [super init]) {
        
        _format = AVF_BGRA;
        _lastFrameBufferAddress = 0;
    }
    return self;
}

- (void) loadFile:(NSString *)filename pixelFormat:(AVFPixelFormat)pixelFormat {
   
    // File loading
    NSURL *fileURL = [NSURL fileURLWithPath:[filename stringByStandardizingPath]];
    NSLog(@"Trying to load %@", filename);
    
    // Init the asset to have access to the file
    AVURLAsset *asset = [[AVURLAsset alloc] initWithURL:fileURL options:nil];

    // Extract movie file information
    NSArray* video_tracks = [asset tracksWithMediaType:AVMediaTypeVideo];
    AVAssetTrack *videoTrack = [video_tracks objectAtIndex:0];
    
    videoSize       = [videoTrack naturalSize];
    videoDuration   = [asset duration];
    videoFps        = [videoTrack nominalFrameRate];
    videoFrameCount = videoFps * CMTimeGetSeconds(videoDuration);
    
    // Set the requested pixel format
    [self setPixelFormat:pixelFormat];
    
    // Load it when it is playable and the tracks are ready (to be able to acces frames)
    NSArray *requestedKeys = [NSArray arrayWithObjects:kTracksKey, kPlayableKey, nil];
    [asset loadValuesAsynchronouslyForKeys:requestedKeys completionHandler:^{
        dispatch_async(dispatch_get_main_queue(), ^{
            [self setupPlaybackWithAsset:asset];
        });
    }];
    
    _useSeekTime = NO;
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
        
        // Callbacks
        [_playerItem addObserver:self forKeyPath:@"status" options:0 context:AVPlayerItemStatusContext];
        [_player addObserver:self forKeyPath:@"status" options:0 context:AVPlayerItemStatusContext];
        
    }
    
    _useSeekTime = NO;
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
        
        // Release callbacks
        [[NSNotificationCenter defaultCenter] removeObserver:self name:AVPlayerItemDidPlayToEndTimeNotification object:nil];
        [_playerItem removeObserver:self forKeyPath:@"status"];
        [_player removeObserver:self forKeyPath:@"status"];
        
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
    if ( _useSeekTime )
        return CMTimeGetSeconds(seekTime);
    
    return CMTimeGetSeconds([self.player currentTime]);
}

- (float) fps {
    return videoFps;
}

// Returns the frame number of the last pixel buffer that was copied to the current Image
- (unsigned int)  currentFrameNumber {
    return currentFrameNumber;
}

- (unsigned int) frameCount {
    return videoFrameCount;
}

- (void) play {
    [self.player play];
    _useSeekTime = NO;
}

- (void) pause {
    [self.player pause];
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

- (void) setRate:(float)rate {
    [_player setRate:rate];
}

- (void) setCurrentTime :(float)timeSecs {
    
    seekTime = CMTimeMake(timeSecs, 1);
    [self.player seekToTime:seekTime];
    _useSeekTime = YES;
}

- (void) setCurrentFrame: (unsigned int)frameNumber {
    
    _useSeekTime = YES;
    
    double timePerFrame = 1.0 / videoFps;
    
    // See where in seconds this frame number falls
    double timeSecs = ((double)frameNumber / (double)videoFrameCount) * CMTimeGetSeconds(videoDuration);
    
    // Calculate the seek time (scale in microseconds, as the CMTime only uses int numbers to represent time/scale)
    seekTime = CMTimeMake((timeSecs + (timePerFrame*0.5)) * 1000000000, 1000000000);
    
    // LOG INFO
    //CMTime currentTime = [_playerItem currentTime];
    //unsigned int currentFrame = round((CMTimeGetSeconds(currentTime) /  CMTimeGetSeconds(videoDuration)) * (double)videoFrameCount);
    //NSLog( @"--Requested jump from time %f, to time %f, from frame: %d, to frame: %d", CMTimeGetSeconds(currentTime), CMTimeGetSeconds(seekTime), currentFrame, frameNumber);
    
    // Perform the seek
    [_player seekToTime:seekTime toleranceBefore:kCMTimeZero toleranceAfter:kCMTimeZero];   
}
- (void) nextFrame {
    
    _useSeekTime = YES;
    
    // Get current time
    CMTime currentTime = [_playerItem currentTime];
    
    // Calculate one frame's time
    float secs = 1.0 / videoFps;
    CMTime oneFrame = CMTimeMake( secs * 1000000, 1000000 );

    // Add both to know next frame's time
    CMTimeShow(oneFrame);
    CMTime added = CMTimeAdd(currentTime, oneFrame);
    
    // Perform seek (it's asyncrhonous, so we shold confirm we have a frame ready before knowing for sure we can use it).
    seekTime = added;
    [_player seekToTime:added toleranceBefore:kCMTimeZero toleranceAfter:kCMTimeZero];
    
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
    [self play];
}

- (void) update {

    // Get the current time
    CMTime currentTime;
    
    if ( _useSeekTime )
        currentTime = seekTime;
    else
        currentTime = [[self videoOutput] itemTimeForHostTime:CACurrentMediaTime()];
    
    // Get Frames
    //if ([[self videoOutput] hasNewPixelBufferForItemTime:currentTime] == YES) {
        [self copyNewPixelBuffer:currentTime];
    //}
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

- (void) copyNewPixelBuffer:(CMTime)currentTime {
    
    CVPixelBufferRef newFrame = [[self videoOutput] copyPixelBufferForItemTime:currentTime itemTimeForDisplay:NULL];
    
    unsigned int newFrameAddress = (unsigned int) newFrame;
    
    
    if ( newFrame && (_lastFrameBufferAddress != newFrameAddress) ) {
        
        //NSLog(@"------New Frame received for time: %f, for address: %d", CMTimeGetSeconds(currentTime), newFrame);
        //CMTimeShow(currentTime);
        
        // Get buffer info
        CVReturn lock = CVPixelBufferLockBaseAddress(newFrame, kCVPixelBufferLock_ReadOnly);
        if ( lock == kCVReturnSuccess ) {
            
            uint8_t *pixelBuffer = (uint8_t*)CVPixelBufferGetBaseAddress(newFrame);
            if ( pixelBuffer ) {
                
                
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
                
                // New frame flag and store the number of frame that was just copied
                _newFrameReady = true;
                currentFrameNumber = round((CMTimeGetSeconds(currentTime) / CMTimeGetSeconds(videoDuration)) * (double)videoFrameCount);
                
                // Store the address of the buffer to avoid retrieving the same buffer in the future.
                _lastFrameBufferAddress = newFrameAddress;
            }
            CVPixelBufferUnlockBaseAddress(newFrame, kCVPixelBufferLock_ReadOnly);
            CVPixelBufferRelease(newFrame);
        }
        
    }
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
