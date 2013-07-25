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
#ifndef _BaseInputDeviceInl_H_
#define _BaseInputDeviceInl_H_

// Precompiled headers
#include "Cing-Precompiled.h"

/**
 * @internal
 * @file: BaseInputDevice template method definitions
 */

namespace Cing
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

} // namespace Cing

#endif // _BaseInputDeviceInl_H_
