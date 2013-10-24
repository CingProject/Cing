//
//  AVFVideoWriter.m
//  Cing
//
//  Created by Julio Obelleiro on 10/23/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#import "AVFVideoWriter.h"
#import <AVFoundation/AVFoundation.h>
#include "common/MathUtils.h"

@implementation AVFVideoWriter


- (void)initWithURL:(NSURL*)path width:(unsigned int)width height:(unsigned int)height {
    
    unsigned long   defaultAvgBitRage = 15728640; // 15Mbps
    float           defaultFps = 30.0f;
    unsigned long   defaultKeyFrameInterval = defaultFps;
    [self initWithURL:path width:width height:height fps:defaultFps avgBitRate:defaultAvgBitRage keyFrameInterval:defaultKeyFrameInterval];

}

- (void)initWithURL:(NSURL*)path width:(unsigned int)width height:(unsigned int)height fps:(float)fps avgBitRate:(unsigned long)avgBitRate keyFrameInterval:(unsigned int)keyFrameInterval  {
        
    NSLog(@"Write Started");
    
    NSError *error = nil;
    
    // Create the asset writer to create the file
    AVAssetWriter *videoWriter = [[AVAssetWriter alloc] initWithURL:path fileType:AVFileTypeMPEG4 error:&error];
    NSParameterAssert(videoWriter);
    
    
    // Encoding settings
    NSDictionary *codecSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                                   [NSNumber numberWithInt:avgBitRate], AVVideoAverageBitRateKey,
                                   [NSNumber numberWithInt:keyFrameInterval],AVVideoMaxKeyFrameIntervalKey,
                                   nil];
    
    NSDictionary *videoSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                                   AVVideoCodecH264, AVVideoCodecKey,
                                   codecSettings, AVVideoCompressionPropertiesKey,
                                   [NSNumber numberWithInt:width], AVVideoWidthKey,
                                   [NSNumber numberWithInt:height], AVVideoHeightKey,
                                   nil];
  
    // create the video writer input that will allow us to encode frames 
    AVAssetWriterInput* videoWriterInput = [[AVAssetWriterInput
                                             assetWriterInputWithMediaType:AVMediaTypeVideo
                                             outputSettings:videoSettings] retain];
    NSParameterAssert(videoWriterInput);
    NSParameterAssert([videoWriter canAddInput:videoWriterInput]);
    videoWriterInput.expectsMediaDataInRealTime = NO;
    [videoWriter addInput:videoWriterInput];
    
    // Create the pixel buffer adaptor to create pixel buffers later on (and to append them to the video being encoded)
    NSDictionary *sourcePixelBufferAttributesDictionary = [NSDictionary dictionaryWithObjectsAndKeys:
                                                           [NSNumber numberWithInt:kCVPixelFormatType_32ARGB], kCVPixelBufferPixelFormatTypeKey, nil];
    
    AVAssetWriterInputPixelBufferAdaptor *adaptor = [AVAssetWriterInputPixelBufferAdaptor assetWriterInputPixelBufferAdaptorWithAssetWriterInput:videoWriterInput
                                                                                                                     sourcePixelBufferAttributes:sourcePixelBufferAttributesDictionary];

    // Start writing session
    [videoWriter startWriting];
    [videoWriter startSessionAtSourceTime:kCMTimeZero];
    
    
    // TEST
    NSBundle *myBundle = [NSBundle mainBundle];
    NSString *resourcePath = [myBundle resourcePath];
    NSString *warningImagePath = [resourcePath stringByAppendingString:@"/warning.png"];
    NSImage *theImage   = [[NSImage alloc] initWithContentsOfFile:warningImagePath];

    //CGImageRef theImage = [[UIImage imageNamed:@"warning.png"] CGImage];
    //CGSize size = CGSizeMake(128, 128);
    
    // Encode samples
    __block unsigned long frameNumber = 0;
    unsigned long framesToWrite = 30;
    
    dispatch_queue_t queueVideo=dispatch_queue_create("assetVideoWriterQueue", NULL);
    [videoWriterInput requestMediaDataWhenReadyOnQueue:queueVideo usingBlock:^
    {
        
        
        while([videoWriterInput isReadyForMoreMediaData])
        {
            /*CMSampleBufferRef sampleBuffer=[assetReaderVideoOutput copyNextSampleBuffer];
            if(sampleBuffer)
            {
                [videoWriterInput appendSampleBuffer:sampleBuffer];
                CFRelease(sampleBuffer);
            }
             */
            
            CVPixelBufferRef buffer = (CVPixelBufferRef)[self pixelBufferFromNSImage:theImage];
            if (buffer && (frameNumber < framesToWrite) )
            {
                if(![adaptor appendPixelBuffer:buffer withPresentationTime:CMTimeMake(frameNumber, fps)])
                    NSLog(@"ERROR encoding frame: %@", videoWriter.error);
                else {
                    NSLog(@"Success:%ld", frameNumber);
                }
                frameNumber++;
                CFRelease(buffer);
            }
            else
            {
                [videoWriterInput markAsFinished];
                [videoWriter finishWriting];
                dispatch_release(queueVideo);
                //videoFinished=YES;
                break;
            }
        }
    }];
    
    NSLog(@"Write Ended");
}


- (CVPixelBufferRef) pixelBufferFromNSImage:(NSImage*) theImg
{
    int width = 1200;
    int height = 1600;
    char* rgba = (char*)malloc(width*height*4);
    //printf("Size of char array: %d\n", width*height*4);
    
    for(int i=0; i < width*height; ++i) {
        rgba[4*i] = (char)Cing::random(255);
        rgba[4*i+1] = (char)Cing::random(255);
        rgba[4*i+2] = (char)Cing::random(255);
        rgba[4*i+3] = (char)Cing::random(255);
    }
    
    // TODO. CHECK RELEASE CALLBACK
    CVPixelBufferRef pixelBuffer = NULL;
    CVReturn result = CVPixelBufferCreateWithBytes (kCFAllocatorDefault,
                                                    width,
                                                    height,
                                                    kCVPixelFormatType_32BGRA,
                                                    rgba,
                                                    width*4,
                                                    NULL,
                                                    NULL,
                                                    NULL,
                                                    &pixelBuffer);
    
    return pixelBuffer;
}


- (CVPixelBufferRef )pixelBufferFromCImage:(NSImage *)image size:(CGSize)size
{
    // Create the pixel buffer
    NSDictionary *options = [NSDictionary dictionaryWithObjectsAndKeys:
                             [NSNumber numberWithBool:YES], kCVPixelBufferCGImageCompatibilityKey,
                             [NSNumber numberWithBool:YES], kCVPixelBufferCGBitmapContextCompatibilityKey, nil];
    CVPixelBufferRef pxbuffer = NULL;
    CVReturn status = CVPixelBufferCreate(kCFAllocatorDefault, size.width, size.height, kCVPixelFormatType_32ARGB, (CFDictionaryRef) options, &pxbuffer);
    // CVReturn status = CVPixelBufferPoolCreatePixelBuffer(NULL, adaptor.pixelBufferPool, &pxbuffer);
    
    NSParameterAssert(status == kCVReturnSuccess && pxbuffer != NULL);
    
    CVPixelBufferLockBaseAddress(pxbuffer, 0);
    void *pxdata = CVPixelBufferGetBaseAddress(pxbuffer);
    NSParameterAssert(pxdata != NULL);
    
    // Create context to draw it
    CGColorSpaceRef rgbColorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(pxdata, size.width, size.height, 8, 4*size.width, rgbColorSpace, kCGImageAlphaNoneSkipFirst);
    NSParameterAssert(context);
    
    // Draw image on it (iOS version)
    //CGContextConcatCTM(context, CGAffineTransformMakeRotation(0));
    //CGContextDrawImage(context, CGRectMake(0, 0, CGImageGetWidth(image), CGImageGetHeight(image)), image);
    
    // Draw image on it
    NSGraphicsContext *nsctxt = [NSGraphicsContext graphicsContextWithGraphicsPort:context flipped:NO];
    [NSGraphicsContext saveGraphicsState];
    [NSGraphicsContext setCurrentContext:nsctxt];
    [image compositeToPoint:NSMakePoint(0.0, 0.0) operation:NSCompositeCopy];
    [NSGraphicsContext restoreGraphicsState];
    
    // Release stuff
    CGColorSpaceRelease(rgbColorSpace);
    CGContextRelease(context);
    
    CVPixelBufferUnlockBaseAddress(pxbuffer, 0);
    
    return pxbuffer;
}

@end
