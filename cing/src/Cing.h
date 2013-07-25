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

#ifndef _Cing_H_
#define _Cing_H_
 
/**
 * \mainpage Cing Documentation
 *
 * \section intro Introduction
 * Cing in an open source library for creative programming which bridges the elegant and intuitive syntax 
 * of Processing with the power and flexibility of C++. Cing intends to provide artists and designers with
 * a tool for innovative and accessible experimentation with advanced capabilities such as: 3D & 2D graphics
 * physics simulation, computer vision, interactivity, sound...
 *
 * Check the <b>CingExamples</b> Visual Studio solution/Xcode Project to have an idea about how the code looks like.
 * Check the <b>CingProcessingExamples</b> Visual Studio solution/XCode Project to see Processing samples running in Cing.
 *
 * To see the documentatio of the main library areas go to the <b>namespaces</b> section and then go to <b>Cing</b>
 * 
 * More info: www.cing.cc
 */


/**
 * @internal
 * @file This is the entry include file from applications
 * It should include all the necessary files to access Cing functionality, so the users don't need to 
 * worry about including files or namespaces
 */

//TODO: Review why when introducing OpenCV 2.1 all warning coming from casts appear in VSTudio
#if OGRE_COMPILER == OGRE_COMPILER_MSVC
	#define _CRT_SECURE_NO_WARNINGS
	#pragma warning (disable : 4244 ) 
	#pragma warning (disable : 4305 ) 
	#pragma warning (disable : 4748 ) 
#endif

#include <cstdio>
#include <cstring>
#include <stdlib.h>

// Graphics
#include "graphics/Object3D.h"
#include "graphics/Box.h"
#include "graphics/Plane.h"
#include "graphics/Sphere.h"
#include "graphics/PointLight.h"
#include "graphics/SpotLight.h"
#include "graphics/DirectionalLight.h"
#include "graphics/GraphicsTypes.h"
#include "graphics/Image.h"
#include "graphics/ImageAnimation.h"
#include "graphics/GraphicsUserAPI.h"
#include "graphics/LightingUserAPI.h"
#include "graphics/Transform.h"
#include "graphics/Font.h"
#include "graphics/GraphicsManager.h"
#include "graphics/Color.h"

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
#include "common/XMLSettings.h"
#include "common/MemoryDumper.h"

// Input
#include "input/InputTypes.h"
#include "input/Mouse.h"
#include "input/Keyboard.h"

// Computer Vision
#include "computervision/BlobFinder.h"
#include "computervision/BlobTracker.h"
#include "computervision/BackgroundSubtraction.h"

// Physics
//#include "physics/PhysicsUserAPI.h"
//#include "physics/PhysicsBox.h"
//#include "physics/PhysicsPlane.h"
//#include "physics/PhysicsSphere.h"
//#include "physics/PhysicsObject.h"

// Audio
//#include "audio/SoundFMOD.h"
//#include "audio/AudioInputFMOD.h"
//#include "audio/SoundOpenAL.h"

// Video
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "video/MediaPlayerVLC.h"
#endif
#include "video/MediaPlayerOCV.h"
#include "video/OCVVideoRecorder.h"


// GUI
//#include "gui/GUIUserAPI.h"

// Midi
//#include "midi/MidiIO.h"

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
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
typedef	OCVCamera Capture;
#endif

// Sound and Mic
typedef SoundFMOD Sound;
typedef AudioInputFMOD	AudioInput;

// Graphics (Processing compatibility)
typedef Color color;
  
// Video
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
//typedef MediaPlayerVLC MediaPlayer;
typedef MediaPlayerOCV MediaPlayer;
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
typedef MediaPlayerOCV MediaPlayer;
#endif

// Video writing
typedef OCVVideoRecorder VideoRecorder;

#endif // _Vision_H_
