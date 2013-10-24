//
//  AVFVideoWriter.h
//  Cing
//
//  Created by Julio Obelleiro on 10/23/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface AVFVideoWriter : NSObject {
    
}

- (void)initWithURL:(NSURL*)path width:(unsigned int)width height:(unsigned int)height;
- (void)initWithURL:(NSURL*)path width:(unsigned int)width height:(unsigned int)height fps:(float)fps avgBitRate:(unsigned long)avgBitRate keyFrameInterval:(unsigned int)keyFrameInterval;

@end
