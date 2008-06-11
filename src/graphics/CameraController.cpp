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

#include "CameraController.h"
#include "Camera3D.h"

// Common
#include "common/LogManager.h"

// Ogre
#include "externLibs/Ogre3d/include/OgreSceneNode.h"
#include "externLibs/Ogre3d/include/OgreCamera.h"
#include "externLibs/Ogre3d/include/ois/OISKeyboard.h"

// Input
#include "input/Mouse.h"
#include "input/Keyboard.h"
#include "input/InputManager.h"

namespace Graphics
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
CameraController::CameraController():
	m_camera		( NULL ),
	m_cameraNode( NULL ),
	m_cameraYawNode( NULL ),
	m_cameraPitchNode( NULL ),
	m_cameraRollNode( NULL ),
	m_moveScale			( 1.5f ),
	m_rotateScale		( 1.0f ),
	m_bIsValid  ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
CameraController::~CameraController()
{
	// Release resources
	end();
}

/**
 * @internal
 * @brief Initializes the scene node structure necessary to control the camera
 */
void CameraController::init( Camera3D& cameraToControl )
{
	// If is already initialized, release it first
	if ( isValid() )
		end();

	// Store the camera pointer
	m_camera = &cameraToControl;

	// Crete the scene nodes to control the camera

	// Get camera's node
	m_cameraNode = m_camera->getSceneNode();
	m_cameraNode->detachObject( m_camera->getOgreCamera()->getName() );

	// Create the camera's yaw node as a child of camera's top node.
	m_cameraYawNode = m_cameraNode->createChildSceneNode();

	// Create the camera's pitch node as a child of camera's yaw node.
	m_cameraPitchNode = m_cameraYawNode->createChildSceneNode();

	// Create the camera's roll node as a child of camera's pitch node and attach the camera to it.
	m_cameraRollNode = m_cameraPitchNode->createChildSceneNode();
	m_cameraRollNode->attachObject( m_camera->getOgreCamera() );
	
	m_bIsValid = true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void CameraController::end()
{
	// Do nothing if end() was called previously
	if ( !isValid() )
		return;

	// Delete yaw node (which is the parent) and re-attach the camera to its original node
	m_cameraNode->removeAndDestroyChild( m_cameraYawNode->getName() );
	m_cameraNode->attachObject( m_camera->getOgreCamera() );

	// TODO is it necessary to delete child scene nodes? think not

	m_bIsValid = false;
}

/**
 * @internal 
 * @brief Updates the movement of the camera
 */
void CameraController::update()
{
	if ( !isValid() )
		return;

	// Check keyboard's state
	keyboardControl();

	// Check mouse's state
	mouseControl();

	// Reset translation vector
	m_translateVector = Vector::ZERO;
}

/**
 * @internal 
 * @brief Checks the mouse state to modify the camera based on it
 */
void CameraController::mouseControl()
{
	Ogre::Real pitchAngle;
	Ogre::Real pitchAngleSign;

	// Get the mouse reference
	const Input::Mouse& mouse = Input::InputManager::getSingleton().getMouse();

	// Yaws the camera according to the mouse relative movement.
	Ogre::Radian rotX = Ogre::Degree( -mouse.getXAxisRelative() * 0.13 );
	m_cameraYawNode->yaw(rotX);

	// Pitches the camera according to the mouse relative movement.
	Ogre::Radian rotY = Ogre::Degree( -mouse.getYAxisRelative() * 0.13 );
	m_cameraPitchNode->pitch(rotY);

	// Translates the camera according to the translate vector which is
	// controlled by the keyboard arrows.
	//
	// NOTE: We multiply the mTranslateVector by the cameraPitchNode's
	// orientation quaternion and the cameraYawNode's orientation
	// quaternion to translate the camera according to the camera's
	// orientation around the Y-axis and the X-axis.
	m_cameraNode->translate(m_cameraYawNode->getOrientation() * m_cameraPitchNode->getOrientation() * m_translateVector, Ogre::SceneNode::TS_LOCAL);

	// Angle of rotation around the X-axis.
	pitchAngle = (2 * Ogre::Degree(Ogre::Math::ACos(m_cameraPitchNode->getOrientation().w)).valueDegrees());

	// Just to determine the sign of the angle we pick up above, the
	// value itself does not interest us.
	pitchAngleSign = m_cameraPitchNode->getOrientation().x;

	// Limit the pitch between -90 degress and +90 degrees, Quake3-style.
	if (pitchAngle > 90.0f)
	{
		if (pitchAngleSign > 0)
			// Set orientation to 90 degrees on X-axis.
			m_cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),
			Ogre::Math::Sqrt(0.5f), 0, 0));
		else if (pitchAngleSign < 0)
			// Sets orientation to -90 degrees on X-axis.
			m_cameraPitchNode->setOrientation(Ogre::Quaternion(Ogre::Math::Sqrt(0.5f),
			-Ogre::Math::Sqrt(0.5f), 0, 0));
	}
}

/**
 * @internal 
 * @brief Checks the keyboard state to modify the camera based on it
 */
void CameraController::keyboardControl()
{
	// Get the keyboard reference
	const Input::Keyboard& keyboard = Input::InputManager::getSingleton().getKeyboard();

	if ( keyboard.isKeyDown(OIS::KC_UP) )
		LOG_CRITICAL( "Up Key pressed" );
	else 
		LOG_CRITICAL( "Up Key NOT pressed" );


	// Move camera upwards along to world's Y-axis.
	if(keyboard.isKeyDown(OIS::KC_PGUP))
		m_cameraNode->setPosition(m_cameraNode->getPosition() + Ogre::Vector3(0, 5, 0));

	// Move camera downwards along to world's Y-axis.
	if(keyboard.isKeyDown(OIS::KC_PGDOWN))
		m_cameraNode->setPosition(m_cameraNode->getPosition() - Ogre::Vector3(0, 5, 0));

	// Move camera forward.
	if(keyboard.isKeyDown(OIS::KC_UP))
		m_translateVector.z = -(m_moveScale);

	// Move camera backward.
	if(keyboard.isKeyDown(OIS::KC_DOWN))
		m_translateVector.z = m_moveScale;

	// Move camera left.
	if(keyboard.isKeyDown(OIS::KC_LEFT))
		m_translateVector.x = -(m_moveScale);

	// Move camera right.
	if(keyboard.isKeyDown(OIS::KC_RIGHT))
		m_translateVector.x = m_moveScale;

	// Rotate camera left.
	if(keyboard.isKeyDown(OIS::KC_A))
		m_cameraYawNode->yaw(m_rotateScale);

	// Rotate camera right.
	if(keyboard.isKeyDown(OIS::KC_D))
		m_cameraYawNode->yaw(-(m_rotateScale));

	// Rotate camera upwards.
	if(keyboard.isKeyDown(OIS::KC_W))
		m_cameraPitchNode->pitch(m_rotateScale);

	// Rotate camera downwards.
	if(keyboard.isKeyDown(OIS::KC_S))
		m_cameraPitchNode->pitch(-(m_rotateScale));

	// Strip all rotation to the camera.
	if(keyboard.isKeyDown(OIS::KC_R))
	{
		m_cameraYawNode->setOrientation(Ogre::Quaternion::IDENTITY);
		m_cameraPitchNode->setOrientation(Ogre::Quaternion::IDENTITY);
		m_cameraRollNode->setOrientation(Ogre::Quaternion::IDENTITY);
	}
}

} // namespace Graphics