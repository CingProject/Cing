//
//  AppFrameworkCocoa.m
//  Cing
//
//  Created by shawn.roske on 8/2/13.
//  Copyright (c) 2013 Cing Project. All rights reserved.
//

#import "AppFrameworkCocoa.h"

#include "Application.h"
#include "UserAppGlobals.h"

// Ogre
#include "OgreException.h"

// Common includes
#include "common/Exception.h"
#include "common/LogManager.h"
#include "common/eString.h"

// OpenCV
#include "opencv2/core/core.hpp"

// mac specific
#import <Foundation/Foundation.h>

namespace Cing
{

void RunApplicationCocoa(const char *_appName)
{
    NSString *an = [NSString stringWithUTF8String:_appName];
    NSLog(@"RunApplicationCocoa for app: %@", an);
    
    try
    {
        // Store app name
        Cing::appName = _appName;
        
        // Init application
        Cing::Application::getSingleton().initApp();
        
        // Enter the application loop (will finish when the application should be closed)
        Cing::Application::getSingleton().drawApp();
        
        // Release application
        Cing::Application::getSingleton().endApp();
        
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

}