//
//  AppDelegate.h
//  Simple_CocoaView
//
//  Created by shawn.roske on 8/4/13.
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

- (IBAction)loadCing:(id)sender;

@end
