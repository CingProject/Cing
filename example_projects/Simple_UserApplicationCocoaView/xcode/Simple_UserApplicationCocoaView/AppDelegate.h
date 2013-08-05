//
//  AppDelegate.h
//  Simple_UserApplicationCocoaView
//
//  Created by shawn.roske on 8/5/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include "Cing.h"
#include "RenderSystems/GL/OSX/OgreOSXCocoaView.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>
{
    NSWindow *_window;
    OgreView *_ogreView;
}

@property (assign) IBOutlet NSWindow *window;
@property (assign) IBOutlet OgreView *ogreView;

@end
