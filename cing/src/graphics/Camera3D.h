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

#ifndef _Camera_H_
#define _Camera_H_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "GraphicsPrereqs.h"

// Common
#include "common/CommonTypes.h"

// Stl
#include <string>

namespace Cing
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
	bool                isValid         				() const	{ return m_bIsValid;     } 
	Ogre::SceneNode*	getSceneNode					()			{ return m_cameraSceneNode; }
	const Ogre::Camera* getOgreCamera   				() const	{ return m_pOgreCamera;  }
	Ogre::Camera*       getOgreCamera   				()			{ return m_pOgreCamera;  }
	
	// set methods
	void                moveRelative    				( const Vector& move );
	void				set3DCameraProperties			();

	static const float        V_FOV_DEG;      ///< Camera's vertical FOV in degrees

private:

	// Constant attributes
	static const std::string  DEFAULT_NAME;   ///< Default camera name when created (unless other specified)

	// Attributes
	Ogre::Camera*				m_pOgreCamera;				///< Ogre camera
	Ogre::SceneManager*			m_pOgreSceneManager;	///< Pointer to the scene manager where the camera will be created
	Ogre::SceneNode*			m_cameraSceneNode;		///< Scene node where the camera is attached to
	Vector						m_cameraPos;      		///< Camera's position
	Vector						m_cameraLookAt;   		///< Camera's look at position
	Vector						m_cameraUpVector; 		///< Camera's look up vector
	float                     	m_vFovRad;        		///< Camera's  vertical fov in radians
	float                     	m_cameraDistance; 		///< Camera's distance to the look at position
	float                     	m_aspectRatio;    		///< Camera's  aspect ratio
	bool                      	m_bIsValid;	      		///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _Camera_H_
