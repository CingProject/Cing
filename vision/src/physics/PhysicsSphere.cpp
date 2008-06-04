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

#include "PhysicsSphere.h"

// Graphics
#include "graphics/Sphere.h"

// Common
#include "common/Exception.h"

// Ogre Bullet
#include "externLibs/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsSphereShape.h"

namespace Physics
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
PhysicsSphere::PhysicsSphere():
	PhysicsObject(),
	m_bIsValid  ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
PhysicsSphere::~PhysicsSphere()
{
	// Release resources
	end();
}

/**
 * @internal
 * @brief Initializes the physics sphere object
 *
 * @param object 3D object (a sphere is expected) for which the physics sphere model will be created
 */
void PhysicsSphere::init( Graphics::Object3D& object )
{
	// Make sure the object is a sphere
	if ( object.getType() != Graphics::Object3D::SPHERE )
		THROW_EXCEPTION( "Trying to create PhysicsSphere for an object that is not a SPHERE" );

	// Cast the object to a sphere
	Graphics::Sphere& sphere = static_cast< Graphics::Sphere& >( object );
	
	// Create the sphere collision shape
	OgreBulletCollisions::SphereCollisionShape *sphereShape = new OgreBulletCollisions::SphereCollisionShape( sphere.getRadius() );

	// Create the rigid body and assign it the collision shape
	createRigidBody( object, sphereShape );

	m_bIsValid = true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void PhysicsSphere::end()
{

	m_bIsValid = false;
}

} // namespace Physics