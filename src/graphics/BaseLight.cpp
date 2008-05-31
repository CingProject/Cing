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

#include "BaseLight.h"
#include "graphics/GraphicsManager.h"

// Ogre
#include "externLibs/Ogre3d/include/OgreStringConverter.h"
#include "externLibs/Ogre3d/include/OgreSceneManager.h"

// Common
#include "common/Exception.h"

namespace Graphics
{

// Static member initialization
const std::string  BaseLight::DEFAUTL_LIGHT_NAME  = "Light";
long               BaseLight::m_lightCounter      = 0;

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
BaseLight::BaseLight():
	m_sceneNode ( NULL  ),
	m_pLight    ( NULL  ),
	m_bIsValid  ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
BaseLight::~BaseLight()
{
	// Release resources
	end();
}

/**
 * @internal
 * @brief Creates the light and initializes its values
 *
 * @return true if the initialization was ok | false otherwise
 */
bool BaseLight::init()
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Get the scene manager
  Ogre::SceneManager& scenaManager = GraphicsManager::getSingleton().getSceneManager();

  // Create the light (and its unique name)
  m_lightName = DEFAUTL_LIGHT_NAME + Ogre::StringConverter::toString( ++m_lightCounter );
  m_pLight = scenaManager.createLight( m_lightName );
 
  // Create a scene node for the light (to have a more convenient way of controlling it in the space)
  m_sceneNode = scenaManager.getRootSceneNode()->createChildSceneNode();
  m_sceneNode->attachObject( m_pLight );

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void BaseLight::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;
  
  // Destroy the light and the scene node
  GraphicsManager::getSingleton().getSceneManager().destroyLight( m_lightName );

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Sets the new position of the light.
 * @param[in] x new x coordinate (horizontal axis) of the light in the scene
 * @param[in] y new y coordinate (vertical axis) of the light in the scene
 * @param[in] z new z coordinate (depth) of the light in the scene
 */
void BaseLight::setPosition( float x, float y, float z )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set position in a Light not correctly initialized" );

  // Set the absolute light position
  m_sceneNode->setPosition( x, y, z );
}

/**
 * @internal
 * @brief Sets the new position of the light. Z coordinate (depth) is set to zero
 * @param[in] x new x coordinate (horizontal axis) of the light in the scene
 * @param[in] y new y coordinate (vertical axis) of the light in the scene
 */
void BaseLight::setPosition( float x, float y )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set position in a Light not correctly initialized" );

  // Set the absolute light position
  m_sceneNode->setPosition( x, y, 0.0f );
}

/**
 * @internal
 * @brief Sets the new position of the light. Z coordinate (depth) is set to zero
 * @param[in] pos new position (horizontal axis) of the light in the scene
 */
void BaseLight::setPosition( const Vector3& pos )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set position in a Light not correctly initialized" );

  // Set the absolute light position
  m_sceneNode->setPosition( pos.x, pos.y, pos.z );
}

/**
 * @internal
 * @brief Sets the new direction of the light.
 * @param[in] x new x coordinate (horizontal axis) of the light in the scene
 * @param[in] y new y coordinate (vertical axis) of the light in the scene
 * @param[in] z new z coordinate (depth) of the light in the scene
 */
void BaseLight::setDirection( float x, float y, float z )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set direction in a Light not correctly initialized" );

  // Set the absolute light direction
  m_sceneNode->setDirection( x, y, z );
}

/**
 * @internal
 * @brief Sets the new direction of the light. Z coordinate (depth) is set to zero
 * @param[in] x new x coordinate (horizontal axis) of the light in the scene
 * @param[in] y new y coordinate (vertical axis) of the light in the scene
 */
void BaseLight::setDirection( float x, float y )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set direction in a Light not correctly initialized" );

  // Set the absolute light direction
  m_sceneNode->setDirection( x, y, 0.0f );
}

/**
 * @internal
 * @brief Sets the new direction of the light. Z coordinate (depth) is set to zero
 * @param[in] dir new direction (horizontal axis) of the light in the scene
 */
void BaseLight::setDirection( const Vector3& dir )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set direction in a Light not correctly initialized" );

  // Set the absolute light direction
  m_sceneNode->setDirection( dir.x, dir.y, dir.z );
}

/**
 * @internal
 * @brief Sets the diffuse (general) color of the light
 * @param[in] r Red component of the new color of the light.    Value Range: 0..1
 * @param[in] g Green component of the new color of the light.  Value Range: 0..1
 * @param[in] b Blue component of the new color of the light.   Value Range: 0..1
 */
void BaseLight::setDiffuseColor( float r, float g, float b )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set diffuse color in a Light not correctly initialized" );

  // Set the absolute light direction
  m_pLight->setDiffuseColour( r, g, b );
}


/**
 * @internal
 * @brief Sets the diffuse (general) color of the light
 * @param[in] color New color of the light
 */
void BaseLight::setDiffuseColor( const Color& color )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set diffuse color in a Light not correctly initialized" );

  // Set the absolute light direction
  m_pLight->setDiffuseColour( color );
}

/**
 * @internal
 * @brief Sets the specular (shines) color of the light
 * @param[in] r Red component of the new specular color of the light.    Value Range: 0..1
 * @param[in] g Green component of the new specular color of the light.  Value Range: 0..1
 * @param[in] b Blue component of the new specular color of the light.   Value Range: 0..1
 */
void BaseLight::setSpecularColor( float r, float g, float b )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set specular color in a Light not correctly initialized" );

  // Set the absolute light direction
  m_pLight->setDiffuseColour( r, g, b );
}


/**
 * @internal
 * @brief Sets the specular (shines) color of the light
 * @param[in] color New color of the light
 */
void BaseLight::setSpecularColor( const Color& color )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to set specular color in a Light not correctly initialized" );

  // Set the absolute light direction
  m_pLight->setDiffuseColour( color );
}

/**
 * @internal
 * Changes the type of light.
 * Available types are:
 *  Light::LT_POINT:        point light source that emits light equally in all directions.
 *  Light::LT_DIRECTIONAL:  light source with no position, only direction. Light range is infinite.
 *  Light::LT_SPOTLIGHT:    light source with position and direction (and many other tunning posibilities).
 * @param[in] type Type to set to the light
 */
void BaseLight::setLightType( Light::LightTypes type )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to change the type of a Light not correctly initialized" );
  
  m_pLight->setType( type );
}

/**
 * @brief Returns the position of the light
 * @return the position of the light
 */
const Pos3D& BaseLight::getPosition() const
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to get the position of a Light not correctly initialized" );
  
  return m_sceneNode->getPosition();
}

} // namespace Graphics