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
#include "externLibs/OgreBullet/Collisions/include/Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "externLibs/OgreBullet/Dynamics/include/OgreBulletDynamicsRigidBody.h"
#include "externLibs/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsTrimeshShape.h"
#include "externLibs/OgreBullet/Collisions/include/OgreBulletCollisionsShape.h"
#include "externLibs/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsBoxShape.h"
#include "externLibs/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsStaticPlaneShape.h"
#include "externLibs/OgreBullet/Collisions/include/Shapes/OgreBulletCollisionsSphereShape.h"


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
 * @brief Initializes the physics for this object, and creates the collision shape to control its collisions
 *
 * @param object				Object that will be controlled by this physic object
 * @param staticObject	If true, this object will be static, this means that this object is affected by 
 * physics objects and forces of the scene (so it will collide with other physics object), but won't move.
 */
void PhysicsObject::init(  Graphics::Object3D& object, bool staticObject )
{
	// Create collision shape depending on object type
	OgreBulletCollisions::CollisionShape*	collisionShape = NULL;
	switch( object.getType() )
	{
		case Graphics::Object3D::BOX:	
			collisionShape = buildBoxShape( static_cast< Graphics::Box& >( object ) );
		break;
		
		case Graphics::Object3D::SPHERE:
			collisionShape = buildSphereShape( static_cast< Graphics::Sphere& >( object ) );
		break;
		
		//case Graphics::Object3D::CYLINDER:
		//break;
		//
		//case Graphics::Object3D::CONE:
		//break;
		
		case Graphics::Object3D::PLANE:
			staticObject = true;
			collisionShape = buildStaticPlaneShape( static_cast< Graphics::Plane& >( object ) );
		break;
		
		case Graphics::Object3D::TRIANGLE_MESH:
			collisionShape = buildTriMeshShape( object );
		break;

		default:
			LOG( "Trying to initialize a physics object from an unknown object type" );
	};

	// Create the rigid body and assign it the collision shape
	createRigidBody( object, collisionShape, staticObject );
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void PhysicsObject::end()
{
	// Release data
	Common::Release( m_rigidBody );

	m_bIsValid = false;
}


/**
 * @internal 
 * @brief Creates a collision shape based on a triangle mesh.
 * 
 * This method should be used when there is no primitive shape that fits well for a specific mesh
 *
 * @param object Object for which the shape will be created
 */
OgreBulletCollisions::CollisionShape* PhysicsObject::buildTriMeshShape( Graphics::Object3D& object )
{
	// Convert the triangle mesh of the object to a collision shape
	OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter	= new OgreBulletCollisions::StaticMeshToShapeConverter( object.getEntity() );
	OgreBulletCollisions::TriangleMeshCollisionShape *triMeshShape = trimeshConverter->createTrimesh();
	delete trimeshConverter;

	return triMeshShape;
}

/**
 * @internal 
 * @brief Creates a collision for a Box object
 *
 * @param box Box for which the collision shape will be created
 */
OgreBulletCollisions::CollisionShape* PhysicsObject::buildBoxShape( Graphics::Box& box )
{
	// Obtain box correct size
	Vector size ( box.getWidth(), box.getHeight(), box.getDepth() );
	size /= 2.0f; // bullet expects half box size
	size /= Graphics::Object3D::OGRE_SCALE_CORRECTION; // To correct scale problems between ogre and maya exporter

	// Create the shape
	OgreBulletCollisions::BoxCollisionShape *boxShape = new OgreBulletCollisions::BoxCollisionShape( size );

	return boxShape;
}

/**
 * @internal 
 * @brief Creates a collision for a Plane object
 *
 * @param plane Plane for which the collision shape will be created
 */
OgreBulletCollisions::CollisionShape* PhysicsObject::buildStaticPlaneShape( Graphics::Plane& plane )
{
	// Calculate plane normal and distance
	Vector p1, p2, p3;
	float halfWidth		= plane.getWidth() / 2.0f;
	float halfHeight	= plane.getHeight() / 2.0f;
	p1.x = -halfWidth; p1.z = -halfHeight;	p1.y = 0.0f;
	p2.x = halfWidth;  p2.z = -halfHeight;	p2.y = 0.0f;
	p3.x = -halfWidth; p3.z = halfHeight;	p3.y = 0.0f;

	// Edges vectors
	Vector v12( p2 - p1 );
	Vector v13( p3 - p1 );

	// Calculate normal
	Vector normal( v13.crossProduct( v12 ) );
	normal.normalise();
	float	 distance = 0.0f;

	// Create collision shape
	OgreBulletCollisions::StaticPlaneCollisionShape* planeShape = new OgreBulletCollisions::StaticPlaneCollisionShape( normal, distance );

	return planeShape;
}

/**
 * @internal 
 * @brief Creates a collision for a Sphere object
 *
 * @param sphere Sphere for which the collision shape will be created
 */
OgreBulletCollisions::CollisionShape* PhysicsObject::buildSphereShape( Graphics::Sphere& sphere )
{
	// Create the sphere collision shape
	float radius = sphere.getRadius() / Graphics::Object3D::OGRE_SCALE_CORRECTION; // To correct scale problems between ogre and maya exporter
	OgreBulletCollisions::SphereCollisionShape *sphereShape = new OgreBulletCollisions::SphereCollisionShape( radius );

	return sphereShape;
}


/**
 * @brief Creates a rigid body for the physics object and assigns it a collision shape
 *
 * @param		object					3D Object for which the rigid body will be created
 * @param		collisionShape	Collision shape that will be use to control the collisions of the object
 * @param		staticBody			If true, the rigid body created will be static. Therefore, it will affect collisions of other
 * physics objects, but it won't move.
 */
void PhysicsObject::createRigidBody( Graphics::Object3D& object, OgreBulletCollisions::CollisionShape* collisionShape, bool staticBody )
{
	// Create the rigid body
	static int rigidBodyCount = 0;
	rigidBodyCount++;
	m_rigidBody = new OgreBulletDynamics::RigidBody( object.getName() + "RigidBody" + Ogre::StringConverter::toString( rigidBodyCount ), PhysicsManager::getSingleton().getWorld() );

	// Attach the collision shape
	if ( staticBody )
	{
		m_rigidBody->setStaticShape(	collisionShape, 
																	Physics::DEFAULT_DYNAMIC_BODY_RESTITUTION, 
																	Physics::DEFAULT_DYNAMIC_BODY_FRICTION, object.getPosition(), object.getOrientation() );
	}
	else
	{
		// Note: The position and rotation of the object are stored here (and not as references)
		// because the Ogrebullet lib has a bug, and resets the node position to zero, even when you speficy one...
		// So, I store position and orientation to pass them by value, instead of by reference
		Vector pos = object.getPosition();
		Quaternion orientation = object.getOrientation();

		m_rigidBody->setShape(	object.getSceneNode(), 
														collisionShape, 
														Physics::DEFAULT_DYNAMIC_BODY_RESTITUTION, 
														Physics::DEFAULT_DYNAMIC_BODY_FRICTION, 
														Physics::DEFAULT_DYNAMIC_BODY_MASS,
														pos,
														orientation );

	}
}


} // namespace Physics