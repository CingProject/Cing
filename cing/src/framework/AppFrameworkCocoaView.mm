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
//#include "RenderSystems/GL/OSX/OgreOSXCocoaWindow.h"


// Common includes
#include "common/Exception.h"
#include "common/LogManager.h"
#include "common/eString.h"

#include "input/InputManagerCocoa.h"

#include "graphics/GraphicsManager.h"

// OpenCV
#include "opencv2/core/core.hpp"

// mac specific
#import <Foundation/Foundation.h>

@interface CingCocoaViewApplication : NSObject
{
    NSTimer *_timer;
    Cing::InputManagerCocoa *_inputManager;
    
    // CVDisplayLink related
    CVDisplayLinkRef    *_displayLinkOut;
    NSOpenGLContext     *_GLContext;
}

@end

@implementation CingCocoaViewApplication

/*
 more about display link http://pastebin.com/PrkLvJKY
 http://iphonedevsdk.com/forum/iphone-sdk-game-development/42309-the-best-game-loop.html
// This is the renderer output callback function when using CVDisplayLink for the render loop
static CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp* now, const CVTimeStamp* outputTime, CVOptionFlags flagsIn, CVOptionFlags* flagsOut, void* displayLinkContext)
{
    CVReturn result = [(__bridge glView*)displayLinkContext getFrameForTime:outputTime];
    return result;
}

 */

- (id)initWithName:(const char *)appName andView:(void *)view andUserApp:(Cing::UserApplicationBase *)userApp
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
            
            //_inputManager = new Cing::InputManagerCocoa();
            //_inputManager->setParentView( ogreView );
            _inputManager = NULL;

            // Init application
            Cing::Application::getSingleton().initApp( _inputManager, (Cing::UserApplicationBase *)userApp );
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
        
        // enter main loop for app
        bool useDisplayLink = false;
        
        // Using Timer
        if ( useDisplayLink == false )
        {
            _timer = [[NSTimer scheduledTimerWithTimeInterval:1.0f/30.0f
                                                       target:self
                                                     selector:@selector(renderFrame)
                                                     userInfo:NULL
                                                      repeats:YES] retain];
            
        }
        // Using Display Link
        else
        {
            /*
            // Store pointer to Ogre's open gl context
            _GLContext = nil;
            Ogre::RenderWindow *ogreWindow =  GraphicsManager::getSingleton().getMainWindow().getOgreWindow();
            if ( ogreWindow )
            {
                Ogre::OSXCocoaWindow* videoOutputCocoa  = static_cast<Ogre::OSXCocoaWindow*>(ogreWindow);
                _GLContext = videoOutputCocoa->nsopenGLContext();
            }
            
            if ( _GLContext )
            {
                // Create a display link capable of being used with all active displays
                CVDisplayLinkCreateWithActiveCGDisplays(_displayLinkOut);
                
                // Set the renderer output callback function
                CVDisplayLinkSetOutputCallback(_displayLinkOut, &MyDisplayLinkCallback, (void*)self);
                
                // Set the display link for the current renderer
                CGLContextObj cglContext = (CGLContextObj) [_GLContext CGLContextObj];
                CGLPixelFormatObj cglPixelFormat = [[ogreView pixelFormat] CGLPixelFormatObj];
                CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(_displayLinkOut, cglContext, cglPixelFormat);
                
                // Activate the display link
                CVDisplayLinkStart(m_displayLink);
                
              
                /////////////////////////

#if PD_USE_DISPLAY_LINK == 1
                static CVReturn displayLinkCallback(CVDisplayLinkRef displayLink,
                                                    const CVTimeStamp* now,
                                                    const CVTimeStamp* outputTime,
                                                    CVOptionFlags,
                                                    CVOptionFlags*,
                                                    void* displayLinkContext)
                {
                    PDOgreRenderSystem * renderSystem =
                    (__bridge PDOgreRenderSystem *) displayLinkContext;
                    
                    @autoreleasepool {
                        
                        if(!renderSystem.isStopped &&
                           Ogre::Root::getSingletonPtr() &&
                           Ogre::Root::getSingleton().isInitialised()) {
                            
                            // Get OpenGL context from render window
                            NSOpenGLContext *ctx =
                            static_cast<Ogre::CocoaWindow const *>(renderSystem.renderWindow)->nsopenGLContext();
                            
                            CGLContextObj cglContext = (CGLContextObj) [ctx CGLContextObj];
                            
                            // lock the context
                            CGLLockContext(cglContext);
                            
                            // compute seconds elapsed since last call
                            float deltaTime = 1.0 / (outputTime->rateScalar * (float)outputTime->videoTimeScale / (float)outputTime->videoRefreshPeriod);
                            
                            // Let Ogre render the next frame
                            [ctx makeCurrentContext];
                            
                            Ogre::Root::getSingleton().renderOneFrame(deltaTime);
                            
                            CGLUnlockContext(cglContext);
                        }
                    }
                    
                    return kCVReturnSuccess;
                }
                /////////////////////////
            }
             */
        }

        // listen to the app termination notification
        [[NSNotificationCenter defaultCenter] addObserver:self
                                                 selector:@selector(applicationWillTerminate)
                                                     name:NSApplicationWillTerminateNotification
                                                   object:nil];
    }
    return self;
}

- (void)dealloc
{
    [_timer invalidate];
    [_timer release];
    _timer = nil;
    
    delete _inputManager;
    _inputManager = NULL;
    
    [super dealloc];
}

- (void)renderFrame
{
    if ( !Cing::Application::getSingleton().shouldExit() )
        Cing::Application::getSingleton().drawOneFrame();
}

/*
// Callback function use when CVDisplayLink controls the render loop
- (CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
    Ogre::RenderSystem *renderSystem = Ogre::Root::getSingleton().getRenderSystem();
    
    @autoreleasepool {
        
        if(Ogre::Root::getSingletonPtr() &&
           Ogre::Root::getSingleton().isInitialised()) {
            
            // Get OpenGL context from render window
            NSOpenGLContext *ctx =            static_cast<Ogre::CocoaWindow const *>(renderSystem.renderWindow)->nsopenGLContext();
            
            CGLContextObj cglContext = (CGLContextObj) [ctx CGLContextObj];
            
            // lock the context
            CGLLockContext(cglContext);
            
            // compute seconds elapsed since last call
            float deltaTime = 1.0 / (outputTime->rateScalar * (float)outputTime->videoTimeScale / (float)outputTime->videoRefreshPeriod);
            
            // Let Ogre render the next frame
            [ctx makeCurrentContext];
            
            Ogre::Root::getSingleton().renderOneFrame(deltaTime);
            [self renderFrame];
            
            CGLUnlockContext(cglContext);
        }
    }
    
    return kCVReturnSuccess;

}
 */


- (void)applicationWillTerminate
{
    Cing::Application::getSingleton().endApp();

    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

@end


namespace Cing
{
    
    void RunApplicationCocoaView(const char *_appName, void *_view, UserApplicationBase *_userApp /*= NULL*/ )
    {
        [[[CingCocoaViewApplication alloc] initWithName:_appName andView:_view andUserApp:(UserApplicationBase *)_userApp] autorelease];
    }
    
}

