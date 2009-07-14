/*
  This source file is part of the Vision project
  For the latest info, see http://www.playthemagic.com/vision

Copyright (c) 2008 Julio Obelleiro and Jorge Cano

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
#include "OgreBullet/Dynamics/include/OgreBulletDynamicsRigidBody.h"

namespace Physics
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
PhysicsObject::PhysicsObject():
	m_rigidBody	( NULL  ),
	m_physicsEnabled( NULL )
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
 * @brief Applies a force to the center of the object.
 *
 * @param force Force to apply
 */
void PhysicsObject::applyCentralForce( const Vector& force )
{
	if ( !isValid() || !m_physicsEnabled )
	{
		LOG( "Trying to apply a force to an invalid physics object, or physics are not enabled in this object" );
		return;
	}

	getBulletRigidBody()->applyCentralForce( OgreBulletCollisions::OgreBtConverter::to( force ) );
}

/**
 * @brief Applies an impulse to the center of the object.
 *
 * @param impulse Impulse to apply
 */
void PhysicsObject::applyCentralImpulse( const Vector& impulse )
{
	if ( !isValid() || !m_physicsEnabled )
	{
		LOG( "Trying to apply a impulse to an invalid physics object, or physics are not enabled in this object" );
		return;
	}

	getBulletRigidBody()->applyCentralImpulse( OgreBulletCollisions::OgreBtConverter::to( impulse ) );
}

/**
 * @brief Applies a force from a relative position (to the object's center)
 *
 * @param force Force to apply
 */
void PhysicsObject::applyForce( const Vector& force, const Vector& relPos )
{
	if ( !isValid() || !m_physicsEnabled )
	{
		LOG( "Trying to apply a force to an invalid physics object, or physics are not enabled in this object" );
		return;
	}

	getBulletRigidBody()->applyForce( OgreBulletCollisions::OgreBtConverter::to( force ), OgreBulletCollisions::OgreBtConverter::to( relPos ) );
}

/**
 * @brief Applies an impulse from a relative position (to the object's center)
 *
 * @param impulse Impulse to apply
 */
void PhysicsObject::applyImpulse( const Vector& impulse, const Vector& relPos )
{
	if ( !isValid() || !m_physicsEnabled )
	{
		LOG( "Trying to apply a impulse to an invalid physics object, or physics are not enabled in this object" );
		return;
	}

	getBulletRigidBody()->applyImpulse( OgreBulletCollisions::OgreBtConverter::to( impulse ), OgreBulletCollisions::OgreBtConverter::to( relPos ) );
}

/**
 * @brief Applies a torque to the center of the object.
 *
 * @param torque Torque to apply
 */
void PhysicsObject::applyTorque( const Vector& torque )
{
	if ( !isValid() || !m_physicsEnabled )
	{
		LOG( "Trying to apply a torque to an invalid physics object, or physics are not enabled in this object" );
		return;
	}

	getBulletRigidBody()->applyTorque( OgreBulletCollisions::OgreBtConverter::to( torque ) );
}

/**
 * @brief Applies a torque to the center of the object.
 *
 * @param torque Torque to apply
 */
void PhysicsObject::applyTorqueImpulse( const Vector& torque )
{
	if ( !isValid() || !m_physicsEnabled )
	{
		LOG( "Trying to apply a torque impulse to an invalid physics object, or physics are not enabled in this object" );
		return;
	}

	getBulletRigidBody()->applyTorqueImpulse( OgreBulletCollisions::OgreBtConverter::to( torque ) );
}

/*
 * @brief Returns the bullet rigid body object. Through this object, all the bullet physics related to the rigid
 * body can be accessed
 *
 * @return the bullet rigid body object
 */
btRigidBody* PhysicsObject::getBulletRigidBody()	
{ 
	return m_rigidBody? m_rigidBody->getBulletRigidBody(): NULL; 
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

	// Activate the rigid body
	getBulletRigidBody()->activate(); 
}


} // namespace Physics