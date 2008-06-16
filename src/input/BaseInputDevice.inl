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
#ifndef _BaseInputDeviceInl_H_
#define _BaseInputDeviceInl_H_

/**
 * @internal
 * @file: BaseInputDevice template method definitions
 */

namespace Input
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
template< class Listener >
BaseInputDevice< Listener >::BaseInputDevice():
  m_bIsValid( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
template< class Listener >
BaseInputDevice< Listener >::~BaseInputDevice()
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
template< class Listener >
bool BaseInputDevice< Listener >::init()
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  removeAllListeners();

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
template< class Listener >
void BaseInputDevice< Listener >::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  removeAllListeners();

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Adds a listener for the events received in this input device
 * 
 * @param[in] pListener Listener to register. It will receive the input events of this device.
 *                      It must implement the Listener interface
 */
template< class Listener >
void BaseInputDevice< Listener >::addListener( Listener* pListener )
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  // Store the listener
  m_listeners[ pListener ] = pListener;
}


/**
 * @internal
 * @brief Removes a listener previously registered to receive the input events of this device
 * 
 * @param[in] pListener pointer of the listener to remove
 */
template< class Listener >
void BaseInputDevice< Listener >::removeListener( Listener* pListener )
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  // Search listener 
  ListenersIt it = m_listeners.find( pListener );

  // Delete it if found
  if( it != m_listeners.end() ) 
    m_listeners.erase( it );
}

/**
 * @internal
 * @brief Removes all registered listeners
 */
template< class Listener >
void BaseInputDevice< Listener >::removeAllListeners()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  m_listeners.clear();
}

} // namespace Input

#endif // _BaseInputDeviceInl_H_
