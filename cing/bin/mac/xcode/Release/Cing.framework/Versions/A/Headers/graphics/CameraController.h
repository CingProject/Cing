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

#ifndef _CameraController_h_
#define _CameraController_h_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "GraphicsPrereqs.h"
#include "common/CommonTypes.h"

// Forward declarations
namespace CCS
{
	class CameraControlSystem;
	class FreeCameraMode;
	class OrbitalCameraMode;
}


namespace Cing
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

	// control methods
	void	useKeyboard( bool value );
	void	useMouse		( bool value );

	// Query  Methods
	bool	isValid	() { return m_bIsValid; }

private:

	// Input control methods
	void	mouseControl		();
	void	keyboardControl	();

	// Attributes
	CCS::CameraControlSystem*	m_cameraCS;			///< Camera control system: manages different camera controllers
	CCS::FreeCameraMode*		m_freeCam;			///< Free camera mode (default)
	CCS::OrbitalCameraMode*		m_orbitalCam;		///< Orbital camera mode
	Camera3D*					m_camera;			///< Pointer to the controlled camera
	Ogre::SceneNode*			m_cameraNode;
	Ogre::SceneNode*			m_cameraYawNode;
	Ogre::SceneNode*			m_cameraPitchNode;
	Ogre::SceneNode*			m_cameraRollNode;	
	Vector						m_translateVector;
	float						m_moveScale;
	Ogre::Degree				m_rotateScale;

	bool						m_useKeyboard;
	bool						m_useMouse;
	bool						m_bIsValid;			///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _CameraController_h_