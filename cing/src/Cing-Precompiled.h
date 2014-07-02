// This define allow to disable including all the precompiled headers below
#define USE_PRECOMPILED_HEADERS

#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif

#ifndef __APPLE__

#ifdef USE_PRECOMPILED_HEADERS

	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		#define WIN32_LEAN_AND_MEAN
		#define _WINSOCKAPI_
		#include <WinSock2.h>
		#include "windows.h"
	#endif

	#include <string>
	#include <sstream>
	#include <iostream>
	#include <vector>
	#include <list>

	#include "boost/config.hpp"
	
	// Ogre headers you need
	#include <Ogre.h>
	#include <OgreException.h>
	#include <OgreRoot.h>
	#include <OgreConfigFile.h>
	#include <OgreCamera.h>
	#include <OgreViewport.h>
	#include <OgreSceneManager.h>
	#include <OgreRenderWindow.h>
	#include <OgreEntity.h>
	#include <OgreWindowEventUtilities.h>
	#include <OgreTexture.h>
	#include <OgreMaterial.h>
	#include <OgrePrerequisites.h>
	#include <OgreString.h>
	#include <OgreIteratorWrappers.h>
	#include <OgreCamera.h>
	#include <OgreCommon.h>

#endif

#endif
