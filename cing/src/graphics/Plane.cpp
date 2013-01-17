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