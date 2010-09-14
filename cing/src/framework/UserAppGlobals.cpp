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

#include "UserAppGlobals.h"
#include "input/InputManager.h"
#include "common/eString.h"
#include "graphics/Color.h"

namespace Cing
{
	// Window
	int 	 				width  = -1;
	int 	 				height = -1;
	String					appName = "Cing Demo";

	// Data folder
	String					dataFolder = "";
	String					cingDataFolder = "../../../cing_bin/data/";

	// Input
	char					key;
	KeyCode					keyCode;
	int						mouseButton;
	int						mouseX = 0;	
	int						mouseY = 0;
	Mouse&					mouse			= InputManager::m_mouse;
	Keyboard&				keyboard		= InputManager::m_keyboard;

	// Time
	double					elapsedSec		= 0;
	unsigned long			elapsedMillis	= 0;
	unsigned long			elapsedMicros	= 0;
	double					secFromStart	= 0;
	unsigned long	 		millisFromStart	= 0;

	// Frame count
	unsigned long  			frameCount      = 0;
	float					frameRate       = 0.0f;   

	// Ogre / Advanced
	Ogre::SceneManager*		ogreSceneManager	= NULL;
	Ogre::Camera*			ogreCamera			= NULL;


	// Pixel manipulation
	std::vector < Color > pixels;

	// PerlinNoise global object
	PerlinNoise _noise = PerlinNoise(4,.63,1.6,0);

} // namespace Cing