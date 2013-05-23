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

#include "DirectionalLight.h"

namespace Cing
{

	/**
	* @internal
	* @brief Constructor. Initializes class attributes.
	*/
	DirectionalLight::DirectionalLight():
m_bIsValid( false )
{
}

/**
* @internal
* @brief Destructor. Class release.
*/
DirectionalLight::~DirectionalLight()
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
* @param[in] nx x direction(horizontal axis) of the light in the scene
* @param[in] ny y direction (vertical axis) of the light in the scene
* @param[in] nz z direction (depth) of the light in the scene
* @return true if the initialization was ok | false otherwise
*/
bool DirectionalLight::init( float r, float g, float b, float nx, float ny, float nz )
{
	// Check if the class is already initialized
	if ( isValid() )
		return true;

	// Init base light
	BaseLight::init( r, g, b, nx, ny, nz );

	// Setup light
	setDirection( nx, ny, nz );
	setDiffuseColor( r, g, b );

	// Set light type
	setLightType( Light::LT_DIRECTIONAL );

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
* @internal
* @brief Releases the class resources. 
* After this method is called the class is not valid anymore.
*/
void DirectionalLight::end()
{
	// Check if the class is already released
	if ( !isValid() )
		return;

	// Release base light
	BaseLight::end();

	// The class is not valid anymore
	m_bIsValid = false;
}

} // namespace Cing