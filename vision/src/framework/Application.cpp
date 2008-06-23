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

#include "Application.h"
#include "UserAppFunctionDeclaration.h"
#include "UserAppGlobals.h"

// Graphics 
#include "graphics/GraphicsManager.h"

// GUI
#include "gui/GUIManagerCEGUI.h"

// Input
#include "input/InputManager.h"

// Physics
#include "physics/PhysicsManager.h"

// Common
#include "common/Exception.h"
#include "common/ResourceManager.h"
#include "common/LogManager.h"

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

	// Init the log manager
	// Note: If the log manager is initalized before the Resource Manager, Ogre.log file won't be created
	Common::LogManager::getSingleton().init();

  // Init graphics manager
  Graphics::GraphicsManager::getSingleton().init();

	// Init physics manager
	Physics::PhysicsManager::getSingleton().init( Graphics::GraphicsManager::getSingleton().getSceneManager() );

  // Init input manager
  Input::InputManager::getSingleton().init();

	// Register the application as listener for mouse and keyboard	
	Input::InputManager::getSingleton().getMouse().addListener( this );
	Input::InputManager::getSingleton().getKeyboard().addListener( this );

	// Init GUI Manager
	GUI::GUIManagerCEGUI::getSingleton().init( Graphics::GraphicsManager::getSingleton().getMainWindow().getOgreWindow(), 
																						 &Graphics::GraphicsManager::getSingleton().getSceneManager() );

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

	// Release GUI Manager
	GUI::GUIManagerCEGUI::getSingleton().end();

  // Release graphics manager
  Graphics::GraphicsManager::getSingleton().end();

	// Release the resource manager
	Common::ResourceManager::getSingleton().end();
	
	// Release the log manager
	Common::LogManager::getSingleton().end();

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
 *
 * @param event Contains the information about the mouse
 */
bool Application::mouseMoved( const OIS::MouseEvent& event )
{
	// Update mouse pos global var
	Globals::mouseX = event.state.X.abs;
	Globals::mouseY = event.state.Y.abs;

  // Call user mousepressed handler
	::mouseMoved();

	return true;
}

/**
 * @internal
 * Mouse pressed handler
 *
 * @param event Contains the information about the mouse
 * @param id button that has been pressed
 */
bool Application::mousePressed( const OIS::MouseEvent& event, OIS::MouseButtonID id  )
{
	Globals::mouseButton = (Input::MouseButton) id;

  // Call user mousepressed handler
	::mousePressed();

	return true;
}


/**
 * @internal
 * Mouse pressed handler
 *
 * @param event Contains the information about the mouse
 * @param id button that has been released
 */
bool Application::mouseReleased( const OIS::MouseEvent& event, OIS::MouseButtonID id  )
{
	Globals::mouseButton = (Input::MouseButton) id;

  // Call user mousepressed handler
	::mouseReleased();

	return true;
}

/**
 * @internal
 * Key pressed handler
 *
 * @param event Contains the information about the event
 */
bool Application::keyPressed( const OIS::KeyEvent& event )
{
	// Set global variable key
	Globals::key = event.text;

  // Call user mousepressed handler
	::keyPressed();

	return true;
}

/**
 * @internal
 * Key pressed handler
 *
 * @param event Contains the information about the event
 */
bool Application::keyReleased( const OIS::KeyEvent& event )
{
	// Set global variable key
	Globals::key = event.text;

  // Call user mousepressed handler
	// TODO: see a posibility to allow the user to define these handlers or not
  //::keyReleased();

	return true;
}

} // namespace Framework