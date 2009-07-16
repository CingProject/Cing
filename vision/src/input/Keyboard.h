/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

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

#ifndef _Keyboard_H_
#define _Keyboard_H_

#include "InputPrereqs.h"
#include "BaseInputDevice.h"

// OIS
#include "Ogre3d/include/OIS/OISKeyboard.h"

namespace Cing
{

/**
 * @internal
 * Manages keyboard input
 */
class Keyboard: public BaseInputDevice< OIS::KeyListener >, public OIS::KeyListener
{
public:

	// Constructor / Destructor
	 Keyboard();
	virtual ~Keyboard();

	// Init / Release / Update
	bool  init   ( OIS::InputManager* pOISInputManager );
	void  end    ();
	void  update ();

	// Query methods
	bool  isValid       ()  const { return m_bIsValid; }
	bool	isKeyDown			( OIS::KeyCode key ) const;
	bool	isModifierDown( OIS::Keyboard::Modifier modifier ) const;

  //bool  isKeyPressed  ( char c )  const;

private:


  // OIS KeyListener interface
  virtual bool keyPressed( const OIS::KeyEvent &arg );
  virtual bool keyReleased( const OIS::KeyEvent &arg );		

	// Attributes
  OIS::InputManager*  m_pOISInputManager; ///< Pointer to the OIS input manager
  OIS::Keyboard*      m_pOISKeyboard;     ///< OIS Keyboard
	bool                m_bIsValid;	        ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _Keyboard_H_
