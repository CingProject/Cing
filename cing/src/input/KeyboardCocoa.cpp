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

#include "KeyboardCocoa.h"
#include "InputUtils.h"

// Framework
#include "framework/Application.h"
#include "framework/UserAppGlobals.h"

// Graphics
#include "graphics/GraphicsManager.h"

namespace Cing
{
    
    /**
     * @internal
     * @brief Constructor. Initializes class attributes.
     */
    KeyboardCocoa::KeyboardCocoa():
    m_bIsValid    ( false )
    {
    }
    
    /**
     * @internal
     * @brief Destructor. Class release.
     */
    KeyboardCocoa::~KeyboardCocoa()
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
    bool KeyboardCocoa::init()
    {
        // Check if the class is already initialized
        if ( isValid() )
            return true;
        
        // Init base input device
        KeyboardBase::init();
        
        // TODO???
        
        // The class is now initialized
        m_bIsValid = true;
        
        return true;
    }
    
    /**
     * @internal
     * @brief Releases the class resources.
     * After this method is called the class is not valid anymore.
     */
    void KeyboardCocoa::end()
    {
        // Check if the class is already released
        if ( !isValid() )
            return;
        
        // The class is not valid anymore
        m_bIsValid = false;
    }
    
    /**
     * @internal
     * @brief Updates the class state
     */
    void KeyboardCocoa::update()
    {
        // nothing
    }
    
    
    /**
     * @internal
     * @brief Returns true if the received key is down
     *
     * @param key key to check
     * @return true if the received key is down
     */
    bool KeyboardCocoa::isKeyDown( KeyCode key ) const
    {
        LOG("KeyboardCocoa::isKeyDown - NOT YET IMPLEMENTED!");
        return false;
    }
    
    /**
     * @internal
     * @brief Returns true if the received key is down
     *
     * @param key key to check
     * @return true if the received key is down
     */
//    bool KeyboardOIS::isModifierDown( OIS::Keyboard::Modifier modifier ) const
//    {
//        return false;
//    }
    
    /**
     * @internal
     * @brief Receives an event of key pressed and informs all registered listeners
     *
     * @param[in] event Received event. Contains the information about the key state
     */
    bool KeyboardCocoa::keyPressed( const KeyEvent &event )
    {
        // Tell registered listeners
        for ( ListenersIt it = m_listeners.begin(); it != m_listeners.end(); ++it )
            it->second->keyPressed( event );
        
        return true;
    }
    
    /**
     * @internal
     * @brief Receives an event of key released and informs all registered listeners
     *
     * @param[in] event Received event. Contains the information about the key state
     */
    bool KeyboardCocoa::keyReleased( const KeyEvent &event )
    {
        // Tell registered listeners
        for ( ListenersIt it = m_listeners.begin(); it != m_listeners.end(); ++it )
            it->second->keyReleased( event );
        
        return true;
    }
    
    
} // namespace Cing
