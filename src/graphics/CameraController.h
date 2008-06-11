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

#ifndef _CameraController_h_
#define _CameraController_h_

#include "GraphicsPrereqs.h"
#include "common/CommonTypes.h"

namespace Graphics
{

/**
 * @internal
 * @brief Controls the camera movement in a firs-person like way
 */
class CameraController
{
public:

	// Constructor / Destructor
	CameraController();
	virtual ~CameraController();

	// Init / Release / Update
	void	init 	( Camera3D& cameraToControl );
	void	end		();	
	void	update();	

	// Query  Methods
	bool	isValid	() { return m_bIsValid; }

private:

	// Input control methods
	void	mouseControl		();
	void	keyboardControl	();

	// Attributes
	Camera3D*					m_camera;					///< Pointer to the controlled camera
	Ogre::SceneNode*	m_cameraNode;
	Ogre::SceneNode*	m_cameraYawNode;
	Ogre::SceneNode*	m_cameraPitchNode;
	Ogre::SceneNode*	m_cameraRollNode;	
	Vector						m_translateVector;
	float							m_moveScale;
	Ogre::Degree			m_rotateScale;
	bool							m_bIsValid;				///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Graphics

#endif // _CameraController_h_