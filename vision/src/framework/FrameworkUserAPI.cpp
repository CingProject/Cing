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

#include "FrameworkUserAPI.h"
#include "UserAppGlobals.h"
#include "graphics/GraphicsManager.h"

namespace Framework
{

  /**
   * @brief Makes the application run in full screen mode
   */
  void fullscreen()
  {
    Graphics::GraphicsManager::getSingleton().fullscreen();
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
	  std::cout << "---EclipseTest: en setup antes de ->setup del graphics manager\n";
    // Set up window and render system configuration
    Graphics::GraphicsManager::getSingleton().setup( width, height, mode );

    std::cout << "---EclipseTest: en setup antes de ->setup del graphics manager\n";
    // Init application subsystems (necessary for the user to load any graphic resource)
    Application::getSingleton().initSubSystems();

    std::cout << "---EclipseTest: en setup despues de init subsystems\n";
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


} // namespace Framework
