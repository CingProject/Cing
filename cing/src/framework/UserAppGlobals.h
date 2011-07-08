/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
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

// Pixel manipulation
extern std::vector < Color >		pixels;

// Other
extern PerlinNoise _noise;
}


#endif // _UserAppGlobals_H_
