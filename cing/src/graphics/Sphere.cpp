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