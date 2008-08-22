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

#include "Keyboard.h"

// Framework
#include "framework/Application.h"
#include "framework/UserAppGlobals.h"

// Graphics
#include "graphics/GraphicsManager.h"

// OIS
#include "OISInputManager.h"

namespace Input
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
  if ( pOISInputManager && ( pOISInputManager->numKeyboards() > 0 ) )
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


} // namespace Input
