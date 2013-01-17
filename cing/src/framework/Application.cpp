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

// Precompiled headers
#include "Cing-Precompiled.h"


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
//#include "physics/PhysicsManager.h"

// Common
#include "common/CommonUtilsIncludes.h"
#include "common/ResourceManager.h"
#include "common/CommonUserAPI.h"

// Extern
#undef nil
#include "PTypes/include/pasync.h"

namespace Cing
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
	setRandomSeed( time(NULL) );

	// First, init the subsystems needed by any user call (the rest will be init in the initSubSystems method
	// called when the user calls the size function

	// Init the resource manager
	ResourceManager::getSingleton().init();

	// Init the log manager
	// Note: If the log manager is initalized before the Resource Manager, Ogre.log file won't be created
	LogManager::getSingleton().init();
	
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

	// End plugins that require it at this point
	endPlugins( END_BEFORE_USER );

	// End user application
	end();

	// End plugins that require it at this point
	endPlugins( END_AFTER_USER );

	// Release input manager
	InputManager::getSingleton().end();

	// Release physics manager
	//PhysicsManager::getSingleton().end();

	// Release GUI Manager
	GUIManagerCEGUI::getSingleton().end();

	// Release graphics manager
	GraphicsManager::getSingleton().end();

	// Release the resource manager
	ResourceManager::getSingleton().end();

	// Release the log manager
	LogManager::getSingleton().end();

	// End plugins that require it at this point
	endPlugins( END_AFTER_CORE );

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
	while( (GraphicsManager::getSingleton().getMainWindow().isClosed() == false) && !m_finish )
	{
		// Store elapsed from timers
		elapsedMicros	= m_timer.getMicroseconds();
		elapsedMillis	= m_timer.getMilliseconds();
		elapsedSec		=  elapsedMicros / 1000000.0;

		// security against negative elapsed (can come from thread related issues)
		if ( elapsedMicros < 0 )
			elapsedMicros = 1000;
		if ( elapsedMillis < 0 )
			elapsedMillis = 1;
		if ( elapsedSec < 0 )
			elapsedSec = 0.001;

		secFromStart	= m_absTimer.getMicroseconds() / 1000000.0;
		millisFromStart	= m_absTimer.getMilliseconds();
		m_timer.reset();

		// Update input manager
		InputManager::getSingleton().update();

		// Update plugins that require it at this point
		updatePlugins( UPDATE_BEFORE_USER );

		// Draw user app
		if (m_loop)
			draw();

		// Update plugins that require it at this point
		updatePlugins( UPDATE_AFTER_USER );

		// Draw user app one time if the user calls redraw() function
		if (m_needUpdate)
		{
			updatePlugins( UPDATE_BEFORE_USER );
			draw();
			updatePlugins( UPDATE_AFTER_USER );
			m_needUpdate = false;
		}

		// Update physics
		//PhysicsManager::getSingleton().update( elapsedMillis  );

		// Update rendering
		GraphicsManager::getSingleton().draw();

		// Update plugins that require it at this point
		updatePlugins( UPDATE_AFTER_RENDER );

		// Update sound()???

		// Update frameCount
		m_frameCount++;
		frameCount  = getFrameCount();

		// Check if we need have a forced frame rate
		if ( m_forcedFrameRate != 0 )
		{

			if ( millisFromStart < (m_timePerFrameMillis * frameCount) )
			{
				unsigned long millisToSleep = (unsigned long)((m_timePerFrameMillis * frameCount) - millisFromStart);
				if ( millisToSleep > 0 )
					pt::psleep( millisToSleep );
			}

			/*
			m_accumulatedMicros = 0;
			if ( elapsedMillis < ( m_timePerFrameMillis + m_accumulatedMicros ) )
			{
			unsigned long sleepMillis = (m_timePerFrameMillis + m_accumulatedMicros) - elapsedMillis;
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

	// Load user resource locations
	ResourceManager::getSingleton().loadUserResourceLocations();

	// Init plugins that require it at this point
	initPlugins( INIT_BEFORE_GRAPHICS );

	// Create app window (and init graphics engine)
	GraphicsManager::getSingleton().createWindow();

	// Init plugins that require it at this point
	initPlugins( INIT_BEFORE_RESOURCE_INIT );

	// Init graphics manager's resources
	GraphicsManager::getSingleton().initReSources();

	// Init plugins that require it at this point
	initPlugins( INIT_AFTER_GRAPHICS );

	// Init input manager
	InputManager::getSingleton().init();

	// Register the application as listener for mouse and keyboard
	InputManager::getSingleton().getMouse().addListener( this );
	InputManager::getSingleton().getKeyboard().addListener( this );

	// Init GUI Manager
	GUIManagerCEGUI::getSingleton().init( GraphicsManager::getSingleton().getMainWindow().getOgreWindow(),&GraphicsManager::getSingleton().getSceneManager() );

	// Init plugins that require it at this point
	initPlugins( INIT_AFTER_SUBSYSTEMS );
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
	mouseX = event.state.X.abs;
	mouseY = event.state.Y.abs;

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
	mouseButton = (int) id;

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
	mouseButton = (int) id;

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
	key			= event.text;
	keyCode	= event.key;

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
	key = event.text;
	keyCode	= event.key;

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

/**
 * @brief Forces the application to execute at a specific frame rate (if possible).
 *
 * @param forcedFrameRate new frame rate for the application execution
 */
void Application::registerPlugin( Plugin& plugin )
{
	m_plugins.push_back( &plugin );
}

/**
 * Returns a plugin registered in the system with a specific name. If there are several, the first will be returned.
 * @param pluginName name of the plugin to be returned.
 * @return pointer to the plugin if found, NULL if not found
 */
Cing::Plugin* Application::getPlugin( const std::string& pluginName )
{
	// search for a plugin with the received name (and return the first found
	for ( PluginList::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it )
	{
		Plugin* plugin = *it;
		if ( plugin && (plugin->getName() == pluginName) )
		{
			return plugin;
		}
	}

	// not found
	return NULL;
}

/**
 * Calls the init methods for all the plugins whose init time matches the received parameter  time
 *
 * @param time The pluings with init time matching this parameter will have the init() method called.
 */
void Application::initPlugins( PluginInitTime time )
{
	for ( PluginList::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it )
	{
		Plugin* plugin = *it;
		if ( plugin )
		{
			if ( plugin->getPluginInitTime() == time )
				plugin->init();
		}
		else
		{
			LOG_ERROR( "There is NULL Plugin registered in the Application." );
		}
	}
}

/**
 * Calls the end methods for all the plugins whose end time matches the received parameter  time
 *
 * @param time The pluings with end time matching this parameter will have the end() method called.
 */
void Application::endPlugins( PluginEndTime time )
{
	for ( PluginList::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it )
	{
		Plugin* plugin = *it;
		if ( plugin )
		{
			if ( plugin->getPluginEndTime() == time )
				plugin->end();
		}
		else
		{
			LOG_ERROR( "There is NULL Plugin registered in the Application." );
		}
	}
}

/**
 * Calls the update methods for all the plugins whose update time matches the received parameter  time
 *
 * @param time The pluings with update time matching this parameter will have the update() method called.
 */
void Application::updatePlugins( PluginUpdateTime time )
{
	for ( PluginList::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it )
	{
		Plugin* plugin = *it;
		if ( plugin )
		{
			if ( plugin->getPluginUpdateTime() == time )
				plugin->update();
		}
		else
		{
			LOG_ERROR( "There is NULL Plugin registered in the Application." );
		}
	}
}

} // namespace Cing
