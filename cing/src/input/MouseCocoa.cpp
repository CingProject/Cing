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

#include "MouseCocoa.h"

// Framework
#include "framework/Application.h"
#include "framework/UserAppGlobals.h"

// Graphics
#include "graphics/GraphicsManager.h"

// Input
#include "input/InputTypes.h"
#include "input/InputUtils.h"

namespace Cing
{
    
    /**
     * @internal
     * @brief Constructor. Initializes class attributes.
     */
    MouseCocoa::MouseCocoa():
    m_bIsValid( false )
    {
    }
    
    /**
     * @internal
     * @brief Destructor. Class release.
     */
    MouseCocoa::~MouseCocoa()
    {
        // Release resources
        end();
    }
    
    /**
     * @internal
     * @brief Initializes the class so it becomes valid.
     * @return true if the initialization was ok | false otherwise
     */
    bool MouseCocoa::init()
    {
        // Check if the class is already initialized
        if ( isValid() )
            return true;
        
        // Init base input device
        MouseBase::init();
        
        // TODO, setup delegate/handlers?
        
        // The class is now initialized
        m_bIsValid = true;
        
        return true;
    }
    
    /**
     * @internal
     * @brief Releases the class resources.
     * After this method is called the class is not valid anymore.
     */
    void MouseCocoa::end()
    {
        // Check if the class is already released
        if ( !isValid() )
            return;
        
        // The class is not valid anymore
        m_bIsValid = false;
    }
    
    /**
     * @internal
     * @brief Captures the mouse events occurred in the frame
     */
    void MouseCocoa::update()
    {
        // nothing
    }
    
    // TEMP
    bool MouseCocoa::isButtonPressed( int n ) const
    {
        LOG("MouseCocoa::isButtonPressed - NOT YET IMPLEMENTED!");
        return false;
    }
    
    int MouseCocoa::getXAxisRelative() const
    {
        LOG("MouseCocoa::getXAxisRelative - NOT YET IMPLEMENTED!");
        return 0;
    }
    int MouseCocoa::getYAxisRelative() const
    {
        LOG("MouseCocoa::getYAxisRelative - NOT YET IMPLEMENTED!");
        return 0;
    }
    int MouseCocoa::getZAxisRelative() const
    {
        LOG("MouseCocoa::getZAxisRelative - NOT YET IMPLEMENTED!");
        return 0;
    }
    
    
    //// OIS Mouse interface
    
    /**
     * @internal
     * @brief Receives an event of mouse moved and informs all registered listeners
     *
     * @param[in] event Received event. Contains the information about the mouse state
     */
    bool MouseCocoa::mouseMoved( const MouseEvent &event )
    {
        // Tell registered listeners
        for ( ListenersIt it = m_listeners.begin(); it != m_listeners.end(); ++it )
            it->second->mouseMoved( event );
        
        return true;
    }
    
    
    
    /**
     * @internal
     * @brief Receives an event of mouse pressed and informs all registered listeners
     *
     * @param[in] event Received event. Contains the information about the mouse state
     * @param[in] id    Id of the mouse that has been pressed
     */
    bool MouseCocoa::mousePressed ( const MouseEvent &event, MouseButtonID id )
    {
        // Tell registered listeners
        for ( ListenersIt it = m_listeners.begin(); it != m_listeners.end(); ++it )
            it->second->mousePressed( event, id );
        
        return true;
    }
    
    /**
     * @internal
     * @brief Receives an event of mouse released and informs all registered listeners
     *
     * @param[in] event Received event. Contains the information about the mouse state
     * @param[in] id    Id of the mouse that has been released
     */
    bool MouseCocoa::mouseReleased( const MouseEvent &event, MouseButtonID id )
    {
        // Tell registered listeners
        for ( ListenersIt it = m_listeners.begin(); it != m_listeners.end(); ++it )
            it->second->mouseReleased( event, id );
        
        return true;
    }
    
    
} // namespace Cing
