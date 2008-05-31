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

private:

  // Private types

  // Listeners container
  typedef typename  std::map< Listener*, Listener* >                  Listeners;
  typedef typename  std::map< Listener*, Listener* >::iterator        ListenersIt;
  typedef typename  std::map< Listener*, Listener* >::const_iterator  ListenersConstIt;

	// Attributes
  Listeners     m_listeners;  ///< Listeners registered to receive events from this input device
	bool          m_bIsValid;	  ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Input


// Include template method definitions
#include "BaseInputDevice.inl"


#endif // _BaseInputDevice_H_
