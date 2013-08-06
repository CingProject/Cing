//
//  CingInputView.m
//  Cing
//
//  Created by shawn.roske on 8/5/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#import "CingInputView.h"

@implementation CingInputView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

- (void)mouseDown:(NSEvent *)theEvent
{
    NSLog(@"mouseDown: %@", theEvent);
    
    [super mouseDown:theEvent];
}

- (void)mouseMoved:(NSEvent *)theEvent
{
    NSLog(@"mouseMoved: %@", theEvent);
    
    [super mouseMoved:theEvent];
}

- (void)mouseUp:(NSEvent *)theEvent
{
    NSLog(@"mouseUp: %@", theEvent);
    
    [super mouseUp:theEvent];
}

- (void)keyDown:(NSEvent *)theEvent
{
    NSLog(@"keyDown: %@", theEvent);
    
    [super keyDown:theEvent];
}

- (void)keyUp:(NSEvent *)theEvent
{
    NSLog(@"keyUp: %@", theEvent);
    
    [super keyUp:theEvent];
}

@end
