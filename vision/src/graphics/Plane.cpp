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

#include "Plane.h"
#include "GraphicsManager.h"


namespace Graphics
{

// Static member init
const std::string  Plane::DEFAULT_MESH			= "plane.mesh";
const std::string  Plane::DEFAULT_MATERIAL	= "EmptyMaterial";

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
 */
void Plane::init( float width, float height )
{
	// Create the object 3d of a plane
	Object3D::init( DEFAULT_MESH, DEFAULT_MATERIAL );

	// Scale correction
	width		*= OGRE_SCALE_CORRECTION;
	height	*= OGRE_SCALE_CORRECTION;

	// Set the size
	setScale( width, 1, height );

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
 */
void Plane::init( float size )
{
	return init( size, size );
}

} // namespace Graphics