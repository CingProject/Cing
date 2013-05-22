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

#include "Plane.h"
#include "GraphicsManager.h"


namespace Cing
{

// Static member init
const std::string  Plane::DEFAULT_MESH			= "plane.mesh";
const std::string  Plane::DEFAULT_MATERIAL	= "EmptyMaterialNoCulling";

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Plane::Plane():
	m_bIsValid  ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Plane::~Plane()
{
	// Release resources
	end();
}

/**
 * @internal
 * @brief Creates a plane with a specific size in each axis
 * @param[in] width		Width of the plane
 * @param[in] height	Height of the plane
 * @param[in] depth		Depth of the plane
 * @param[in] parent		   Parent scene node. If not passed a new node will be created
 */
void Plane::init( float width, float height, Ogre::SceneNode* parent /*= NULL*/ )
{
	// Create the object 3d of a plane
	Object3D::init( DEFAULT_MESH, DEFAULT_MATERIAL, parent );

	// Scale correction
	width		*= OGRE_SCALE_CORRECTION;
	height	*= OGRE_SCALE_CORRECTION;

	// Set the size
	setScale( width, 1, height );

	// Flip the plane
	// TODO: Flip the actual model!
	setOrientation( Vector::UNIT_X, 180 );

	// Store scale
	m_width		= width;
	m_height	= height;

	// This is a primitive object type (Plane)
	setType( PLANE );

	m_bIsValid = true;
}


/**
 * @internal
 * @brief Creates a plane with the same size in the three axis
 * @param[in] size Defines the size of the cube (the same in the three axis: width, height, depth)
 * @param[in] parent		   Parent scene node. If not passed a new node will be created
 */
void Plane::init( float size, Ogre::SceneNode* parent /*= NULL*/ )
{
	return init( size, size, parent );
}

} // namespace Cing