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

#ifndef _BaseInputDevice_H_
#define _BaseInputDevice_H_

// Stl includes
#include <map>


namespace Input
{

/**
 * @internal
 * Abstract Base class to all input devices. Provides event listeners management.
 * Template parameters:
 *  - Listener: Listener class that this devices manages
 */
template< class Listener >
class BaseInputDevice
{
public:

	// Constructor / Destructor
	          BaseInputDevice     ();
	virtual   ~BaseInputDevice    ();

	// Init / Release / Update
	virtual bool      init        ();
	virtual void      end         ();
	virtual void      update      () = 0;

	// Query methods
	bool      isValid             () const { return m_bIsValid; }

  // Listeners
  void      addListener         ( Listener* pListener );
  void      removeListener      ( Listener* pListener );
  void      removeAllListeners  ();

protected:

	// Listeners container
	typedef typename  std::map< Listener*, Listener* >                  Listeners;
	typedef typename  std::map< Listener*, Listener* >::iterator        ListenersIt;
	typedef typename  std::map< Listener*, Listener* >::const_iterator  ListenersConstIt;

	// Protected attributes
	Listeners     m_listeners;  ///< Listeners registered to receive events from this input device

private:

	// Attributes
	bool          m_bIsValid;	  ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};


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


// Include template method definitions
//#include "BaseInputDevice.inl"


#endif // _BaseInputDevice_H_
