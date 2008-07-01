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

#include "SegoviaParticleSystem.h"
#include "Vision.h"

#include "externLibs/Ogre3d/include/OgreSceneNode.h"
#include "externLibs/Ogre3d/include/OgreParticleSystem.h"
#include "externLibs/Ogre3d/include/OgreParticle.h"
#include "externLibs/Ogre3d/include/OgreStringConverter.h"


int		SegoviaParticleSystem::index = 0;


/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
SegoviaParticleSystem::SegoviaParticleSystem( float x, float y, float z ):
	m_pSystem( NULL ),
	m_particleNode( NULL ),
	m_timeControl	( 0 ),
	m_prevNumParticles( 0 )
{
	// Create the particle system's node
	m_particleNode = ogreSceneManager->getRootSceneNode()->createChildSceneNode( "SevoviaParticleSystemNode" + Ogre::StringConverter::toString( index++ ) );

	// Create the particle system
	m_pSystem = ogreSceneManager->createParticleSystem( "SevoviaParticleSystem" + Ogre::StringConverter::toString( index++ ), "particleTest");

	// Attach it to the node
	m_particleNode->attachObject( m_pSystem );
	
	// Set pos
	m_particleNode->setPosition( x, y, z );

}

/**
 * @internal
 * @brief Destructor. Class release.
 */
SegoviaParticleSystem::~SegoviaParticleSystem()
{
	// destroy node
	ogreSceneManager->getRootSceneNode()->removeAndDestroyChild( m_particleNode->getName() );

	// destroy particle system
	ogreSceneManager->destroyParticleSystem( m_pSystem );
}

/**
 * @internal 
 * @brief 
 *
 * @return False if the system is dead, true if it is still alive
 */
bool SegoviaParticleSystem::update()
{
	// Control vars
	m_timeControl			+= 0.01f * elapsedSec;
	float upSpeed			= 0.5f * elapsedSec;
	float followSpeed = 0.3f;

	// Get current number of particles
	size_t nParticles = m_pSystem->getNumParticles();

	// If it had particles and has 0 now -> the system is dead
	if ( (m_prevNumParticles > 0 ) && (nParticles == 0) )
		return false;

	// Affect particles
	// OPTION 1: spiral
	//Ogre::ParticleIterator pit = m_pSystem->_getIterator();
	//while (!pit.end())
	//{
	//	Ogre::Particle* particle = pit.getNext();
	//	Vector& pos = particle->position;
	//	float val = cos( (particle->totalTimeToLive - particle->timeToLive)* rotSpeed );
	//	pos.x +=  val / 10.0f;
	//}

	// Affect particles
	// OPTION 2: follow the first particle
	Ogre::ParticleIterator pit = m_pSystem->_getIterator();
	Ogre::Particle* firstParticle;
	Ogre::Particle* particle, *prevParticle;
	int index = 0;
	while (!pit.end())
	{
		// First
		if ( index ==  0)
		{
			firstParticle = pit.getNext();
			//firstParticle->totalTimeToLive = 10;
			//firstParticle->timeToLive = 10;
			Vector& pos = firstParticle->position;
			float val = cos( m_timeControl );
			pos.x +=  val;
			pos.y += upSpeed;

			prevParticle = firstParticle;
		}
		// Rest
		else
		{
			particle = pit.getNext();
			Vector& pos = particle->position;
			float val = cos( m_timeControl );
			//pos.y += upSpeed;
			//pos.x +=  val;
			pos = ((1.0f - followSpeed) * pos) + (followSpeed * prevParticle->position);

			prevParticle = particle;
		}

		index++;
	}

	// Store the number of particles
	m_prevNumParticles = nParticles;

	return true;
}

/**
 * @internal 
 * @brief 
 *
 * @param
 */
void SegoviaParticleSystem::setPosition( float x, float y, float z )
{
	m_particleNode->setPosition( x, y, z );
}
