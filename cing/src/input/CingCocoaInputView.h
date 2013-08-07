//
//  CingCocoaInputView.h
//  Cing
//
//  Created by shawn.roske on 8/6/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "InputTypes.h"
#include "KeyboardCocoa.h"
#include "MouseCocoa.h"

@interface CingCocoaInputView : NSView
{
    Cing::KeyboardCocoa *_keyboard;
    Cing::MouseCocoa *_mouse;
}

- (void)setKeyboard:(Cing::KeyboardCocoa *)keyboard;
- (void)setMouse:(Cing::MouseCocoa *)mouse;

@end
