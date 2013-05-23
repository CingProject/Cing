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

// Precompiled headers
#include "Cing-Precompiled.h"


#include "FrameworkUserAPI.h"
#include "UserAppGlobals.h"
#include "graphics/GraphicsManager.h"

namespace Cing
{

/**
 * @brief Makes the application run in full screen mode
 */
void fullscreen(int width, int height, GraphicMode mode )
{
	// Resolution not specified
	if (height == -1) 
	{
		GraphicsManager::getSingleton().fullscreen();
	}
	// Resolution specified
	else
	{
		GraphicsManager::getSingleton().fullscreen();

		// Set up window and render system configuration
		GraphicsManager::getSingleton().setup( width, height, mode );

		// Init application subsystems (necessary for the user to load any graphic resource)
		Application::getSingleton().initSubSystems();		
	}
}

/**
 * @brief Defines whether the app should sync with diaplay refresh or not  
 */
void vSync( bool value/* = true*/ )
{
	GraphicsManager::getSingleton().vSync( value );
}


/**
 * @brief Sets the full screen antialiasing
 * @param fsaa Antialiasing level to be used when in full screen
 */
void FSAA( int fsaa )
{
	GraphicsManager::getSingleton().FSAA( fsaa );
}



/**
 * @brief Allows to remove the window border/frame (if set to false)
 * @param if false, the window won't have border/frame
 */
void windowBorder( bool border )
{
	GraphicsManager::getSingleton().windowBorder( border );
}

/**
 * @brief Allows to set the monitor index for the main window (allows to set windows in secondary monitors)
 * @param index Monitor index to use for main window
 */
void windowMonitorIndex	( unsigned int index  )
{
	GraphicsManager::getSingleton().windowMonitorIndex( index );
}


/**
* @brief Configures the application window size and render system used
* @note It should be called first to any other graphics related function
* @param windowWidth   width of the application's window
* @param windowHeight  height of the application's window
* @param mode          specifies the render driver to use. Default OPENGL
*/
void size( int width, int height, GraphicMode mode )
{
	// Set up window and render system configuration
	GraphicsManager::getSingleton().setup( width, height, mode );

	// Init application subsystems (necessary for the user to load any graphic resource)
	Application::getSingleton().initSubSystems();
}


/**
 * @internal
 * @brief Finishes the application
 *
 * @param
 */
void exit()
{
	Application::getSingleton().exit();
}

/**
 * @internal
 * @brief Force the application to stop running for a specified time in milliseconds
 *
 * @param milliseconds
 */
void delay( unsigned int milliseconds)
{
	Application::getSingleton().delay( milliseconds );
}

/**
 * @internal
 * @brief Force the application to continuosly call user draw() function
 *
 * @param
 */
void loop()
{
	Application::getSingleton().loop();
}

/**
 * @internal
 * @brief Force the application to stop call user draw() function
 *
 * @param
 */
void noLoop()
{
	Application::getSingleton().noLoop();
}

/**
 * @internal
 * @brief Force the application to  call user draw() function one time
 *
 * @param
 */
void redraw()
{
	Application::getSingleton().redraw();
}

/**
 * @brief Forces the application to execute at a specific frame rate( if possible)
 *
 * @param forcedFrameRate new frame rate that the application will try to achieve
 */
void setFrameRate( int frameRate )
{
	Application::getSingleton().frameRate( frameRate );
}

/**
 * Registers a plugin that will be init/updated/ended by the application framework
 */
void registerPlugin( Plugin& plugin )
{
	Application::getSingleton().registerPlugin( plugin );
}

/**
 * Returns a plugin registered in the system with a specific name. If there are several, the first will be returned.
 * @param pluginName name of the plugin to be returned.
 * @return pointer to the plugin if found, NULL if not found
 */
Cing::Plugin* getPlugin( const std::string& pluginName )
{
	return Application::getSingleton().getPlugin( pluginName );
}



} // namespace Cing
