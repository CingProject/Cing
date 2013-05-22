/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

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

// Precompiled headers
#include "Cing-Precompiled.h"

#include "CameraController.h"
#include "Camera3D.h"

// Common
#include "common/CommonUtilsIncludes.h"

// Graphics
#include "graphics/GraphicsManager.h"

// Ogre
#include "OgreSceneNode.h"
#include "OgreCamera.h"
#include "OISKeyboard.h"

// Input
#include "input/Mouse.h"
#include "input/Keyboard.h"
#include "input/InputManager.h"

// Ogre Camera System
#include <CCSCameraControlSystem.h>
#include "CCSBasicCameraModes.h"
#include "CCSFreeCameraMode.h"
#include "CCSOrbitalCameraMode.h"

namespace Cing
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
CameraController::CameraController():
	m_cameraCS		( NULL ),
	m_freeCam		( NULL ),
	m_orbitalCam	( NULL ),
	m_camera		( NULL ),
	m_cameraNode( NULL ),
	m_cameraYawNode( NULL ),
	m_cameraPitchNode( NULL ),
	m_cameraRollNode( NULL ),
	m_moveScale			( 5.0f ),
	m_rotateScale		( 0.1f ),
	m_useMouse			( true	),
	m_useKeyboard		( true ),
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

	// First we have to detach the camera from the current scene node, because the CameraControlSystem will reatach it again
	m_camera->getSceneNode()->detachObject(m_camera->getOgreCamera());

	// Create the camera control system
	// TODO: error checking
	m_cameraCS = new CCS::CameraControlSystem(m_camera->getOgreCamera()->getSceneManager(), "CameraControlSystem", m_camera->getOgreCamera());

	// Set the camera to render the main viewport
	GraphicsManager::getSingleton().getMainWindow().attachCameraToWindow( m_cameraCS->getOgreCamera() );

	// Register available camera modes
	
	// Create an instance of the "orbital" camera mode
	m_orbitalCam = new CCS::OrbitalCameraMode(m_cameraCS, 200);
	m_orbitalCam->setZoomFactor(100);
	m_orbitalCam->setRotationFactor(5);
	m_cameraCS->registerCameraMode("Orbit",m_orbitalCam);

	// Create a free camera mode
	m_freeCam = new CCS::FreeCameraMode(m_cameraCS);
	m_freeCam->setMoveFactor( 30 );
	m_freeCam->setRotationFactor( 0.2f );
	m_cameraCS->registerCameraMode("Free",m_freeCam);

	// Set the initial camera mode (Free)
	m_cameraCS->setCurrentCameraMode(m_freeCam);
	m_cameraCS->setCameraTarget(GraphicsManager::getSingleton().getSceneManager().getRootSceneNode());

	// Set the camera position
	LOG( "Default Camera Controller forces OPENGL3D coordinate system" );
	GraphicsManager::getSingleton().applyCoordinateSystemTransform(OPENGL3D);
	m_cameraCS->getOgreCamera()->setPosition( Vector( 0, 0, 2000.0 ) );

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

	Release(m_orbitalCam);
	Release(m_freeCam);
	Release(m_cameraCS);

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
	
	// Update camera controller
	if ( m_cameraCS ) 
		m_cameraCS->update((float)elapsedSec);

	// Keyboard control
	if ( m_useKeyboard )
		keyboardControl();

	// Mouse Control
	if ( m_useMouse )
		mouseControl();
}

/**
 * @brief Allows camera control through the keyboard
 *
 * @note keys are: w/a/s/d - q/e - r - cursors
 *
 * @param value if true, keyboard control will be activated, if false it will be deactivated
 */
void CameraController::useKeyboard( bool value )
{
	m_useKeyboard = value;
}

/**
 * @brief Allows camera control through the mouse
 *
 * @param value if true, mouse control will be activated, if false it will be deactivated
 */
void CameraController::useMouse( bool value )
{
	m_useMouse = value;
}
/**
 * @internal 
 * @brief Checks the mouse state to modify the camera based on it
 */
void CameraController::mouseControl()
{
	if (m_cameraCS->getCameraModeName( m_cameraCS->getCurrentCameraMode() ) == "Free")
	{
		CCS::FreeCameraMode* freeCameraMode = (CCS::FreeCameraMode*)m_cameraCS->getCameraMode("Free");
		if ( freeCameraMode )
		{
			freeCameraMode->yaw( (float)mouse.getXAxisRelative() );
			freeCameraMode->pitch( (float)mouse.getYAxisRelative() );
		}
	}
}

/**
 * @internal 
 * @brief Checks the keyboard state to modify the camera based on it
 */
void CameraController::keyboardControl()
{
	// Free camera mode
	if(m_cameraCS->getCameraModeName(m_cameraCS->getCurrentCameraMode()) == "Free")
	{
		CCS::FreeCameraMode* freeCameraMode = (CCS::FreeCameraMode*)m_cameraCS->getCameraMode("Free");
		if ( !freeCameraMode )
			return;

		if(keyboard.isKeyDown(OIS::KC_A))
			freeCameraMode->goLeft();

		if(keyboard.isKeyDown(OIS::KC_D))
			freeCameraMode->goRight();

		if(keyboard.isKeyDown(OIS::KC_UP) || keyboard.isKeyDown(OIS::KC_W) )
			freeCameraMode->goForward();

		if(keyboard.isKeyDown(OIS::KC_DOWN) || keyboard.isKeyDown(OIS::KC_S) )
			freeCameraMode->goBackward();

		if(keyboard.isKeyDown(OIS::KC_PGUP))
			freeCameraMode->goUp();

		if(keyboard.isKeyDown(OIS::KC_PGDOWN))
			freeCameraMode->goDown();

		if(keyboard.isKeyDown(OIS::KC_RIGHT))
			freeCameraMode->yaw(-1);

		if(keyboard.isKeyDown(OIS::KC_LEFT))
			freeCameraMode->yaw(1);	
	}
	// Orbital camera mode
	else if(m_cameraCS->getCameraModeName(m_cameraCS->getCurrentCameraMode()) == "Orbital")
	{
		CCS::OrbitalCameraMode* orbitalCameraMode = (CCS::OrbitalCameraMode*)m_cameraCS->getCameraMode("Orbital");
		if ( !orbitalCameraMode )
			return;

		if(keyboard.isKeyDown(OIS::KC_A))
			orbitalCameraMode->yaw(1);

		if(keyboard.isKeyDown(OIS::KC_D))
			orbitalCameraMode->yaw(-1);

		if(keyboard.isKeyDown(OIS::KC_W))
			orbitalCameraMode->pitch(1);

		if(keyboard.isKeyDown(OIS::KC_S))
			orbitalCameraMode->pitch(-1);

		if(keyboard.isKeyDown(OIS::KC_PGUP))
			orbitalCameraMode->zoom(-1);

		if(keyboard.isKeyDown(OIS::KC_PGDOWN))
			orbitalCameraMode->zoom(1);	
	}
		
}

} // namespace Cing