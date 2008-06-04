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

#include "Sphere.h"
#include "GraphicsManager.h"

// Physics
#include "physics/PhysicsSphere.h"

namespace Graphics
{

// Static member init
const std::string  Sphere::DEFAULT_MESH			= "sphere.mesh";
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
 * @param[in] radius		Radius of the sphere
 */
void Sphere::init( int radius )
{
	// Create the object 3d of a sphere
	Object3D::init( DEFAULT_MESH, DEFAULT_MATERIAL );

	// Set the size
	setScale( radius, radius, radius );

	// Store scale
	m_radius	= radius;

	// This is a primitive object type (Sphere)
	setType( SPHERE );

	m_bIsValid = true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void Sphere::end()
{

	m_bIsValid = false;
}


/*
 * @brief Activates the physics for this Sphere
 * 
 * From now on, the sphere's position and rotation will be driven by the physics of the scene.
 */
void Sphere::activatePhysics()
{
	// Create and init the physics object
	m_physicsObject = new Physics::PhysicsSphere();
	m_physicsObject->init( *this );
}

} // namespace Graphics