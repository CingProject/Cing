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
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32      
#include "windows.h"    
#endif

#include "AppFramework.h"
#include "Application.h"

// Ogre
#include "externLibs/Ogre3d/include/OgreException.h"

// Common includes
#include "common/Exception.h"


namespace Framework
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
      MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
    #else
      std::cerr << "An exception has occurred: " << e.getFullDescription();
    #endif
  }
  catch ( Common::Exception& e ) {
    #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
      MessageBox( NULL, e.getErrorMessage().c_str(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
    #else
      std::cerr << "An exception has occurred: " << e.getErrorMessage();
    #endif

  }
}


} // namespace FrameWork
