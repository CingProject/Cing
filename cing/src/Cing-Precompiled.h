// This define allow to disable including all the precompiled headers below
#define USE_PRECOMPILED_HEADERS

#ifdef USE_PRECOMPILED_HEADERS
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


	#include <gst/gst.h>

	// any other header can be included, as usual
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN
		#include "windows.h"
	#endif
#endif
