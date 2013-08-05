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
    NSString *cocoaAppName = [NSString stringWithUTF8String:_appName];
    NSLog(@"RunApplicationCocoa for app: %@", cocoaAppName);
    
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