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

#include "Line.h"
#include "Vision.h"

#include "externLibs/Ogre3d/include/OgreSceneNode.h"
#include "externLibs/Ogre3d/include/OgreParticleSystem.h"
#include "externLibs/Ogre3d/include/OgreParticle.h"
#include "externLibs/Ogre3d/include/OgreStringConverter.h"


int		Line::index = 0;


/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Line::Line( float x, float y, float z, float phase ):
	m_pSystem( NULL ),
	m_particleNode( NULL ),
	m_timeControl	( phase ),
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
	m_center.x = x;
	m_center.y = y;
	m_center.z = z;

	m_target.x = width/2.0f;
	m_target.y = height - 100;
	m_target.z = 0;
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Line::~Line()
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
bool Line::update()
{
	// Get current number of particles
	size_t nParticles = m_pSystem->getNumParticles();

	// If it had particles and has 0 now -> the system is dead
	if ( (m_prevNumParticles > 0 ) && (nParticles == 0) )
		return false;

	// Control vars
	m_timeControl			+= 0.01f * elapsedMillis;
	float followSpeed = 0.6f;
	float spiralWidth	= 60;

	float speed = 0.7f;
	speed *= elapsedMillis;

	//Vector speed( 0.0f, 0.5f, 0.0f );
	Vector dir( m_target - m_center );
	dir.normalise();

	// Calculate rotation of current direction (respect vertical y)
	float		angle = angleBetweenVectors( Vector::UNIT_Y, dir );
	Vector	axis  = (Vector::UNIT_Y.crossProduct( dir )).normalisedCopy();
	Ogre::Matrix3 m;
	m.FromAxisAngle( axis, Ogre::Radian( angle ) );

	// Update center
	dir				*= speed;
	m_center	+= dir;

	// Affect particles -> make all follow the first particle
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
			firstParticle->timeToLive = 10;
			Vector& pos = firstParticle->position;
			float xSpiral = cos( m_timeControl ) * spiralWidth;
			float ySpiral = sin( m_timeControl ) * spiralWidth;

			Vector spiral( xSpiral, 0, 0 );
			Vector spiralNorm = spiral.normalisedCopy();
			spiral = spiral * m;
			//pos.x =  xCenter + val;
			//pos.y += upSpeed;
			//pos += speed;
			

			pos.x = m_center.x + xSpiral;
			pos.y = m_center.y /*+ ySpiral*/;

			prevParticle = firstParticle;
		}
		// Rest
		else
		{
			particle = pit.getNext();
			Vector& pos = particle->position;
			//float val = cos( m_timeControl );
			//pos.y += upSpeed;
			//pos.x +=  val;
			pos.x = ((1.0f - followSpeed) * pos.x) + (followSpeed * prevParticle->position.x);
			pos.y = ((1.0f - followSpeed) * pos.y) + (followSpeed * prevParticle->position.y);

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
void Line::setPosition( float x, float y, float z )
{
	m_particleNode->setPosition( x, y, z );
}
