/*
  This source file is part of the Vision project
  For the latest info, see http://www.playthemagic.com/vision

Copyright (c) 2008 Julio Obelleiro and Jorge Cano

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

#include "FrameworkPrereqs.h"
#include <string>

// Input
#include "input/InputTypes.h"
#include "input/Keyboard.h"

#include "graphics/Color.h"
#include "common/PerlinNoise.h"

// Forward declarations
namespace	Input
{
	class Mouse;
}

/**
 * @file
 * @brief This file contains the global variables that can be accessed by the user application
 */
namespace Globals
{

// Window
extern unsigned short				width;					///< Window's width
extern unsigned short				height;					///< Window's height

// Data folder
extern std::string					dataFolder;			///< Path to user's application data folder

// Input
extern	char								key;						///< Key being currently pressed
extern	Input::KeyCode			keyCode;				///< Key code being pressed (allow to detect also special keys such as the arrow or function keys)
extern	int									mouseButton;		///< Mouse button being pressed (LEFT, CENTER, RIGHT)
extern	int									mouseX;					///< X coordinate of the mouse (in screen coordinates)
extern	int									mouseY;					///< Y coordinate of the mouse (in screen coordinates)
extern	Input::Mouse&				mouse;					///< System mouse
extern	Input::Keyboard&		keyboard;				///< System keyboard

// Time
extern double								elapsedSec;			///< Elapsed time in seconds from previous to current frame
extern unsigned long				elapsedMillis;	///< Elapsed time in milliseconds from previous to current frame
extern unsigned long				elapsedMicros;	///< Elapsed time in microseconds from previous to current frame
extern double								secFromStart;		///< Seconds elapsed since application started
extern unsigned long				millisFromStart;///< Milliseconds elapsed since application started

// Frame count
extern unsigned long  			frameCount;			///< Actual frameRate of app (aproximated)
extern float								frameRate;

// Advanced 
extern Ogre::SceneManager*	ogreSceneManager; ///< Ogre Scene Manager. Almost all communication with ogre is done through this object
extern Ogre::Camera*				ogreCamera;				///< Ogre Camera used to render the scene

// Pixel manipulation
extern std::vector < Graphics::Color > pixels;

// Other
extern PerlinNoise _noise;
}


#endif // _UserAppGlobals_H_
