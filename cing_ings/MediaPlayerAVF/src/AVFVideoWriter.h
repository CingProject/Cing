//
//  AVFVideoWriter.h
//  Cing
//
//  Created by Julio Obelleiro on 10/23/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface AVFVideoWriter : NSObject {
    AVAssetWriter                           *_videoWriter;
    AVAssetWriterInput                      *_videoWriterInput;
    AVAssetWriterInputPixelBufferAdaptor    *_pixelAdaptor;
    
    unsigned int        _width;
    unsigned int        _height;
    float               _fps;
    NSString            *_movieType; // Like AVFileTypeQuickTimeMovie or AVFileTypeMPEG4
    NSString            *_videoCodec; // Like AVVideoCodecH264, AVVideoCodecJPEG, AVVideoCodecAppleProRes422 or AVVideoCodecAppleProRes4444
    
    
    unsigned int        _frameCount; // Number of frames encoded in the video
}

// Configure before creating file
- (void)setMovieType:(NSString*)movieType;
- (void)setVideoCodec:(NSString*)videoCodec;

// Create files
- (void)initWithURL:(NSString*)path width:(unsigned int)width height:(unsigned int)height;
- (void)initWithURL:(NSString*)path width:(unsigned int)width height:(unsigned int)height fps:(float)fps avgBitRate:(unsigned long)avgBitRate keyFrameInterval:(unsigned int)keyFrameInterval;
- (void)finishVideo;

// Add frames. NOTE: for now only RGBA format is supported (so 4 bytes per pixel).
- (void)addFrame:(char*)data width:(unsigned int)width height:(unsigned int)height;

@end
