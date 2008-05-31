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

#include "PhysicsManager.h"

// Common
#include "common/Release.h"

// Ogre bullet
#include "externLibs/OgreBullet/Dynamics/include/OgreBulletDynamicsWorld.h"

namespace Physics
{


/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
PhysicsManager::PhysicsManager():
	m_physicsWorld				( NULL ),
	m_physicsDebugDrawer	( NULL ),
	m_bIsValid						( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
PhysicsManager::~PhysicsManager()
{
	// Release resources
	end();
}

/**
 * @internal
 * @brief Initializes the physics engine.
 *
 * @param sceneManager	General Scene manager
 * @param gravityVector	Gravity used in physics calculations
 * @param bounds				Bounds of the physics affected world
 */
void PhysicsManager::init( Ogre::SceneManager&	sceneManager, 
													 const Vector3&				gravityVector /*= Vector3(0,-9.81,0)*/,
													 const AABox&					bounds				/*= AABox (Vector3 (-10000, -10000, -10000), Vector3 (10000,  10000,  10000))*/ )
{
	// Start Bullet Physics engine
	m_physicsWorld = new OgreBulletDynamics::DynamicsWorld ( &sceneManager, bounds, gravityVector );

	// add Debug info display tool
	m_physicsDebugDrawer = new OgreBulletCollisions::DebugDrawer();

	// Set the active debug drawer for physics
	m_physicsWorld->setDebugDrawer( m_physicsDebugDrawer );

	// Attach it to the scene
	Ogre::SceneNode *node = sceneManager.getRootSceneNode()->createChildSceneNode( "physicsDebugDrawer", Vector3::ZERO );
	node->attachObject(static_cast < Ogre::SimpleRenderable *>( m_physicsDebugDrawer ));


	m_bIsValid = true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void PhysicsManager::end()
{
	// Release bullet related stuff
	Common::Release( m_physicsDebugDrawer );
	m_physicsWorld->setDebugDrawer( NULL );
	Common::Release( m_physicsWorld );

	m_bIsValid = false;
}

/**
 * @brief Updates the physics state
 *
 * @param elapsedSec Time elapsed since last frame in seconds
 */
void PhysicsManager::update( unsigned long elapsedSec )
{
	m_physicsWorld->stepSimulation( elapsedSec );
}

} // namespace Physics