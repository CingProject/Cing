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
//#include "PhysicsSphere.h"
//#include "PhysicsManager.h"
//
//// Common
//#include "common/LogManager.h"
//
//namespace Cing
//{
//
//// Static member init
//const std::string  PhysicsSphere::DEFAULT_MESH			= "Sphere.mesh";
//const std::string  PhysicsSphere::DEFAULT_MATERIAL	= "EmptyMaterial";
//
///**
// * @internal
// * @brief Constructor. Initializes class attributes.
// */
//PhysicsSphere::PhysicsSphere():
//	m_bIsValid  ( false )
//{
//}
//
///**
// * @internal
// * @brief Destructor. Class release.
// */
//PhysicsSphere::~PhysicsSphere()
//{
//	// Release resources
//	end();
//}
//
///**
// * @internal
// * @brief Creates a PhysicsSphere with a specific radius
// * @param[in] diameter		Diameter of the PhysicsSphere
// */
//void PhysicsSphere::init( float diameter )
//{
//	// Create the object 3d of a PhysicsSphere
//	Object3D::init( DEFAULT_MESH, DEFAULT_MATERIAL );
//
//	// Scale correction
//	diameter *= OGRE_SCALE_CORRECTION;
//
//	// Set the size
//	m_radius = diameter / 2.0f;
//	setScale( m_radius, m_radius, m_radius );
//
//	// This is a primitive object type (PhysicsSphere)
//	setType( SPHERE );
//
//	m_bIsValid = true;
//}
//
///**
// * @brief Creates the collision shape to control sphere's collisions and activates its physics
// *
// * @param staticObject	If true, this object will be static, this means that this object is affected by 
// * physics objects and forces of the scene (so it will collide with other physics object), but won't move.
// */
//void PhysicsSphere::enablePhysics( bool staticObject /*= false*/ )
//{
//	if ( m_physicsEnabled )
//	{
//		LOG_ERROR_NTIMES( 1, "Trying to enable physics on a sphere that has physics already enabled" );
//		return;
//	}
//
//	// We need the pysics manger activated, so make sure
//	PhysicsManager::getSingleton().enable();
//
//	// Create collision shape for the object based on the triangle mesh
//	OgreBulletCollisions::CollisionShape*	collisionShape =  PhysicsManager::getSingleton().buildSphereShape( m_radius );
//
//	// Create rigid body
//	enableRigidBodyPhysics( collisionShape, staticObject );
//}
//
//} // namespace Cing