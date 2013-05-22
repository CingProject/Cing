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

#if defined( _MSC_VER )
#include "windows.h"
#endif

#include "AppFramework.h"
#include "Application.h"
#include "UserAppGlobals.h"

// OpenCV
#include "OpenCV/cxcore.h"

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
	void RunApplication( const char* _appName )
	{

		try
		{
			// Store app name
			appName = _appName;

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
		catch( cv::Exception& e )
		{
			LOG_ERROR( "OpenCV Exception: %s", e.what() );
		}
		catch ( Exception& e ) 
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			LOG_ERROR( "Exception: %s", e.getErrorMessage().c_str() );
			//fallback();
#else
			std::cerr << "An exception has occurred: " << e.getErrorMessage();
#endif

		}
		catch(const std::exception& e) 
		{
			LOG_ERROR( " Stl Exception: %s", e.what() );
		} 
		catch (...)
		{
			LOG_ERROR( "Unidentified exception" );
		}

	}


} // namespace Cing
	