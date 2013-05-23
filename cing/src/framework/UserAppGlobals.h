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

#ifndef _UserAppGlobals_H_
#define _UserAppGlobals_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "FrameworkPrereqs.h"
#include <string>

// Input
#include "input/InputTypes.h"
#include "input/Keyboard.h"

#include "common/PerlinNoise.h"

/**
 * @file
 * @brief This file contains the global variables that can be accessed by the user application
 */
namespace Cing
{

// Forward declarations
class Mouse;
class Window;


// Window
extern int							width;				///< Window's width
extern int							height;				///< Window's height
extern String						appName;			///< Name of the application (window's title)

// Data folder
extern String						dataFolder;			///< Path to user's application data folder
extern String						cingDataFolder;		///< Cing data folder (required for the user apps to run)


// Input
extern	char						key;				///< Key being currently pressed
extern	KeyCode						keyCode;			///< Key code being pressed (allow to detect also special keys such as the arrow or function keys)
extern	int							mouseButton;		///< Mouse button being pressed (LEFT, CENTER, RIGHT)
extern	int							mouseX;				///< X coordinate of the mouse (in screen coordinates)
extern	int							mouseY;				///< Y coordinate of the mouse (in screen coordinates)
extern	Mouse&						mouse;				///< System mouse
extern	Keyboard&					keyboard;			///< System keyboard

// Time
extern double						elapsedSec;			///< Elapsed time in seconds from previous to current frame
extern unsigned long				elapsedMillis;		///< Elapsed time in milliseconds from previous to current frame
extern unsigned long				elapsedMicros;		///< Elapsed time in microseconds from previous to current frame
extern double						secFromStart;		///< Seconds elapsed since application started
extern unsigned long				millisFromStart;	///< Milliseconds elapsed since application started

// Frame count
extern unsigned long long			frameCount;			///< Actual frame number the app is rendering
extern float						frameRate;			///< Current app framerate (approx)

// Advanced 
extern Ogre::SceneManager*			ogreSceneManager;	///< Ogre Scene Manager. Almost all communication with ogre is done through this object
extern Ogre::Camera*				ogreCamera;			///< Ogre Camera used to render the scene
extern Window*						appWindow;			///< App window

// Pixel manipulation
extern std::vector < Color >		pixels;

// Other
extern PerlinNoise _noise;
}


#endif // _UserAppGlobals_H_
