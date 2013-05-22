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

#include "Sphere.h"
#include "GraphicsManager.h"

namespace Cing
{

// Static member init
const std::string  Sphere::DEFAULT_MESH		= "sphere.mesh";
const std::string  Sphere::DEFAULT_MATERIAL	= "EmptyMaterial";

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Sphere::Sphere():
	m_bIsValid  ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Sphere::~Sphere()
{
	// Release resources
	end();
}

/**
 * @internal
 * @brief Creates a sphere with a specific radius
 * @param[in] diameter		Diameter of the sphere
 * @param[in] parent		   Parent scene node. If not passed a new node will be created
 */
void Sphere::init( float diameter, Ogre::SceneNode* parent /*= NULL*/  )
{
	// Create the object 3d of a sphere
	Object3D::init( DEFAULT_MESH, DEFAULT_MATERIAL, parent );

	// Scale correction
	diameter *= OGRE_SCALE_CORRECTION;

	// Set the size
	m_radius = diameter / 2.0f;
	setScale( m_radius, m_radius, m_radius );

	// This is a primitive object type (Sphere)
	setType( SPHERE );

	m_bIsValid = true;
}


} // namespace Cing