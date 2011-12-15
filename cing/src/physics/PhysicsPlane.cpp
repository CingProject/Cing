///*
//  This source file is part of the Cing project
//  For the latest info, see http://www.cing.cc
//
//  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software Foundation,
//  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//*/
//
//// Precompiled headers
#include "Cing-Precompiled.h"
//
//#include "PhysicsPlane.h"
//#include "PhysicsManager.h"
//
//// Common
//#include "common/LogManager.h"
//
//namespace Cing
//{
//
//// Static member init
//const std::string  PhysicsPlane::DEFAULT_MESH			= "Plane.mesh";
//const std::string  PhysicsPlane::DEFAULT_MATERIAL	= "EmptyMaterial";
//
///**
// * @internal
// * @brief Constructor. Initializes class attributes.
// */
//PhysicsPlane::PhysicsPlane():
//	m_bIsValid  ( false )
//{
//}
//
///**
// * @internal
// * @brief Destructor. Class release.
// */
//PhysicsPlane::~PhysicsPlane()
//{
//	// Release resources
//	end();
//}
//
///**
// * @internal
// * @brief Creates a PhysicsPlane with a specific size in each axis
// * @param[in] width		Width of the PhysicsPlane
// * @param[in] height	Height of the PhysicsPlane
// * @param[in] depth		Depth of the PhysicsPlane
// */
//void PhysicsPlane::init( float width, float height )
//{
//	// Create the object 3d of a PhysicsPlane
//	Object3D::init( DEFAULT_MESH, DEFAULT_MATERIAL );
//
//	// Scale correction
//	width		*= OGRE_SCALE_CORRECTION;
//	height	*= OGRE_SCALE_CORRECTION;
//
//	// Set the size
//	setScale( width, 1, height );
//
//	// Store scale
//	m_width		= width;
//	m_height	= height;
//
//	// This is a primitive object type (PhysicsPlane)
//	setType( PLANE );
//
//	m_bIsValid = true;
//}
//
//
///**
// * @internal
// * @brief Creates a PhysicsPlane with the same size in the three axis
// * @param[in] size Defines the size of the cube (the same in the three axis: width, height, depth)
// */
//void PhysicsPlane::init( float size )
//{
//	return init( size, size );
//}
//
///**
// * @brief Creates the collision shape to control plane's collisions and activates its physics
// *
// * @param staticObject	If true, this object will be static, this means that this object is affected by 
// * physics objects and forces of the scene (so it will collide with other physics object), but won't move.
// */
//void PhysicsPlane::enablePhysics( bool staticObject /*= false*/ )
//{
//	if ( m_physicsEnabled )
//	{
//		LOG( "Trying to enable physics on an object that has physics already enabled" );
//		return;
//	}
//
//	// We need the pysics manger activated, so make sure
//	PhysicsManager::getSingleton().enable();
//
//	// Create collision shape for the object based on the triangle mesh
//	OgreBulletCollisions::CollisionShape* collisionShape = PhysicsManager::getSingleton().buildStaticPlaneShape( m_width, m_height );
//
//	// Create the rigid body and assign it the collision shape
//	m_rigidBody = PhysicsManager::getSingleton().createRigidBody( *this, collisionShape, staticObject );	
//	m_physicsEnabled = true;
//}
//
//} // namespace Cing