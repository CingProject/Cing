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

#ifndef _Application_H_
#define _Application_H_

// Common
#include "common/Singleton.h"

// OIS
#include "externLibs/Ogre3d/include/ois/OISMouse.h"
#include "externLibs/Ogre3d/include/ois/OISKeyboard.h"

// Ogre
#include "externLibs/Ogre3d/include/OgreTimer.h"

namespace Framework
{

/**
 * @internal
 * Base class to create an application. Manages the render loop and the initialization and 
 * release of the main application systems
 */
class Application: public Common::SingletonStatic< Application >, public OIS::KeyListener, public OIS::MouseListener
{
public:

	// Required for singleton
	friend class Common::SingletonStatic< Application >;

	// Constructor / Destructor
	virtual ~Application();

	// Init / Release / Loop
	bool  initApp   ();
	void  endApp    ();
	void  drawApp   ();

	// Query methods
	bool  isValid() const { return m_bIsValid; }
	
  // Event handlers
	bool mouseMoved			( const OIS::MouseEvent& event );
	bool mousePressed		( const OIS::MouseEvent& event, OIS::MouseButtonID id  );
	bool mouseReleased	( const OIS::MouseEvent& event, OIS::MouseButtonID id  );
	bool keyPressed			( const OIS::KeyEvent& event );
	bool keyReleased		( const OIS::KeyEvent& event);

private:
	// private constructor to ensure singleton
	Application();

	// Attributes
	Ogre::Timer		m_timer;				///< Application timer. Userd to messure time between frames
	bool					m_bIsValid;			///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Framework

#endif // _Application_H_
