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

// Common
#include "common/CommonTypes.h"

// Stl
#include <string>

namespace Graphics
{

/**
 * @internal
 * @brief Class to control a camera in the 3d scene. Used to render the scene.
 */
class Camera3D
{
public:

	// Constructor / Destructor
	 Camera3D();
	~Camera3D();

	// Init / Release / Update
	bool                init            				( Ogre::SceneManager* pOgreSceneManager, const std::string& cameraName = DEFAULT_NAME );
	void                end             				();
	void                update          				();

	// Query methods
	bool                isValid         				() const  { return m_bIsValid;     } 
	Ogre::SceneNode*		getSceneNode						()				{ return m_cameraSceneNode; }
	const Ogre::Camera* getOgreCamera   				() const  { return m_pOgreCamera;  }
	Ogre::Camera*       getOgreCamera   				()        { return m_pOgreCamera;  }
	
	// set methods
	void                moveRelative    				( const Vector& move );
	void								setProcessingLikeCamera	();

private:

	// Constant attributes
	static const std::string  DEFAULT_NAME;   ///< Default camera name when created (unless other specified)
	static const float        V_FOV_DEG;      ///< Camera's vertical FOV in degrees

	// Attributes
	Ogre::Camera*             m_pOgreCamera;				///< Ogre camera
	Ogre::SceneManager*				m_pOgreSceneManager;	///< Pointer to the scene manager where the camera will be created
	Ogre::SceneNode*          m_cameraSceneNode;		///< Scene node where the camera is attached to
	Vector										m_cameraPos;      		///< Camera's position
	Vector										m_cameraLookAt;   		///< Camera's look at position
	Vector										m_cameraUpVector; 		///< Camera's look up vector
	float                     m_vFovRad;        		///< Camera's  vertical fov in radians
	float                     m_cameraDistance; 		///< Camera's distance to the look at position
	float                     m_aspectRatio;    		///< Camera's  aspect ratio
	bool                      m_bIsValid;	      		///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Graphics

#endif // _Camera_H_
