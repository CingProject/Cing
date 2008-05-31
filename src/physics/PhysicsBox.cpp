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

#include "PhysicsBox.h"

// Graphics
#include "graphics/Box.h"

// Common
#include "common/Exception.h"

// Ogre Bullet
#include "externLibs/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsBoxShape.h"

namespace Physics
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
PhysicsBox::PhysicsBox():
	PhysicsObject(),
	m_bIsValid  ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
PhysicsBox::~PhysicsBox()
{
	// Release resources
	end();
}

/**
 * @internal
 * @brief Initializes the physics box object
 *
 * @param object 3D object (a box is expected) for which the physics box model will be created
 */
void PhysicsBox::init( Graphics::Object3D& object )
{
	// Make sure the object is a box
	if ( object.getType() != Graphics::Object3D::BOX )
		THROW_EXCEPTION( "Trying to create PhysicsBox for an object that is not a BOX" );

	// Cast the object to a box
	Graphics::Box& box = static_cast< Graphics::Box& >( object );
	
	// Create the box collision shape
	Vector3 size ( box.getWidth(), box.getHeight(), box.getDepth() );
	OgreBulletCollisions::BoxCollisionShape *boxShape = new OgreBulletCollisions::BoxCollisionShape( size );

	// Create the rigid body and assign it the collision shape
	createRigidBody( object, boxShape );

	m_bIsValid = true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void PhysicsBox::end()
{

	m_bIsValid = false;
}

} // namespace Physics