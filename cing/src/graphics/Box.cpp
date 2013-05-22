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

#include "Box.h"
#include "GraphicsManager.h"

namespace Cing
{

// Static member init
const std::string  Box::DEFAULT_MESH		= "box.mesh";
const std::string  Box::DEFAULT_MATERIAL	= "EmptyMaterial";

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Box::Box():
	m_bIsValid  ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Box::~Box()
{
	// Release resources
	end();
}

/**
 * @internal
 * @brief Creates a box with a specific size in each axis
 * @param[in] width		Width of the box
 * @param[in] height	Height of the box
 * @param[in] depth		Depth of the box
 */
void Box::init( float width, float height, float depth )
{
	// Create the object 3d of a box
	Object3D::init( DEFAULT_MESH, DEFAULT_MATERIAL );

	// Scale correction
	width		*= OGRE_SCALE_CORRECTION;
	height		*= OGRE_SCALE_CORRECTION;
	depth		*= OGRE_SCALE_CORRECTION;

	// Set the size
	setScale( width, height, depth );

	// Store scale
	m_width		= width;
	m_height	= height;
	m_depth		= depth;

	// This is a primitive object type (Box)
	setType( BOX );

	m_bIsValid = true;
}


/**
 * @internal
 * @brief Creates a box with the same size in the three axis
 * @param[in] size Defines the size of the cube (the same in the three axis: width, height, depth)
 */
void Box::init( float size )
{
	return init( size, size, size );
}

} // namespace Cing