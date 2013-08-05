//
//  AppDelegate.m
//  Simple_UserApplicationCocoaView
//
//  Created by shawn.roske on 8/5/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#import "AppDelegate.h"

@implementation AppDelegate

@synthesize window = _window;
@synthesize ogreView = _ogreView;

- (void)dealloc
{
    [super dealloc];
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Insert code here to initialize your application
    
    CREATE_COCOA_VIEW_APPLICATION( "Cing Demo", self.ogreView );
}

@end
