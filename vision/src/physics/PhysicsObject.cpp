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

// Ogre bullet
#include "externLibs/OgreBullet/Collisions/include/Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "externLibs/OgreBullet/Dynamics/include/OgreBulletDynamicsRigidBody.h"
#include "externLibs/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "externLibs/OgreBullet/Collisions/include/OgreBulletCollisionsShape.h"

namespace Physics
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
PhysicsObject::PhysicsObject():
	m_rigidBody	( NULL  ),
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
 * @brief Creates the collision shape to control the collisions of an object in the scene
 *
 * @param object	Object that will be controlled by this physic object
 */
void PhysicsObject::init( Graphics::Object3D& object )
{
	// TODO: chequear si es necesario delete del shape

	// Convert the triangle mesh of the object to a collision shape
	OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter	= new OgreBulletCollisions::StaticMeshToShapeConverter( object.getEntity() );
	OgreBulletCollisions::TriangleMeshCollisionShape *triMeshShape = trimeshConverter->createTrimesh();
	delete trimeshConverter;

	// Create the rigid body and assign it the collision shape
	createRigidBody( object, triMeshShape );

	m_bIsValid = true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void PhysicsObject::end()
{

	// Release data
	OgreBulletCollisions::CollisionShape* collisionShape = m_rigidBody->getShape();
	Common::Release( collisionShape );
	Common::Release( m_rigidBody );

	m_bIsValid = false;
}

/**
 * @brief Creates a rigid body for the physics object and assigns it a collision shape
 *
 * @param		object					3D Object for which the rigid body will be created
 * @param		collisionShape	Collision shape that will be use to control the collisions of the object
 */
void PhysicsObject::createRigidBody( Graphics::Object3D& object, OgreBulletCollisions::CollisionShape* collisionShape )
{
	// Create the rigid body
	m_rigidBody = new OgreBulletDynamics::RigidBody( object.getName() + "RigidBody", PhysicsManager::getSingleton().getWorld() );

	// Attach the collision shape
	m_rigidBody->setShape(	object.getSceneNode(), 
													collisionShape, 
													Physics::DEFAULT_DYNAMIC_BODY_RESTITUTION, 
													Physics::DEFAULT_DYNAMIC_BODY_FRICTION, 
													Physics::DEFAULT_DYNAMIC_BODY_MASS );

}

} // namespace Physics