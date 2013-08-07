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

#include "InputManagerCocoa.h"
#include "MouseCocoa.h"
#include "KeyboardCocoa.h"

// Graphics
#include "graphics/GraphicsManager.h"

// Ogre
#include "OgreStringConverter.h"

// STL
#include <sstream>

#import "CingCocoaInputView.h"

namespace Cing
{
	/**
	 * @internal
	 * @brief Constructor. Initializes class attributes.
	 */
	InputManagerCocoa::InputManagerCocoa():
    m_parentView(NULL)
	{
	}
	
	/**
	 * @internal
	 * @brief Destructor. Class release.
	 */
	InputManagerCocoa::~InputManagerCocoa()
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
	bool InputManagerCocoa::init()
	{
		// Check if the class is already initialized
		if ( isValid() )
			return true;
        
        m_mouseCocoa = new MouseCocoa();
        m_mouseCocoa->init();
        m_mouse = m_mouseCocoa;
        
        m_keyboardCocoa = new KeyboardCocoa();
        m_keyboardCocoa->init();
        m_keyboard = m_keyboardCocoa;
        
        
        NSWindow *window = (NSWindow *)GraphicsManager::getSingleton().getMainWindow().getWindowHandle();
        [window setAcceptsMouseMovedEvents:YES];
        
        // if the user passed in a parent OgreView, use that, otherwise just use the main content view
        NSView *v = nil;
        if ( m_parentView != NULL )
            v = m_parentView;
        else
            v = [window contentView];
        if ( v != nil )
        {
            // create and add CingCocoaInputView to the hierarchy
            CingCocoaInputView *iv = [[CingCocoaInputView alloc] initWithFrame:[v frame]];
            [iv setMouse:m_mouseCocoa];
            [iv setKeyboard:m_keyboardCocoa];
            [v addSubview:iv];
            [iv release];
        }
		
		// The class is now initialized
		m_isValid = true;
		
		return true;
	}
	
	/**
	 * @internal
	 * @brief Releases the class resources.
	 * After this method is called the class is not valid anymore.
	 */
	void InputManagerCocoa::end()
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
		
		
		// The class is not valid anymore
		m_isValid = false;
	}
	
	/**
	 * @internal
	 * @brief Updates the class state
	 */
	void InputManagerCocoa::update()
	{
        // nothing
	}
    
} // namespace Cing
