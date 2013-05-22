/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

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

// Precompiled headers
#include "Cing-Precompiled.h"

#include "UserAppGlobals.h"
#include "input/InputManager.h"
#include "common/eString.h"
#include "graphics/Color.h"
#include "graphics/Window.h"

namespace Cing
{
	// Window
	int 	 				width  = -1;
	int 	 				height = -1;
	String					appName = "Cing Demo";

	// Data folder
	String					dataFolder = "";
#ifdef WIN32
	String					cingDataFolder = "../../../cing_bin/data/";
#elif __APPLE__
	String					cingDataFolder = "../Resources/cing_bin/data/";
#endif
	
	// Input
	char					key;
	KeyCode					keyCode;
	int						mouseButton;
	int						mouseX = 0;	
	int						mouseY = 0;
	Mouse&					mouse			= InputManager::m_mouse;
	Keyboard&				keyboard		= InputManager::m_keyboard;

	// Time
	double					elapsedSec		= 1;
	unsigned long			elapsedMillis	= 1;
	unsigned long			elapsedMicros	= 1;
	double					secFromStart	= 1;
	unsigned long	 		millisFromStart	= 1;

	// Frame count
	unsigned long long  	frameCount      = 0;
	float					frameRate       = 0.0f;   

	// Ogre / Advanced
	Ogre::SceneManager*		ogreSceneManager	= NULL;
	Ogre::Camera*			ogreCamera			= NULL;
	Window*					appWindow			= NULL;



	// Pixel manipulation
	std::vector < Color > pixels;

	// PerlinNoise global object
	PerlinNoise _noise = PerlinNoise(6,0.43f,1.6f,0);

} // namespace Cing