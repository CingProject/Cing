/* -------------------------------------------------------
Copyright (c) 2009 Alberto G. Salguero (agsh@ugr.es)

Permission is hereby granted, free of charge, to any
person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the
Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice
shall be included in all copies or substantial portions of
the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------- */

#ifndef _CameraControlSystem_Prerequisites_H_
#define _CameraControlSystem_Prerequisites_H_

/// Include external headers
#ifdef CCS_OGRE_1_7
     #include <OGRE/Ogre.h>
#else
     #include <Ogre.h>
#endif
#include <map>
#include <vector>
#include <limits>

/// Define the dll export qualifier if compiling for Windows
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
   #ifdef CCS_LIB
     #define DllExport __declspec (dllexport)
   #else
     #define DllExport __declspec (dllimport)
   #endif
#else
   #define DllExport
#endif

// Define some macros
#define CCSLOG(text) Ogre::LogManager::getSingleton().logMessage("CCS: " + Ogre::String(text));
#define CCS_DEPRECATED(text) __declspec(deprecated(text))

/// Version defines
#define CCS_VERSION_MAJOR 0
#define CCS_VERSION_MINOR 4
#define CCS_VERSION_PATCH 0

#endif
