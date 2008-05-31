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

#include "Camera.h"

// Ogre includes
#include "OgreCamera.h"
#include "OgreSceneManager.h"

// System includes
#include "Exception.h"
#include "Release.h"

namespace Graphics
{

// Static member inicialization
const std::string  Camera::DEFAULT_NAME = "DefaultCam";


/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Camera::Camera():
  m_bIsValid    ( false ),
  m_pOgreCamera ( NULL )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Camera::~Camera()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @param[in] pOgreSceneManager Ogre Scene Manager. It allows the creation of the ogre camera
 * @param[in] cameraName        Optional parameter to specify a name for the camera
 * @return true if the initialization was ok | false otherwise
 **/
bool Camera::init( Ogre::SceneManager* pOgreSceneManager, const std::string& cameraName /*= DEFAULT_NAME*/ )
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Check scene manager
  if ( !pOgreSceneManager )
    THROW_EXCEPTION( "Internal Error: NULL Scene Manager" );

  // Create camera  
  m_pOgreCamera = pOgreSceneManager->createCamera( cameraName );
  m_pOgreCamera->setPosition( 2000, 2000, 2000 );
  m_pOgreCamera->lookAt( 0, 0, 0 );

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 **/
void Camera::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  // Delete resources
  System::Release( m_pOgreCamera );

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Updates the class state
 **/
void Camera::update()
{

}

} // namespace Graphics