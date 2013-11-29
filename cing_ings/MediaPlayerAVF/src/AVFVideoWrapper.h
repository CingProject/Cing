//
//  AVFVideoWrapper.h
//  Cing
//
//  Created by Julio Obelleiro on 8/23/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>
#import <AVFoundation/AVFoundation.h>
#import <OpenGL/OpenGL.h>

typedef enum _AVFPixelFormat {
    AVF_RGB, // Default value is setFormat is not called before load
    AVF_RGBA,
    AVF_BGR,
    AVF_BGRA,
} AVFPixelFormat;

@interface AVFVideoWrapper : NSObject <AVPlayerItemOutputPullDelegate>
{
    AVURLAsset              *_asset;
    AVPlayer                *_player;
    AVPlayerItem            *_playerItem;
    AVPlayerItemVideoOutput *_videoOutput;
    unsigned int            _widthStep;
    
    bool                    _newFrameReady;
    uint8_t                 *_frameBuffer;
    
    
    CVPixelBufferRef        _pixelBuffer;
    
    CGSize                  videoSize;
    CMTime                  videoDuration;
    float                   videoFps;
    unsigned int            videoFrameCount;
    unsigned int            currentFrameNumber; // Frame number for the last pixe buffer that was copied/decoded from the movie file
    AVFPixelFormat          _format;
    unsigned int            _channelCount;
    
    CMTime                  seekTime;
    BOOL                    _useSeekTime; // If this is true the current frame will come from the time set in the sekk call and not from regular playback time.
    BOOL                    loading;
    BOOL                    ready;
    
    BOOL                    deallocWhenReady;
    
    // NOTE: storing the address of the last received buffer, to avoid repeating buffers (this is done as hasNewPixelBufferForItemTime:currentTime is not behaving as expected
    // and often does not return
    unsigned int            _lastFrameBufferAddress;
    
}

@property (nonatomic, strong)   AVPlayer                *player;
@property (nonatomic, strong)   AVPlayerItem            *playerItem;
@property (nonatomic, strong)   AVPlayerItemVideoOutput *videoOutput;
@property (nonatomic, strong)   AVURLAsset              *asset;
@property (readonly)            BOOL                    ready;


// File control
- (Boolean) loadFile:(NSString *)filename pixelFormat:(AVFPixelFormat)pixelFormat;
- (void) play;
- (void) stop;
- (void) pause;
- (void) update;

// Ser player settings
- (void) setPixelFormat :(AVFPixelFormat)format;
- (void) setFps         :(float)desiredFps;
- (void) setRate:(float)rate;
- (void) setCurrentTime :(float)timeSecs;
- (void) setCurrentFrame: (unsigned int)frameNumber;
- (void) nextFrame;


// Query
- (bool)            newFrameReady;
- (uint8_t*)        lastFrameBuffer;
- (BOOL)            isReady;
- (BOOL)            isLoading;
- (CGSize)          size;
- (CMTime)          duration;
- (float)           time;
- (float)           fps;
- (unsigned int)    currentFrameNumber;
- (unsigned int)    frameCount;
- (unsigned int)    widthStep;

// Callbacks (internal)
- (void) playerItemDidReachEnd:(NSNotification *) notification;

// not in use currently
//- (void) render;


@end
