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
#include "common/CommonUtilsIncludes.h"
#include "common/ResourceManager.h"
#include "common/CommonUserAPI.h"

// Extern
#include "externLibs/PTypes/include/pasync.h"


namespace Framework
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Application::Application():
  m_bIsValid( false ), m_finish( false ), m_loop( true ), m_needUpdate( false ), m_forcedFrameRate( 0 ), m_timePerFrameMillis( 0 ) 
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Application::~Application()
{
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

  // First, init the subsystems needed by any user call (the rest will be init in the initSubSystems method
  // called when the user calls the size function

  // Init the resource manager
  Common::ResourceManager::getSingleton().init();

  // Init the log manager
  // Note: If the log manager is initalized before the Resource Manager, Ogre.log file won't be created
  Common::LogManager::getSingleton().init();

  // Init user application
  setup();

  // Check subsystems init ok
  checkSubsystemsInit();

	// Reset timer
	m_timer.reset();	
	m_absTimer.reset();

	// Set frameCount to zero
	m_frameCount = 0;
	
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
	//Physics::PhysicsManager::getSingleton().end();

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
  while( (Graphics::GraphicsManager::getSingleton().getMainWindow().isClosed() == false) && !m_finish )
  {
		// Store elapsed from timers
		Globals::elapsedMicros  = m_timer.getMicroseconds();	
		Globals::elapsedMillis	= m_timer.getMilliseconds();
		Globals::elapsedSec			=  Globals::elapsedMillis / 1000.0;
		Globals::secFromStart		= m_absTimer.getMilliseconds() / 1000.0;
		Globals::millisFromStart= m_absTimer.getMilliseconds();
		m_timer.reset();

    // Update input manager
    Input::InputManager::getSingleton().update();
		
		// Draw user app
		if (m_loop)
			draw();

		// Draw user app one time if the user calls redraw() function
		if (m_needUpdate)
		{
			draw();
			m_needUpdate = false;
		}

		// Update physics
		Physics::PhysicsManager::getSingleton().update( Globals::elapsedMillis  );

    // Update rendering
    Graphics::GraphicsManager::getSingleton().draw();

		// Update sound()???
		
		// Update frameCount
		m_frameCount++;
		Globals::frameCount  = getFrameCount();	

		// Check if we need have a forced frame rate
		if ( m_forcedFrameRate != 0 ) 
		{
	
		if ( Globals::millisFromStart < (m_timePerFrameMillis * Globals::frameCount) )
		{
			unsigned long millisToSleep = (m_timePerFrameMillis * Globals::frameCount) - Globals::millisFromStart;
			if ( millisToSleep > 0 )
				pt::psleep( millisToSleep );
		}

			/*
			m_accumulatedMicros = 0;
			if ( Globals::elapsedMillis < ( m_timePerFrameMillis + m_accumulatedMicros ) )
			{
				unsigned long sleepMillis = (m_timePerFrameMillis + m_accumulatedMicros) - Globals::elapsedMillis;
				//div_t result = div( sleepMicros, 1000 );
				//unsigned long sleepMillis = result.quot;
				//m_accumulatedMicros = 0; //result.rem;
				if ( sleepMillis > 0 )
				{
					pt::psleep( sleepMillis );
				}
			}
			*/
		}
  }
}

/**
 * @brief Initializes the subsystems needed by the application
 */
void Application::initSubSystems()
{
  // The class is now initialized (it is called now to avoid recursion lock if this function is called from
  // any of the subsystems initialized below).
  m_bIsValid = true;

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
}

/**
 * @brief Checks if the necessary subsystems for the application to run are correctly init
 * If they are not -> it inits them.
 */
void Application::checkSubsystemsInit()
{
  if ( isValid() )
    return;

  initSubSystems();
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
	Globals::mouseButton = (int) id;

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
	Globals::mouseButton = (int) id;

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
	Globals::key			= event.text;
	Globals::keyCode	= event.key;

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
	Globals::keyCode	= event.key;

  // Call user mousepressed handler
	// TODO: see a posibility to allow the user to define these handlers or not
  //::keyReleased();

	return true;
}

/**
 * @internal 
 * @brief Force the application to stop running for a specified time in milliseconds
 *
 * @param milliseconds
 */
void Application::delay( unsigned int milliseconds)
{
	pt::psleep( milliseconds );
}


/**
 * @brief Forces the application to execute at a specific frame rate (if possible).
 *
 * @param forcedFrameRate new frame rate for the application execution
 */
void Application::frameRate( int forcedFrameRate )
{
	m_forcedFrameRate = forcedFrameRate;

	// Calculate the time per frame (in microseconds) to have the specified frame rate
	m_timePerFrameMillis = 1000.0 / (double)m_forcedFrameRate;
}


} // namespace Framework