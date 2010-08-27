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
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "windows.h"
#endif

#include "AppFramework.h"
#include "Application.h"
#include "UserAppGlobals.h"

// Ogre
#include "OgreException.h"

// Common includes
#include "common/Exception.h"
#include "common/LogManager.h"
#include "common/eString.h"


namespace Cing
{

/**
 * @internal
 * @brief Creates and runs an application.
 *
 * This means that it creates the application and then calls its following methods
 *      setup:     At the begining of the execution.
 *      end:       At the end of the execution.
 *      draw:      Called every frame. Here the application can draw and update its state.
 */
void RunApplication( const char* appName )
{

  try
  {
	// Store app name
	appName = appName;

	// Init application
    Application::getSingleton().initApp();

    // Enter the application loop (will finish when the application should be closed)
    Application::getSingleton().drawApp();

    // Release application
    Application::getSingleton().endApp();

  }
  catch( Ogre::Exception& e )
  {
    // TODO: pasar esto a formto propio
    #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	  if ( e.getNumber() == Ogre::Exception::ERR_FILE_NOT_FOUND)
	  {
		  LOG_ERROR( "Error: File not found: %s", e.getDescription().c_str());
	  }
	  // Non specific exception
	  else
	  {
		  LOG_ERROR( "Internal Exception: %s", e.getFullDescription().c_str() );
	  }
    #else
      std::cerr << "An exception has occurred: " << e.getFullDescription();
    #endif
  }
  catch ( Exception& e ) {
    #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      LOG_ERROR( "Internal Exception: %s", e.getErrorMessage().c_str() );
    #else
      std::cerr << "An exception has occurred: " << e.getErrorMessage();
    #endif

  }
}


} // namespace Cing
