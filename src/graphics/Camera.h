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

#ifndef _Camera_H_
#define _Camera_H_

#include "GraphicsPrereqs.h"

// Stl
#include <string>

namespace Graphics
{

/**
 * @internal
 * 
 */
class Camera
{
public:

	// Constructor / Destructor
	 Camera();
	~Camera();

	// Init / Release / Update
  bool                init            ( Ogre::SceneManager* pOgreSceneManager, const std::string& cameraName = DEFAULT_NAME );
	void                end             ();
	void                update          ();

	// Query methods
	bool                isValid         () const  { return m_bIsValid;     } 
  const Ogre::Camera* getOgreCamera   () const  { return m_pOgreCamera;  }
  Ogre::Camera*       getOgreCamera   ()        { return m_pOgreCamera;  }
	
private:

  // Constant attributes
  static const std::string  DEFAULT_NAME;   ///< Default camera name when created (unless other specified)
  
	// Attributes
  Ogre::Camera*             m_pOgreCamera;  ///< Ogre camera
	bool                      m_bIsValid;	    ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Graphics

#endif // _Camera_H_
