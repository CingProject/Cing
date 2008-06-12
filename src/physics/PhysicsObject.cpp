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

#include "PhysicsObject.h"
#include "PhysicsManager.h"

// Common
#include "common/Release.h"
#include "common/LogManager.h"

// Graphics
#include "graphics/Box.h"
#include "graphics/Plane.h"
#include "graphics/Sphere.h"

// Ogre bullet
#include "externLibs/OgreBullet/Dynamics/include/OgreBulletDynamicsRigidBody.h"

namespace Physics
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
PhysicsObject::PhysicsObject():
	m_rigidBody	( NULL  ),
	m_physicsEnabled( NULL ),
	m_bIsValid  ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
PhysicsObject::~PhysicsObject()
{
	// Release resources
	end();
}


/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void PhysicsObject::end()
{
	// Release data if physics world is still alive...
	if ( PhysicsManager::getSingleton().getWorld() )
		Common::Release( m_rigidBody );

	// Release parent class stuff
	Object3D::end();

	m_bIsValid = false;
}

/**
 * @brief Creates the collision shape to control object's collisions and activates its physics
 *
 * @param staticObject	If true, this object will be static, this means that this object is affected by 
 * physics objects and forces of the scene (so it will collide with other physics object), but won't move.
 */
void PhysicsObject::enablePhysics( bool staticObject )
{
	if ( m_physicsEnabled )
	{
		LOG( "Trying to enable physics on an object that has physics already enabled" );
		return;
	}

	// Create collision shape for the object based on the triangle mesh
	OgreBulletCollisions::CollisionShape*	collisionShape =  PhysicsManager::getSingleton().buildTriMeshShape( *this );	

	enableRigidBodyPhysics( collisionShape, staticObject );
	m_physicsEnabled = true;
}


/*
 * @brief DeActivates the physics for this object. 
 * 
 * From now on, the object's position and rotation will be again controlled manually
 */
void PhysicsObject::disablePhysics()
{
	// Release physics's rigid body
	Common::Release( m_rigidBody );
	m_physicsEnabled = false;
}

/**
 * @internal 
 * @brief Enables rigid body physics using the received collision shape
 *
 * @param collisionShape	collision shape to use by the rigid body
 * @param staticObject		if true the object will be static (regarding the physics)				
 */
void PhysicsObject::enableRigidBodyPhysics( OgreBulletCollisions::CollisionShape* collisionShape, bool staticObject )
{
	// If previously enabled -> disable first to avoid memory leak
	if ( m_rigidBody )
		Common::Release( m_rigidBody );

	// Create the rigid body and assign it the collision shape
	m_rigidBody = PhysicsManager::getSingleton().createRigidBody( *this, collisionShape, staticObject );
	m_physicsEnabled = true;
}


} // namespace Physics