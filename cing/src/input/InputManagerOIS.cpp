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

#include "InputManagerOIS.h"
#include "MouseOIS.h"
#include "KeyboardOIS.h"

// Graphics
#include "graphics/GraphicsManager.h"

// OIS
#include "OIS.h"

// Ogre
#include "OgreStringConverter.h"

// STL
#include <sstream>

namespace Cing
{	
	/**
	 * @internal
	 * @brief Constructor. Initializes class attributes.
	 */
	InputManagerOIS::InputManagerOIS():
        m_pOISInputManager   ( NULL )
	{
	}
	
	/**
	 * @internal
	 * @brief Destructor. Class release.
	 */
	InputManagerOIS::~InputManagerOIS()
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
	bool InputManagerOIS::init()
	{
		// Check if the class is already initialized
		if ( isValid() )
			return true;
		
		// Variables to init OIS
		OIS::ParamList      paramList;
		size_t              windowHnd = 0;
		std::ostringstream  windowHndStr;
		
		// Fill params depending on the OS
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		{
			//paramList.insert( std::make_pair(std::string( "w32_mouse" ), std::string( "DISCL_BACKGROUND" ) ) );
			//paramList.insert( std::make_pair(std::string( "w32_mouse" ), std::string( "DISCL_NONEXCLUSIVE" ) ) );
			
			paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
			paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
			
			
			// TODO: try -> Uncomment these two lines to allow users to switch keyboards via the language bar
			paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND") ));
			paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE") ));
		}
#else
		{	
			paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
			paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
			paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
			paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
		}
#endif
		
		// Get window handle
		windowHnd = GraphicsManager::getSingleton().getMainWindow().getWindowHandle();
		
		// Insert window handle in parameters
		windowHndStr << (unsigned int) windowHnd;
		//paramList.insert( std::make_pair( std::string( "WINDOW" ), windowHndStr.str() ) );   
		paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(windowHnd)));

		
		// Create OIS input manager
		m_pOISInputManager = OIS::InputManager::createInputSystem( paramList );
		
		// Init mouse and keyboard
        m_mouse     = new MouseOIS();
        m_keyboard  = new KeyboardOIS();
        m_mouseOIS  = static_cast<MouseOIS*>(m_mouse);
        m_keyboardOIS = static_cast<KeyboardOIS*>(m_keyboard);
		m_mouseOIS->init( m_pOISInputManager );
		m_keyboardOIS->init( m_pOISInputManager );
		
		// The class is now initialized
		m_isValid = true;
		
		return true;
	}
	
	/**
	 * @internal
	 * @brief Releases the class resources. 
	 * After this method is called the class is not valid anymore.
	 */
	void InputManagerOIS::end()
	{
		// Check if the class is already released
		if ( !isValid() )
			return;
		
		// Release keyboad and mouse
		if ( m_mouse )
        {
            delete m_mouse;
            m_mouse = NULL;
        }
        
        if ( m_keyboard )
        {
            delete m_keyboard;
            m_keyboard = NULL;
		}
		// Release OIS input manager
		m_pOISInputManager->destroyInputSystem( m_pOISInputManager );
		m_pOISInputManager = NULL;
		
		
		// The class is not valid anymore
		m_isValid = false;
	}
	
	/**
	 * @internal
	 * @brief Updates the class state
	 */
	void InputManagerOIS::update()
	{
		// Update input devices
		m_mouseOIS->update();
		m_keyboardOIS->update();
	}
} // namespace Cing