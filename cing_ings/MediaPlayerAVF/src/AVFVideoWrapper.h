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
    AVFPixelFormat          _format;
    unsigned int            _channelCount;
    
    
    BOOL loading;
    BOOL ready;
    BOOL deallocWhenReady;
}

@property (nonatomic, strong)   AVPlayer                *player;
@property (nonatomic, strong)   AVPlayerItem            *playerItem;
@property (nonatomic, strong)   AVPlayerItemVideoOutput *videoOutput;
@property (nonatomic, strong)   AVURLAsset              *asset;
@property (readonly)            BOOL                    ready;


// File control
- (void) loadFile:(NSString *)filename;
- (void) play;
- (void) stop;
- (void) update;

// Ser player settings
- (void) setPixelFormat :(AVFPixelFormat)format;
- (void) setFps         :(float)desiredFps;

// Query
- (bool)            newFrameReady;
- (uint8_t*)        lastFrameBuffer;
- (BOOL)            isReady;
- (BOOL)            isLoading;
- (CGSize)          size;
- (CMTime)          duration;
- (float)           time;
- (float)           fps;
- (unsigned int)    widthStep;

// Callbacks (internal)
- (void) playerItemDidReachEnd:(NSNotification *) notification;

// not in use currently
//- (void) render;


@end
