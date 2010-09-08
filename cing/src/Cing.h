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

#ifndef _Cing_H_
#define _Cing_H_
  

/**
 * \mainpage Cing Documentation
 *
 * \section intro Introduction
 * Cing is an open source C++ programming libary that can be used to create interactive audiovisual applications 
 * involving: audio, video, 2d and 3d graphics, computer vision... and much more.
 *
 * Check the <b>CingExamples.sln</b> Visual Studio solution to have an idea about how the code looks like.
 *
 * To see the documentatio of the main library areas go to the <b>namespaces</b> section.
 * 
 * More into: www.cing.cc
 */


/**
 * @internal
 * @file This is the entry include file from applications
 * It should include all the necessary files to access Cing functionality, so the users don't need to 
 * worry about including files or namespaces
 */

#include <cstdio>
#include <cstring>
#include <stdlib.h>

// Graphics
#include "graphics/Object3D.h"
#include "graphics/Box.h"
#include "graphics/Plane.h"
#include "graphics/Sphere.h"
#include "graphics/PointLight.h"
#include "graphics/GraphicsTypes.h"
#include "graphics/Image.h"
#include "graphics/GraphicsUserAPI.h"
#include "graphics/LightingUserAPI.h"
#include "graphics/Transform.h"
#include "graphics/Font.h"

// CameraInput
#include "cameraInput/OCVCamera.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "cameraInput/PVCamera.h"
#include "cameraInput/VICamera.h"
#endif

// Framework 
#include "framework/AppMain.h"
#include "framework/AppFramework.h"
#include "framework/UserAppGlobals.h"
#include "framework/FrameworkUserAPI.h"

// Common
#include "common/CommonUserAPI.h"
#include "common/CommonUtilsIncludes.h"
#include "common/CommonConstants.h"
#include "common/XMLElement.h"

// Input
#include "input/InputTypes.h"
#include "input/Mouse.h"
#include "input/Keyboard.h"

// Computer Vision
#include "computervision/BlobFinder.h"
#include "computervision/BlobTracker.h"
#include "computervision/BackgroundSubtraction.h"

// Physics
#include "physics/PhysicsUserAPI.h"
#include "physics/PhysicsBox.h"
#include "physics/PhysicsPlane.h"
#include "physics/PhysicsSphere.h"
#include "physics/PhysicsObject.h"

// Audio
#include "audio/SoundFMOD.h"
#include "audio/AudioInputFMOD.h"

// Video
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "video/MediaPlayerVLC.h"
#include "video/MediaPlayerGS.h"
#endif
#include "video/OCVMovie.h"

// GUI
#include "gui/GUIUserAPI.h"

// Midi
#include "midi/MidiIO.h"

// OGRE -> this is included because some Ogre objects are exposed to user application
// just in case an advanced user wants to use them...
#include "OgreSceneManager.h"
#include "OgreCamera.h"

// namespaces (the library user don't need to know about them...)
using namespace Cing;

// Types to make user live easier
typedef XMLElement::XMLElementArray XMLElementArray;

// Classes used by user

// Camera
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
typedef	VICamera Capture;
typedef	OCVCamera Capture;
#endif

// Sound and Mic
typedef SoundFMOD Sound;
typedef AudioInputFMOD	AudioInput;
  
// Video
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
typedef MediaPlayerVLC MediaPlayer;
typedef MediaPlayerGS MediaPlayer;
#endif


#endif // _Vision_H_
