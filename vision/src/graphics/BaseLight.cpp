/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

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
#include "Ogre3d/include/OgreStringConverter.h"
#include "Ogre3d/include/OgreSceneManager.h"
#include "Ogre3d/include/OgreBillboardSet.h"
#include "Ogre3d/include/OgreBillboard.h"

// Common
#include "common/Exception.h"

// Framework
#include "framework/Application.h"

namespace Cing
{

// Static member initialization
const std::string  BaseLight::DEFAUTL_LIGHT_NAME  = "Light";
long               BaseLight::m_lightCounter      = 0;

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
BaseLight::BaseLight():
	m_sceneNode 		( NULL  ),
	m_pLight    		( NULL  ),
	m_lightFlare		( NULL  ),
	m_lightFlareSet ( NULL  ),
	m_bIsValid			( false )
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
 * @param[in] r Red component of the diffuse color of the light.    Value Range: 0..255
 * @param[in] g Green component of the diffuse color of the light.  Value Range: 0..255
 * @param[in] b Blue component of the diffuse color of the light.   Value Range: 0..255
 * @param[in] x x coordinate (horizontal axis) of the light in the scene
 * @param[in] y y coordinate (vertical axis) of the light in the scene
 * @param[in] z z coordinate (depth) of the light in the scene
 * @return true if the initialization was ok | false otherwise
 */
bool BaseLight::init( float r, float g, float b, float x, float y, float z )
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Check application correctly initialized (could not be if the user didn't calle size function)
  Application::getSingleton().checkSubsystemsInit();

  // Get the scene manager
  Ogre::SceneManager& scenaManager = GraphicsManager::getSingleton().getSceneManager();

  // Create the light (and its unique name)
  m_lightName = DEFAUTL_LIGHT_NAME + Ogre::StringConverter::toString( ++m_lightCounter );
  m_pLight = scenaManager.createLight( m_lightName );

  // Create a scene node for the light (to have a more convenient way of controlling it in the space)
  m_sceneNode = scenaManager.getRootSceneNode()->createChildSceneNode();
  m_sceneNode->attachObject( m_pLight );

	// Create billboard for debugging purposes
	m_lightFlareSet = GraphicsManager::getSingleton().getSceneManager().createBillboardSet( m_lightName + "_billboard", 1 );
	m_lightFlareSet->setMaterialName( "Examples/Flare" );
	m_lightFlare = m_lightFlareSet->createBillboard( Vector( 0, 0, 0 ) );
	m_lightFlare->setColour( Color( r, g, b, 255 ).normalized() );
	m_lightFlareSet->setVisible( false );

	// Attach billboard to light scene node
	m_sceneNode->attachObject( m_lightFlareSet );

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
	Ogre::SceneManager* sceneManager = GraphicsManager::getSingleton().getSceneManagerPtr();
	if ( sceneManager )
	{
		sceneManager->destroyLight( m_lightName );
		sceneManager->destroyBillboardSet( m_lightFlareSet );
	}

	// Just reset billboard pointers (ogre scene manager is suposed to delete them)
	m_lightFlareSet = NULL;
	m_lightFlare		= NULL;

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
void BaseLight::setPosition( const Vector& pos )
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
void BaseLight::setDirection( const Vector& dir )
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

	setDiffuseColor( Color( r, g, b ) );
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

  m_pLight->setDiffuseColour( color.normalized() );
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

	setSpecularColor( Color( r, g, b ) );
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

  m_pLight->setSpecularColour( color.normalized() );
}

/**
 * @internal
 * Changes the type of light.
 * Available types are:
 *  Light::LT_POINT:        point light source that emits light equally in all directions.
 *  Light::LT_DIRECTIONAL:  light source with no position, only direction. Light range is infinite.
 *  Light::LT_SPOTLIGHT:    light source with position and direction (and many other tunning possibilities).
 * @param[in] type Type to set to the light
 */
void BaseLight::setLightType( Light::LightTypes type )
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to change the type of a Light not correctly initialized" );
  
  m_pLight->setType( type );
}

/**
 * @brief Makes the light visible in the scene (for debugging purposes)
 *
 * @param draw if true, the light will be visible. If false, it will be invisible (although it will continue
 * lighting up the scene)
 */
void BaseLight::drawDebug( bool draw )
{
	  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to draw a Light not correctly initialized" );

	m_lightFlareSet->setVisible( draw );
}

/**
 * @brief Returns the position of the light
 * @return the position of the light
 */
const Vector& BaseLight::getPosition() const
{
  if ( !isValid() )
    THROW_EXCEPTION( "Error. Trying to get the position of a Light not correctly initialized" );
  
  return m_sceneNode->getPosition();
}

} // namespace Cing