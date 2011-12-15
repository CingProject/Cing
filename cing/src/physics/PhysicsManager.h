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
//#ifndef _PhysicsManager_h_
//#define _PhysicsManager_h_
//
//// Precompiled headers
//#include "Cing-Precompiled.h"
//
//#include "PhysicsPrereqs.h"
//
//// Graphics
//#include "graphics/Object3D.h"
//
//// Common
//#include "common/Singleton.h"
//
//namespace Cing
//{
//
//// Types of Physics collision shapes
//enum CollisionShape
//{
//	BOX,
//	TRIANGLE_MESH
//};
//
//// Physics constants (default values)
//const float				DEFAULT_STATIC_BODY_RESTITUTION		= 0.1f;
//const float				DEFAULT_STATIC_BODY_FRICTION		= 0.8f;
//
//const float				DEFAULT_DYNAMIC_BODY_RESTITUTION	= 0.6f;
//const float				DEFAULT_DYNAMIC_BODY_FRICTION		= 0.6f;
//const float				DEFAULT_DYNAMIC_BODY_MASS			= 1.0f;
//
//const int				MAX_SIMULATION_SUBSTEPS				= 3;
//const float				FIXED_TIME_STEP						= 1.0f / 60.f;
//
//const Vector			DEFAULT_GRAVITY						= Vector(0,-50.0f,0);
//
//
///**
// * @internal
// * @brief Manages all core code of the physics
// */
//class PhysicsManager: public SingletonStatic< PhysicsManager >
//{
//public:
//
//	// Required for singleton
//	friend class SingletonStatic< PhysicsManager >;
//
//	// Destructor
//	virtual ~PhysicsManager();
//
//	// Init / Release / Update
//	void	init		( Ogre::SceneManager& sceneManager, const Vector& gravityVector = DEFAULT_GRAVITY, const AABox &bounds = AABox (Vector (-10000, -10000, -10000), Vector (10000,  10000,  10000)));
//	void	end			();
//	void	update	( unsigned long elapsedMillis );
//
//
//	// Control methods
//	void	enable		( bool enable = true );
//	void	setGravity	( const Vector& gravity );
//
//	// Collision shape construction
//	OgreBulletCollisions::CollisionShape*		buildTriMeshShape			( Object3D& object );
//	OgreBulletCollisions::CollisionShape*		buildBoxShape				( float width, float heigh, float depth );
//	OgreBulletCollisions::CollisionShape*		buildStaticPlaneShape		( float width, float heigh );
//	OgreBulletCollisions::CollisionShape*		buildSphereShape			( float radius );
//
//	// Rigid bodies
//	OgreBulletDynamics::RigidBody*				createRigidBody				( Object3D& object, OgreBulletCollisions::CollisionShape* collisionShape, bool staticBody );
//
//	// Debug methods
//	void	drawPhysics( bool draw );
//
//	// Query  Methods
//	bool								isValid	()	{ return m_bIsValid; }
//	OgreBulletDynamics::DynamicsWorld*	getWorld()	{ return m_physicsWorld; }
//
//private:
//	// private constructor to ensure singleton
//	PhysicsManager();
//
//	// Static attributes
//	static long							m_rigidObjectCounter;///< Used to generate unique names for the rigid objects
//
//	// Attributes
//	bool								m_enabled;				///< Physics are enabled/disabled?
//	OgreBulletDynamics::DynamicsWorld*	m_physicsWorld;			///< Manager of the physics world. All physics communication is managed through this object
//	OgreBulletCollisions::DebugDrawer*	m_physicsDebugDrawer;	///< Contains all methods related to physics visual debugging (such as activate bbox rendering...etc).
//	bool								m_bIsValid;				///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
//
//};
//
//} // namespace Cing
//
//#endif // _PhysicsManager_h_
