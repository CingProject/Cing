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

#include "UserAppGlobals.h"
#include "input/InputManager.h"

namespace Globals
{

// Window
unsigned short 	 		width  = -1;
unsigned short 	 		height = -1;

// Data folder
std::string					dataFolder = "";

// Input
char								key;
Input::KeyCode			keyCode;
Input::MouseButton	mouseButton;
int									mouseX = 0;	
int									mouseY = 0;
Input::Mouse&				mouse			= Input::InputManager::m_mouse;
Input::Keyboard&		keyboard	= Input::InputManager::m_keyboard;

// Time
double							elapsedSec			= 0;
unsigned long				elapsedMillis		= 0;
unsigned long				elapsedMicros		= 0;
double							secFromStart		= 0;
unsigned long	 			millisFromStart	= 0;

// Frame count
int  								frameCount      = 0;
float								frameRate       = 0.0f;   

// Advanced
Ogre::SceneManager*	ogreSceneManager	= NULL;
Ogre::Camera*				ogreCamera				= NULL;

} // namespace Globals