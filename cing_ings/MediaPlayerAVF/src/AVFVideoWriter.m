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
    
    [self finishVideo];
    [super dealloc];
}

// Configure movie and codec type

- (void)setMovieType:(NSString*)movieType {
    _movieType = movieType;
}

- (void)setVideoCodec:(NSString*)videoCodec {
    _videoCodec = videoCodec;
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
    
    // Set default values for movie and codec type of not set prior
    if ( _movieType == nil )
        _movieType = AVFileTypeMPEG4;
    if ( _videoCodec == nil )
    {
        //_videoCodec = AVVideoCodecJPEG;
        //_videoCodec = AVVideoCodecAppleProRes422;
        //_videoCodec = AVVideoCodecAppleProRes4444;
        _videoCodec = AVVideoCodecH264;
    }
    
    // Create the asset writer to create the file
    if ( _videoWriter != nil )
        [_videoWriter release];
    
    // Encoding settings
    NSDictionary *codecSettings;
    
    // NOTES on available video codecs
    // http://stackoverflow.com/questions/12602835/avassetwriting-write-in-uncompressed-format
    // https://developer.apple.com/library/mac/documentation/CoreMedia/Reference/CMFormatDescription/Reference/reference.html#//apple_ref/doc/c_ref/CMVideoCodecType
    NSDictionary *videoSettings;
    
    // Depending on codec (for example ProRes needs quicktime movie type (and no bitrate or keyframe settings)
    if ( (_videoCodec == AVVideoCodecAppleProRes4444) || (_videoCodec == AVVideoCodecAppleProRes422) ) {
        
        // Force quicktime file (due to prores) 
        _movieType = AVFileTypeQuickTimeMovie;
        videoSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                         _videoCodec, AVVideoCodecKey,
                         [NSNumber numberWithInt:width], AVVideoWidthKey,
                         [NSNumber numberWithInt:height], AVVideoHeightKey,
                         nil];
    }
    else if ( _videoCodec == AVVideoCodecJPEG )
    {
        codecSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                         [NSNumber numberWithDouble:1.0f], AVVideoQualityKey,
                         nil];
        
        videoSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                         _videoCodec, AVVideoCodecKey,
                         [NSNumber numberWithInt:width], AVVideoWidthKey,
                         [NSNumber numberWithInt:height], AVVideoHeightKey,
                         codecSettings, AVVideoCompressionPropertiesKey,
                         nil];
    }
    else
    {
        NSNumber *bitRateNumber = [NSNumber numberWithDouble:avgBitRate];
        NSNumber *keyframeIntervalNumber = [NSNumber numberWithInt:keyFrameInterval];
        NSLog(@"bitRateNumber: %@", bitRateNumber);
        NSLog(@"keyframeIntervalNumber: %@", keyframeIntervalNumber);
        
        codecSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                         bitRateNumber, AVVideoAverageBitRateKey,
                         keyframeIntervalNumber, AVVideoMaxKeyFrameIntervalKey,
                         AVVideoProfileLevelH264HighAutoLevel, AVVideoProfileLevelKey, // had to change from "AVVideoProfileLevelH264HighAutoLevel" because it was added in 10.9
                         nil];
        
        videoSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                         _videoCodec, AVVideoCodecKey,
                         [NSNumber numberWithInt:width], AVVideoWidthKey,
                         [NSNumber numberWithInt:height], AVVideoHeightKey,
                         codecSettings, AVVideoCompressionPropertiesKey,
                         nil];
    }
    
    // Build path to file and create the writer
    NSURL *fileURL = [NSURL fileURLWithPath:[path stringByStandardizingPath]];
    _videoWriter = [[AVAssetWriter alloc] initWithURL:fileURL fileType:_movieType error:&error];
    NSParameterAssert(_videoWriter);
  
    // create the video writer input that will allow us to encode frames
    _videoWriterInput = [[AVAssetWriterInput
                          assetWriterInputWithMediaType:AVMediaTypeVideo
                          outputSettings:videoSettings] retain];

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

    if ( _pixelAdaptor ) {
        [_pixelAdaptor release];
        _pixelAdaptor = nil;
    }
    if ( _videoWriter ) {
        [_videoWriter release];
        _videoWriter = nil;
    }
}

// Adds a frame to the video
- (void)addFrame:(char*)data width:(unsigned int)width height:(unsigned int)height {
    
    // Check
    if ( _videoWriter == nil ) {
        NSLog( @"ERROR attempting to encode a frame on a not valid video writer. Maybe it was not initialized or has been deallocated or the video finished?" );
        return;
    }
    
    // Double check that the size is correct
    if ( (width != _width) || (height != _height) ) {
         NSLog( @"ERROR attempting to encode frame with wrong side: Received size is: (%d, %d), Expected size is: (%d, %d)", width, height, _width, _height);
        return;
    }
    
    // Wait for the writer to be ready
    unsigned int count = 0;
    unsigned int lockLimit = 50; // total wait, 5 sec max (as each frame we can't write, we're waiting 0.1 secs in this while loop)
    while ( ([_videoWriterInput isReadyForMoreMediaData] == NO) && (count < lockLimit)) {
        NSDate *maxDate = [NSDate dateWithTimeIntervalSinceNow:0.1];
        [[NSRunLoop currentRunLoop] runUntilDate:maxDate]; // NOTE: this is better than a sleep, as this allows for another task in this same thread to complete and mark the writer as ready, whereas sleep will leave the thread on pause.
        ++count;
    }
    
    // media is not ready to receive frames
    if ( count >= lockLimit ) {
        NSLog(@"ERROR: Time out to attempt to write new frame. Media is not ready for some reason" );
        return;
    }
    

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
