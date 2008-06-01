/*
  This source file is part of the Vision project
  For the latest info, see http://www.XXX.org

  Copyright (c) 2008 XXX

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

#include "Application.h"
#include "UserAppFunctionDeclaration.h"
#include "UserAppGlobals.h"

// Graphics 
#include "graphics/GraphicsManager.h"

// Input
#include "input/InputManager.h"

// Physics
#include "physics/PhysicsManager.h"

// Common
#include "common/Exception.h"
#include "common/ResourceManager.h"

namespace Framework
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Application::Application():
  m_bIsValid( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Application::~Application()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @return true if the initialization was ok | false otherwise
 */
bool Application::initApp()
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;
  
  // Init random number generator seed
  //TODO: setRandomSeed( timeGetTime() )
  
	// Init the resource manager
	Common::ResourceManager::getSingleton().init();

  // Init graphics manager
  Graphics::GraphicsManager::getSingleton().init();

	// Init physics manager
	Physics::PhysicsManager::getSingleton().init( Graphics::GraphicsManager::getSingleton().getSceneManager() );

  // Init input manager
  Input::InputManager::getSingleton().init();

  // Init user application
  setup();

	// Reset timer
	m_timer.reset();
		
	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void Application::endApp()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  // End user application
  end();

  // Release input manager
  Input::InputManager::getSingleton().end();

	// Release physics manager
	Physics::PhysicsManager::getSingleton().end();

  // Release graphics manager
  Graphics::GraphicsManager::getSingleton().end();

	// Release the resource manager
	Common::ResourceManager::getSingleton().end();

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Allows user to draw and update the application
 */
void Application::drawApp()
{
  // Loop while window is open
  while( Graphics::GraphicsManager::getSingleton().getMainWindow().isClosed() == false )
  {
		// Store elapsed (and convert it to seconds)
		Globals::elapsedSec = m_timer.getMilliseconds();
		m_timer.reset();

    // Update input manager
    Input::InputManager::getSingleton().update();

    // Draw user app
    draw();

		// Update physics
		Physics::PhysicsManager::getSingleton().update( Globals::elapsedSec  );

    // Update rendering
    Graphics::GraphicsManager::getSingleton().draw();
  }
}

/**
 * @internal
 * Mouse pressed handler
 */
void Application::mousePressedEvent()
{
  // Call user mousepressed handler
  mousePressed();
}

/**
 * @internal
 * Key pressed handler
 */
void Application::keyPressedEvent()
{
  // Call user mousepressed handler
  keyPressed();
}

} // namespace Framework