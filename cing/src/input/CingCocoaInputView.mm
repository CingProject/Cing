//
//  CingCocoaInputView.m
//  Cing
//
//  Created by shawn.roske on 8/6/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#import "CingCocoaInputView.h"

@implementation CingCocoaInputView

- (void)setKeyboard:(Cing::KeyboardCocoa *)keyboard
{
    _keyboard = keyboard;
}

- (void)setMouse:(Cing::MouseCocoa *)mouse
{
    _mouse = mouse;
}

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
        // nothing
    }
    
    return self;
}

- (void)dealloc
{
    _keyboard = NULL;
    _mouse = NULL;
    
    [super dealloc];
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

#pragma mark - Mouse Events

- (Cing::MouseEvent)convertMouseEvent:(NSEvent *)theEvent
{
    NSPoint eventLoc = [theEvent locationInWindow];
    NSPoint viewLoc = [self convertPoint:eventLoc fromView:self.superview];
    
    // y needs to be transposed to match Cing coords
    viewLoc.y = self.superview.frame.size.height-viewLoc.y;
    
    return Cing::MouseEvent( (int)viewLoc.x, (int)viewLoc.y );
}

- (void)mouseDown:(NSEvent *)theEvent
{
    if ( _mouse != NULL )
        _mouse->mousePressed( [self convertMouseEvent:theEvent], (Cing::MouseButtonID)theEvent.buttonNumber );
    
    [super mouseDown:theEvent];
}

- (void)mouseMoved:(NSEvent *)theEvent
{
    if ( _mouse != NULL )
        _mouse->mouseMoved( [self convertMouseEvent:theEvent] );
    
    [super mouseMoved:theEvent];
}

- (void)mouseUp:(NSEvent *)theEvent
{
    if ( _mouse != NULL )
        _mouse->mouseReleased( [self convertMouseEvent:theEvent], (Cing::MouseButtonID)theEvent.buttonNumber );
    
    [super mouseUp:theEvent];
}

#pragma mark - Keyboard Events

- (Cing::KeyEvent)convertKeyEvent:(NSEvent *)theEvent
{
    unsigned int character = 0;
    
    if ( [[theEvent characters] length] == 0 )
        character = [[theEvent characters] characterAtIndex:0];
    
    return Cing::KeyEvent( theEvent.keyCode, character );
}

- (void)keyDown:(NSEvent *)theEvent
{
    if ( _keyboard != NULL )
        _keyboard->keyPressed( [self convertKeyEvent:theEvent] );
    
    [super keyDown:theEvent];
}

- (void)keyUp:(NSEvent *)theEvent
{
    if ( _keyboard != NULL )
        _keyboard->keyReleased( [self convertKeyEvent:theEvent] );
    
    [super keyUp:theEvent];
}

@end
