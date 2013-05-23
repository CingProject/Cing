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