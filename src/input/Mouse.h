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

#ifndef _Mouse_H_
#define _Mouse_H_

#include "BaseInputDevice.h"
#include "InputPrereqs.h"

// OIS
#include "OIS/OISMouse.h"

namespace Input
{

/**
 * @internal
 * Manages mouse input
 */
  class Mouse: public BaseInputDevice< OIS::MouseListener > , public OIS::MouseListener
{
public:

	// Constructor / Destructor
	        Mouse();
	virtual ~Mouse();

	// Init / Release / Update
	bool  init   ( OIS::InputManager *pOISInputManager );
	void  end    ();
	void  update ();

	// Query methods
	bool  isValid() const { return m_bIsValid; }

  // TEMP
  bool  isButtonPressed ( int n ) const;
  int   getXAxisRelative() const;
  int   getYAxisRelative() const;
  int   getZAxisRelative() const;
	
private:

  // Private Methods

  // OIS Mouse interface
  virtual bool mouseMoved   ( const OIS::MouseEvent &arg );
  virtual bool mousePressed ( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
  virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );


	// Attributes
  OIS::InputManager*  m_pOISInputManager; ///< Pointer to the OIS input manager
  OIS::Mouse*         m_pOISMouse;        ///< OIS Mouse
	bool                m_bIsValid;	        ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Input

#endif // _Mouse_H_
