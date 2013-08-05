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

#ifndef _APP_MAIN_H_
#define _APP_MAIN_H_

// Precompiled headers
#include "Cing-Precompiled.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include "AppFrameworkCocoa.h"
#include "AppFrameworkCocoaView.h"
#else
#include "AppFrameworkBasic.h"
#endif

namespace Cing
{

/**
 * @internal
 * Macro to create and start an application. Defines the main
 * so the user does not have to define it.
 * Example of use:
 * @code
 *  CREATE_APPLICATION( "Application Name" )
 * @endcode
 */
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 && !_CONSOLE

    #include "Windows.h"

    #define CREATE_APPLICATION( appName )                                     \
        INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)   \
        {                                                                       \
            RunApplicationBasic( appName  );                                    \
            return 0;                                                           \
        };
    
    #define CREATE_USER_APPLICATION( appName, UserAppClass )                \
    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)   \
    {                                                                       \
        UserAppClass userAppClassInstance;                                  \
        RunApplicationBasic( appName, &userAppClassInstance );              \
        return 0;                                                           \
    };

#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
    
    #define CREATE_APPLICATION( appName )                                     \
        int main()                                                              \
        {                                                                       \
            RunApplicationCocoa( appName );                                     \
            return 0;                                                           \
        };
    
    #define CREATE_USER_APPLICATION( appName, UserAppClass )                \
    int main()                                                              \
    {                                                                       \
        UserAppClass userAppClassInstance;                                  \
        RunApplicationCocoa( appName, &userAppClassInstance );              \
        return 0;                                                           \
    };
    
    #define CREATE_COCOA_VIEW_APPLICATION( appName, ogreView )                \
        RunApplicationCocoaView( appName, ogreView );
    
#else

    #define CREATE_APPLICATION( appName )                                     \
        int main()                                                              \
        {                                                                       \
            RunApplicationBasic( appName  );                                    \
            return 0;                                                           \
        };
#endif

} // namespace

#endif // _APP_MAIN_H_

