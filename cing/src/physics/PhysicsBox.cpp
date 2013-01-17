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
//#include "PhysicsBox.h"
//#include "PhysicsManager.h"
//
//// Common
//#include "common/LogManager.h"
//
//namespace Cing
//{
//
//// Static member init
//const std::string  PhysicsBox::DEFAULT_MESH			= "Box.mesh";
//const std::string  PhysicsBox::DEFAULT_MATERIAL	= "EmptyMaterial";
//
///**
// * @internal
// * @brief Constructor. Initializes class attributes.
// */
//PhysicsBox::PhysicsBox():
//	m_bIsValid  ( false )
//{
//}
//
///**
// * @internal
// * @brief Destructor. Class release.
// */
//PhysicsBox::~PhysicsBox()
//{
//	// Release resources
//	end();
//}
//
///**
// * @internal
// * @brief Creates a PhysicsBox with a specific size in each axis
// * @param[in] width		Width of the PhysicsBox
// * @param[in] height	Height of the PhysicsBox
// * @param[in] depth		Depth of the PhysicsBox
// */
//void PhysicsBox::init( float width, float height, float depth )
//{
//	// Create the object 3d of a PhysicsBox
//	Object3D::init( DEFAULT_MESH, DEFAULT_MATERIAL );
//
//	// Scale correction
//	width		*= OGRE_SCALE_CORRECTION;
//	height		*= OGRE_SCALE_CORRECTION;
//	depth		*= OGRE_SCALE_CORRECTION;
//
//	// Set the size
//	setScale( width, height, depth );
//
//	// Store scale
//	m_width		= width;
//	m_height	= height;
//	m_depth		= depth;
//
//	// This is a primitive object type (PhysicsBox)
//	setType( BOX );
//
//	m_bIsValid = true;
//}
//
//
///**
// * @internal
// * @brief Creates a PhysicsBox with the same size in the three axis
// * @param[in] size Defines the size of the cube (the same in the three axis: width, height, depth)
// */
//void PhysicsBox::init( float size )
//{
//	return init( size, size, size );
//}
//
///**
// * @brief Creates the collision shape to control box's collisions and activates its physics
// *
// * @param staticObject	If true, this object will be static, this means that this object is affected by 
// * physics objects and forces of the scene (so it will collide with other physics object), but won't move.
// */
//void PhysicsBox::enablePhysics( bool staticObject /*= false*/ )
//{
//	if ( m_physicsEnabled )
//	{
//		LOG_ERROR_NTIMES( 1, "Trying to enable physics on a box that has physics already enabled" );
//		return;
//	}
//
//	// We need the pysics manger activated, so make sure
//	PhysicsManager::getSingleton().enable();
//
//	// Create collision shape for the object based on the triangle mesh
//	OgreBulletCollisions::CollisionShape* collisionShape = PhysicsManager::getSingleton().buildBoxShape( m_width, m_height, m_depth );
//
//	// Create rigid body
//	enableRigidBodyPhysics( collisionShape, staticObject );
//}
//
//} // namespace Cing