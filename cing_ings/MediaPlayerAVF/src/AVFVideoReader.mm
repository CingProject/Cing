//
//  AVFVideoReader.m
//  Cing
//
//  Created by Julio Obelleiro on 10/14/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#import "AVFVideoReader.h"

@implementation AVFVideoReader

@synthesize fps         = _fps;
@synthesize duration    = _duration;
@synthesize frameCount  = _frameCount;
@synthesize width       = _width;
@synthesize height      = _height;


- (void) dealloc {
    free(_frameBuffer);
    _frameBuffer = nil;
    
    [_asset release];
    _asset = nil;
    
    [_assetReader release];
    _assetReader = nil;
    
    [_assetReaderOutput release];
    _assetReaderOutput = nil;
    
    _frameImg.end();
    
    [super dealloc];
}


- (Boolean) loadFile:(NSString *)filename {
    
    _frameBuffer = nil;
    
    // File loading
    NSURL *fileURL = [NSURL fileURLWithPath:[filename stringByStandardizingPath]];
    //NSLog(@"Trying to load %@", filename);
    
    // Init the asset to have access to the file
    _asset = [[AVURLAsset alloc] initWithURL:fileURL options:nil];
    
    // Create the asset reader
    NSError *error;
    _assetReader = [[AVAssetReader alloc] initWithAsset:_asset error:&error];
    if ( !_assetReader ) {
        NSLog( @"Error reading file %@", filename );
        return false;
    }
    
    // Get video track
    NSArray* video_tracks = [_asset tracksWithMediaType:AVMediaTypeVideo];
    AVAssetTrack* video_track = [video_tracks objectAtIndex:0];
    
    // Set desired video format
    NSMutableDictionary* dictionary = [[NSMutableDictionary alloc] init];
    [dictionary setObject:[NSNumber numberWithInt:kCVPixelFormatType_32BGRA] forKey:(NSString*)kCVPixelBufferPixelFormatTypeKey];
    
    // Extract video properties
    _fps = [video_track nominalFrameRate];
    _duration = CMTimeGetSeconds([_asset duration]);
    _frameCount = _duration * _fps;
    _width  = [video_track naturalSize].width;
    _height = [video_track naturalSize].height;
    
    //NSLog(@"video track %f %f %f", [video_track naturalSize].width, [video_track naturalSize].height, [video_track nominalFrameRate]);

    
    //Construct the actual track output and add it to the asset reader:
    _assetReaderOutput = [[AVAssetReaderTrackOutput alloc] initWithTrack:video_track outputSettings:dictionary];
    [_assetReader addOutput:_assetReaderOutput];
    
    // Release unnecesary objects to keep
    [dictionary release];
    dictionary = nil;
    
    
    // Start reading
    if ( [_assetReader startReading] == YES ) {
        NSLog(@"Reading video asset");
        return true;
    }
    // Couldn't start reading, review error
    else {
        NSError* error = [_assetReader error];
        NSLog(@"AVFVideoReader couldn't read requested file.  Error: [%@]", error);
        return false;
    }
    
    return false;
}

- (Cing::Image &)getNextFrame {
    
    CMSampleBufferRef buffer = nil;
    if ( [_assetReader status]==AVAssetReaderStatusReading )
        buffer = [_assetReaderOutput copyNextSampleBuffer];
    
    if ( buffer ) {
                
        // Get buffer info
        CVImageBufferRef newFrame = CMSampleBufferGetImageBuffer(buffer);
        CVReturn lock = CVPixelBufferLockBaseAddress(newFrame, kCVPixelBufferLock_ReadOnly);
        if ( lock == kCVReturnSuccess ) {
            
            uint8_t *pixelBuffer = (uint8_t*)CVPixelBufferGetBaseAddress(newFrame);
            if ( pixelBuffer ) {
                                
                // Get buffer low level data
                size_t bytesPerRow = CVPixelBufferGetBytesPerRow(newFrame);
                
                unsigned int _widthStep = bytesPerRow;
                size_t w = CVPixelBufferGetWidth(newFrame);
                size_t h = CVPixelBufferGetHeight(newFrame);
                
                //size_t totalSize = CVPixelBufferGetDataSize((CVPixelBufferRef)newFrame);
                
                // adn the pixel formar
                //OSType pixelFormat = CVPixelBufferGetPixelFormatType (newFrame);
                
                unsigned int _channelCount = 4;
                int depth = _channelCount;
                int extraBytesPaddingPerRow = bytesPerRow - w*depth;
                int expectedBytesPerRow = w*depth;
                
                // TODO: optimize:
                // If it is the first frame, init the internal buffer
                size_t bufferSize = expectedBytesPerRow * h * sizeof(char);
                if ( _frameBuffer == nil ) {
                    _frameBuffer = (uint8_t*)malloc(bufferSize);
                    _frameImg.init( w, h, Cing::BGRA );
                    _frameImg.getTexturedQuad().enableLighting(false);

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
                
                // Copy the new frame
                _frameImg.setData( _frameBuffer, w, h, Cing::BGRA, _widthStep );
                static int m_preloadedFrameCount = 0;
                m_preloadedFrameCount++;
            }
            CVPixelBufferUnlockBaseAddress(newFrame, kCVPixelBufferLock_ReadOnly);
            //CVPixelBufferRelease(newFrame); // released with CFRelease(buffer);
        }
        CFRelease(buffer);
    }
    
    return _frameImg;
}

@end
