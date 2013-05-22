/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

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

#include "Keyboard.h"

// Framework
#include "framework/Application.h"
#include "framework/UserAppGlobals.h"

// Graphics
#include "graphics/GraphicsManager.h"

// OIS
#include "OISInputManager.h"

namespace Cing
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Keyboard::Keyboard():
  BaseInputDevice< OIS::KeyListener >(),
  m_pOISKeyboard( NULL ),
  m_bIsValid    ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Keyboard::~Keyboard()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @param[in] pOISInputManager OIS Input manager. Allows to create and destroy the OIS Keyboard
 * @return true if the initialization was ok | false otherwise
 */
bool Keyboard::init( OIS::InputManager* pOISInputManager )
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

	// Init base input device
	BaseInputDevice< OIS::KeyListener >::init();

  // If possible create a buffered keyboard
	if ( pOISInputManager && ( pOISInputManager->getNumberOfDevices(OIS::OISKeyboard ) > 0 ) )
	{
	 m_pOISKeyboard = static_cast<OIS::Keyboard*>( pOISInputManager->createInputObject( OIS::OISKeyboard, true ) );
	 m_pOISKeyboard->setEventCallback( this );
	}

  // To be able to ask for ascii codes
  m_pOISKeyboard->setTextTranslation( OIS::Keyboard::Ascii );

  // Store OIS input manager
  m_pOISInputManager = pOISInputManager;


	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources.
 * After this method is called the class is not valid anymore.
 */
void Keyboard::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  // Release keyboard
  m_pOISInputManager->destroyInputObject( m_pOISKeyboard );
  m_pOISKeyboard         = NULL;
  m_pOISInputManager  = NULL;

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Updates the class state
 */
void Keyboard::update()
{
	if ( isValid() )
		m_pOISKeyboard->capture();
}


/**
 * @internal
 * @brief Returns true if the received key is down
 *
 * @param key key to check
 * @return true if the received key is down
 */
bool Keyboard::isKeyDown( OIS::KeyCode key ) const
{
	if ( isValid() )
		return m_pOISKeyboard->isKeyDown( key );

	return false;
}

/**
 * @internal
 * @brief Returns true if the received key is down
 *
 * @param key key to check
 * @return true if the received key is down
 */
bool	Keyboard::isModifierDown( OIS::Keyboard::Modifier modifier ) const
{
	if ( isValid() )
		return m_pOISKeyboard->isModifierDown( modifier );

	return false;
}

/**
 * @internal
 * @brief Receives an event of key pressed and informs all registered listeners
 *
 * @param[in] event Received event. Contains the information about the key state
 */
bool Keyboard::keyPressed( const OIS::KeyEvent &event )
{
	// TODO: install boost, tr1 c++ or create binders that support rederence parameters!
	//std::for_each( m_listeners.begin(), m_listeners.end(), std::bind2nd( std::mem_fun( &OIS::KeyListener::keyPressed ), event ) );

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
bool Keyboard::keyReleased( const OIS::KeyEvent &event )
{
	// Tell registered listeners
	for ( ListenersIt it = m_listeners.begin(); it != m_listeners.end(); ++it )
		it->second->keyReleased( event );

	return true;
}


} // namespace Cing
