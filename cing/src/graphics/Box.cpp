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