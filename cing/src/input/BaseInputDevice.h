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

#ifndef _BaseInputDevice_H_
#define _BaseInputDevice_H_

// Precompiled headers
#include "Cing-Precompiled.h"

// Stl includes
#include <map>


namespace Cing
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

} // namespace Cing


// Include template method definitions
#include "BaseInputDevice.inl"


#endif // _BaseInputDevice_H_
