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

#ifndef _InputManager_H_
#define _InputManager_H_

#include "InputPrereqs.h"
#include "Mouse.h"
#include "Keyboard.h"

// Common
#include "common/Singleton.h"

namespace Input
{

/**
 * @internal
 * 
 */
class InputManager: public Common::SingletonStatic< InputManager >
{
public:

	// Required for singleton
	friend class Common::SingletonStatic< InputManager >;

	// Destructor
	virtual ~InputManager         ();

	// Init / Release / Update
	bool              init        ();
	void              end         ();
	void              update      ();

	// Query methods
	bool        isValid     () const	{ return m_bIsValid; }
  Mouse&      getMouse    ()				{ return m_mouse;    } 
  Keyboard&   getKeyboard ()				{ return m_keyboard; } 
	

	// Public input devices
	static Mouse        m_mouse;            ///< System's mouse
	static Keyboard     m_keyboard;         ///< System's keyboard


private:

	// private constructor to ensure singleton
	InputManager                 ();

	// Attributes
  OIS::InputManager*  m_pOISInputManager; ///< Pointer to the OIS input manager
	bool                m_bIsValid;	        ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.


};


} // namespace Input

#endif // _InputManager_H_