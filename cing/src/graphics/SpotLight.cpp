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

// Precompiled headers
#include "Cing-Precompiled.h"

#include "SpotLight.h"
#include "common/LogManager.h"

namespace Cing
{

	/**
	* @internal
	* @brief Constructor. Initializes class attributes.
	*/
	SpotLight::SpotLight():
m_bIsValid( false )
{
}

/**
* @internal
* @brief Destructor. Class release.
*/
SpotLight::~SpotLight()
{
	// Release resources
	end();
}

/**
* @internal
* @brief Initializes the class so it becomes valid.
*
* @param[in] r Red component of the diffuse color of the light.    Value Range: 0..255
* @param[in] g Green component of the diffuse color of the light.  Value Range: 0..255
* @param[in] b Blue component of the diffuse color of the light.   Value Range: 0..255
* @param[in] x x coordinate (horizontal axis) of the light in the scene
* @param[in] y y coordinate (vertical axis) of the light in the scene
* @param[in] z z coordinate (depth) of the light in the scene
* @param[in] nx x direction(horizontal axis) of the light in the scene
* @param[in] ny y direction (vertical axis) of the light in the scene
* @param[in] nz z direction (depth) of the light in the scene
* @param[in] angleRad angle in radians of the spotlight cone. It is possible tolater control inner and outer angle of the light.
* @param[in] concentration fall off of the light (Range: 0.0-1.0). Less means slower falloff, higher means faster falloff.
* @return true if the initialization was ok | false otherwise
*/
bool SpotLight::init( float r, float g, float b, float x, float y, float z, float nx, float ny, float nz, float angleRad, float concentration )
{
	// Check if the class is already initialized
	if ( isValid() )
		return true;

	// Init base light
	BaseLight::init( r, g, b, x, y, z );

	// Set light type
	setLightType( Light::LT_SPOTLIGHT );

	// Setup light
	setDirection( nx, ny, nz );
	setDiffuseColor( r, g, b );
	Ogre::Light* light = getOgreLight();
	if ( light )
		light->setSpotlightRange( Ogre::Radian(angleRad), Ogre::Radian(angleRad), concentration );

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
* @internal
* @brief Releases the class resources. 
* After this method is called the class is not valid anymore.
*/
void SpotLight::end()
{
	// Check if the class is already released
	if ( !isValid() )
		return;

	// Release base light
	BaseLight::end();

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @brief Sets the range of the lighet (inner and outer angles of the cone + the falloff between them)
 * @param	innerAngleRad 		Angle covered by the bright inner cone The inner cone applicable only to Direct3D, it'll always treat as zero in OpenGL. 
 * @param	outerAngleRad 		Angle covered by the outer cone 
 * @param	concentration	Fall off of the light.	The rate of falloff between the inner and outer cones. 
 *							1.0 means a linear falloff, less means slower falloff, higher means faster falloff. 
 */
void  SpotLight::setRange( float innerAngleRad, float outerAngleRad, float concentration /*= 1.0f*/ )
{
	// Check if the class is already released
	if ( !isValid() )
	{
		LOG_ERROR( "Cannot setRange. Init needs to be called first to initialize the light" );
		return;
	}

	// Set new range
	Ogre::Light* light = getOgreLight();
	if ( light )
		light->setSpotlightRange( Ogre::Radian(innerAngleRad), Ogre::Radian(outerAngleRad), concentration );
}

/**
 * @brief Sets the inner angle of the light cone
 * @param	innerAngleRad 		Angle covered by the bright inner cone The inner cone applicable only to Direct3D, it'll always treat as zero in OpenGL. 
 */
void  SpotLight::setInnerAngle( float innerAngleRad )
{
	// Check if the class is already released
	if ( !isValid() )
	{
		LOG_ERROR( "Cannot setInnerAngle. Init needs to be called first to initialize the light" );
		return;
	}

	// Set new angle
	Ogre::Light* light = getOgreLight();
	if ( light )
		light->setSpotlightInnerAngle( Ogre::Radian(innerAngleRad) );
}

/**
 * @brief Sets the outer angle of the light cone
 * @param	outerAngleRad 		Angle covered by the outer cone 
 */
void  SpotLight::setOuterAngle( float outerAngleRad )
{
	// Check if the class is already released
	if ( !isValid() )
	{
		LOG_ERROR( "Cannot setOuterAngle. Init needs to be called first to initialize the light" );
		return;
	}

	// Set new angle
	Ogre::Light* light = getOgreLight();
	if ( light )
		light->setSpotlightOuterAngle( Ogre::Radian(outerAngleRad) );
}

/**
 * @brief Sets the outer angle of the light cone
 * @param	concentration	Fall off of the light.	The rate of falloff between the inner and outer cones. 
 *							1.0 means a linear falloff, less means slower falloff, higher means faster falloff. 
 */
void  SpotLight::setConcentration( float concentration )
{
	// Check if the class is already released
	if ( !isValid() )
	{
		LOG_ERROR( "Cannot setConcentration. Init needs to be called first to initialize the light" );
		return;
	}

	// Set new falloff or concentration
	Ogre::Light* light = getOgreLight();
	if ( light )
		light->setSpotlightFalloff( concentration );
}

} // namespace Cing