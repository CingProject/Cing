//
//  AVFVideoWriter.m
//  Cing
//
//  Created by Julio Obelleiro on 10/23/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#import "AVFVideoWriter.h"
#include "common/MathUtils.h"

@implementation AVFVideoWriter

- (void)dealloc {
    [_videoWriter release];
    [super dealloc];
}

// Creates a video with default settings
- (void)initWithURL:(NSString*)path width:(unsigned int)width height:(unsigned int)height {
    
    unsigned long   defaultAvgBitRate = width * height * 11.7; // This is equivalent to "high" endoding setting
    float           defaultFps = 30.0f;
    unsigned long   defaultKeyFrameInterval = defaultFps;
    [self initWithURL:path width:width height:height fps:defaultFps avgBitRate:defaultAvgBitRate keyFrameInterval:defaultKeyFrameInterval];

}

// Creates a video with further control over its encoding.
- (void)initWithURL:(NSString*)path width:(unsigned int)width height:(unsigned int)height fps:(float)fps avgBitRate:(unsigned long)avgBitRate keyFrameInterval:(unsigned int)keyFrameInterval  {
        
    NSError *error = nil;
    
    // Create the asset writer to create the file
    if ( _videoWriter != nil )
        [_videoWriter release];
    
    NSURL *fileURL = [NSURL fileURLWithPath:[path stringByStandardizingPath]];

    _videoWriter = [[AVAssetWriter alloc] initWithURL:fileURL fileType:AVFileTypeMPEG4 error:&error];
    NSParameterAssert(_videoWriter);
    
    
    // Encoding settings
    NSDictionary *codecSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                                   [NSNumber numberWithDouble:avgBitRate], AVVideoAverageBitRateKey,
                                   [NSNumber numberWithInt:keyFrameInterval],AVVideoMaxKeyFrameIntervalKey,
                                   nil];
    
    NSDictionary *videoSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                                   AVVideoCodecH264, AVVideoCodecKey,
                                   [NSNumber numberWithInt:width], AVVideoWidthKey,
                                   [NSNumber numberWithInt:height], AVVideoHeightKey,
                                   codecSettings, AVVideoCompressionPropertiesKey,
                                   nil];
  
    // create the video writer input that will allow us to encode frames 
    _videoWriterInput = [[AVAssetWriterInput
                          assetWriterInputWithMediaType:AVMediaTypeVideo
                          outputSettings:videoSettings] retain];
    NSParameterAssert(_videoWriterInput);
    NSParameterAssert([_videoWriter canAddInput:_videoWriterInput]);
    _videoWriterInput.expectsMediaDataInRealTime = NO;
    [_videoWriter addInput:_videoWriterInput];
    
    // Create the pixel buffer adaptor to create pixel buffers later on (and to append them to the video being encoded)
    NSDictionary *sourcePixelBufferAttributesDictionary = [NSDictionary dictionaryWithObjectsAndKeys:
                                                           [NSNumber numberWithInt:kCVPixelFormatType_32ARGB], kCVPixelBufferPixelFormatTypeKey, nil];
    
    _pixelAdaptor = [[AVAssetWriterInputPixelBufferAdaptor assetWriterInputPixelBufferAdaptorWithAssetWriterInput:_videoWriterInput
                                                                                                                     sourcePixelBufferAttributes:sourcePixelBufferAttributesDictionary] retain];

    // Store video info
    _width = width;
    _height = height;
    _fps = fps;
    _frameCount = 0;
    
    // Start writing session
    [_videoWriter startWriting];
    [_videoWriter startSessionAtSourceTime:kCMTimeZero];
}

-(void)finishVideo {
    [_videoWriter finishWriting];
}

// Adds a frame to the video
- (void)addFrame:(char*)data width:(unsigned int)width height:(unsigned int)height {
    
    // Double check that the size is correct
    if ( (width != _width) || (height != _height) ) {
         NSLog( @"ERROR attempting to encode frame with wrong side: Received size is: (%d, %d), Expected size is: (%d, %d)", width, height, _width, _height);
        return;
    }
    
    // Wait for the writer to be ready
    // NOTE: should we add a timeout here?
    while( [_videoWriterInput isReadyForMoreMediaData] == false )
        ;

    // Append the new frame
    CVPixelBufferRef buffer = (CVPixelBufferRef)[self pixelBufferFromData:data];
    if ( buffer )
    {
        if(![_pixelAdaptor  appendPixelBuffer:buffer withPresentationTime:CMTimeMake(_frameCount, _fps)])
            NSLog(@"ERROR encoding frame: %@", _videoWriter.error);
        
        CFRelease(buffer);
    }
    
    _frameCount++;
}


- (CVPixelBufferRef) pixelBufferFromData:(char*)data {
    CVPixelBufferRef pixelBuffer = NULL;
    CVReturn result = CVPixelBufferCreateWithBytes (kCFAllocatorDefault,
                                                    _width,
                                                    _height,
                                                    kCVPixelFormatType_32BGRA,
                                                    data,
                                                    _width*4,
                                                    NULL,
                                                    NULL,
                                                    NULL,
                                                    &pixelBuffer);

    if ( result != kCVReturnSuccess ) {
        NSLog( @"Error at pixelBufferFromData creating the pixel buffer from data");
    }
    
    return pixelBuffer;
}

@end
