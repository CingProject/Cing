/*
 This source file is part of the Cing project
 For the latest info, see http://www.cing.cc
 
 License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
 Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include "AppFrameworkCocoaView.h"

#include "Application.h"
#include "UserAppGlobals.h"

// Ogre
#include "OgreException.h"
#include "RenderSystems/GL/OSX/OgreOSXCocoaView.h"

// Common includes
#include "common/Exception.h"
#include "common/LogManager.h"
#include "common/eString.h"

// OpenCV
#include "opencv2/core/core.hpp"

// mac specific
#import <Foundation/Foundation.h>

@interface CingCocoaViewApplication : NSObject
{
    NSTimer *_timer;
}

@end

@implementation CingCocoaViewApplication

- (id)initWithName:(const char *)appName andView:(void *)view
{
    self = [super init];
    if ( self != nil )
    {
        NSString *cocoaAppName = [NSString stringWithUTF8String:appName];
        OgreView *ogreView = (OgreView *)view;
        
        NSLog(@"RunApplicationCocoaView for app: %@, view: %@", cocoaAppName, ogreView);
        
        try
        {
            // Store app name
            Cing::appName = appName;
            
            Cing::Application::getSingleton().setOgreView( view );
            
            // Init application
            Cing::Application::getSingleton().initApp();
            
            // enter main loop for app
            _timer = [[NSTimer scheduledTimerWithTimeInterval:0.02
                                                       target:self
                                                     selector:@selector(renderFrame)
                                                     userInfo:NULL
                                                      repeats:YES] retain];
            
            // listen to the app termination notification
            [[NSNotificationCenter defaultCenter] addObserver:self
                                                     selector:@selector(applicationWillTerminate)
                                                         name:NSApplicationWillTerminateNotification
                                                       object:nil];
        }
        catch( Ogre::Exception& e )
        {
            // TODO: pasar esto a formto propio
            std::cerr << "An exception has occurred: " << e.getFullDescription();
        }
        catch( cv::Exception& e )
        {
            LOG_ERROR( "OpenCV Exception: %s", e.what() );
        }
        catch ( Cing::Exception& e )
        {
            std::cerr << "An exception has occurred: " << e.getErrorMessage();
        }
        catch(const std::exception& e)
        {
            LOG_ERROR( " Stl Exception: %s", e.what() );
        }
        catch (...)
        {
            LOG_ERROR( "Unidentified exception" );
        }
    }
    return self;
}

- (void)dealloc
{
    [_timer invalidate];
    [_timer release];
    _timer = nil;
    
    [super dealloc];
}

- (void)renderFrame
{
    if ( !Cing::Application::getSingleton().shouldExit() )
        Cing::Application::getSingleton().drawOneFrame();
}

- (void)applicationWillTerminate
{
    Cing::Application::getSingleton().endApp();

    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

@end


namespace Cing
{
    
    void RunApplicationCocoaView(const char *_appName, void *_view)
    {
        [[[CingCocoaViewApplication alloc] initWithName:_appName andView:_view] autorelease];
    }
    
}

