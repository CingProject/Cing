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

#include "Ribbon.h"
#include "Vision.h"

#include "externLibs/Ogre3d/include/OgreSceneNode.h"
#include "externLibs/Ogre3d/include/OgreRibbonTrail.h"
#include "externLibs/Ogre3d/include/OgreAnimation.h"
#include "externLibs/Ogre3d/include/OgreStringConverter.h"
#include "externLibs/Ogre3d/include/OgreBillboardSet.h"


int		Ribbon::index = 0;


/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
Ribbon::Ribbon( float x, float y, float z, float phase, float length, float duration, const std::vector< Vector >& path  ):
	m_ribbon( NULL ),
	m_ribbonNode( NULL ),
	m_ribbonAnimState( NULL ),
	m_animNode( NULL ),
	m_ribbonAnimation( NULL ),
	m_timeControl	( phase ),
	m_prevNumParticles( 0 )
{
	int currentIndex = index++;

	// Create Ribbon
	m_ribbon = ogreSceneManager->createRibbonTrail( "Ribbon" + Ogre::StringConverter::toString( currentIndex ) );
	m_ribbon->setMaterialName( "Examples/LightRibbonTrail" );
	m_ribbon->setTrailLength( length );

	// Create scene node for the ribbon (so it is visible) and attach it
	m_ribbonNode = ogreSceneManager->getRootSceneNode()->createChildSceneNode( "RibbonNode" + Ogre::StringConverter::toString( currentIndex ) );
	m_ribbonNode->attachObject( m_ribbon );

	// Create animation node for the ribbon
	m_animNode = ogreSceneManager->getRootSceneNode()->createChildSceneNode();
	m_animNode->setPosition( x, y, z );

	// Create animation for the ribbon
	//m_ribbonAnimation = ogreSceneManager->createAnimation( "RibbonAnimation" + Ogre::StringConverter::toString( currentIndex ), duration );
	//m_ribbonAnimation->setInterpolationMode(Ogre::Animation::IM_LINEAR);

	// Set key frames
	//Ogre::NodeAnimationTrack* track = m_ribbonAnimation->createNodeTrack( 1, m_animNode );
	//Ogre::TransformKeyFrame* kf = NULL;

	//float pathNodeStep = duration / (float)(path.size()-1);
	//float kfTime = 0;
	//for ( size_t i = 0; i < path.size(); i++ )
	//{
	//	// Create key frame and set position
	//	kf = track->createNodeKeyFrame( kfTime );
	//	kf->setTranslate( path[i] );
	//	
	//	// Increase time for next key frame
	//	kfTime += pathNodeStep;
	//}

	// Create animation state to control animation 
	//m_ribbonAnimState = ogreSceneManager->createAnimationState( m_ribbonAnimation->getName() );
	//m_ribbonAnimState->setEnabled( true );
	//m_ribbonAnimState->setLoop( false );

	// Set ribbon properties
	m_ribbon->setInitialColour(0, 1.0, 0.8, 0);
	m_ribbon->setColourChange(0, 0.5, 0.5, 0.5, 0.5);
	m_ribbon->setInitialWidth(0, 5);
	m_ribbon->addNode( m_animNode );

	// Add billboard to the head of the ribbon
	Ogre::BillboardSet* bbs = ogreSceneManager->createBillboardSet( "RibbonBillboard" + Ogre::StringConverter::toString( currentIndex ), 1 );
	bbs->setDefaultDimensions( 25.0f, 25.0f );
	bbs->createBillboard( Vector::ZERO, m_ribbon->getInitialColour(0) );
	bbs->setMaterialName( "Examples/Flare" );
	m_animNode->attachObject( bbs );

	// Store data
	m_duration = duration;
	std::copy( path.begin()+1, path.end(), std::back_inserter( m_path ) );

	// Set current target
	m_targetIndex = 0;

}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Ribbon::~Ribbon()
{
	// destroy stuff
	ogreSceneManager->destroyRibbonTrail( m_ribbon );
	ogreSceneManager->getRootSceneNode()->removeAndDestroyChild( m_ribbonNode->getName() );
	ogreSceneManager->getRootSceneNode()->removeAndDestroyChild( m_animNode->getName() );
}

/**
 * @internal 
 * @brief 
 *
 * @return False if the system is dead, true if it is still alive
 */
bool Ribbon::update()
{
	// TODO: Check if the system is dead

	// Get a copy of the current position
	Vector pos = m_animNode->getPosition();

	// Check if we have reached the current target
	float targetDistThreshold = 100;
	if ( pos.distance( m_path[m_targetIndex] ) < targetDistThreshold )
	{
		m_targetIndex++;
		if ( m_targetIndex == m_path.size() )
			return false;
	}

	// Move the ribbon

	// Control vars
	m_timeControl			+= 0.01f * elapsedSec;
	float spiralWidth	= 5;
	float speed				= 0.7f * elapsedSec;
	
	// Direction vector
	Vector dir( m_path[m_targetIndex] - pos );
	dir.normalise();
	dir *= speed;

	// Calculate rotation of current direction (respect vertical y)
	float		angle = angleBetweenVectors( Vector::UNIT_Y, dir );
	Vector	axis  = (Vector::UNIT_Y.crossProduct( dir )).normalisedCopy();
	Ogre::Matrix3 m;
	m.FromAxisAngle( axis, Ogre::Radian( angle ) );

	// Update position (center)
	pos += dir;

	// Calculate spiral coordinates (use just x coord for now)
	float xSpiral = cos( m_timeControl ) * spiralWidth;
	float ySpiral = sin( m_timeControl ) * spiralWidth; 

	// Rotate it to match current direction
	Vector spiral( xSpiral, 0, 0 );
	Vector spiralNorm = spiral.normalisedCopy();
	spiral = spiral * m;

	// Modify position
	pos.x = pos.x + xSpiral;
	pos.y = pos.y /*+ ySpiral*/;
	
	// Update scene node's position
	m_animNode->setPosition( pos );

	//// Get current number of particles
	//size_t nParticles = m_pSystem->getNumParticles();

	//// If it had particles and has 0 now -> the system is dead
	//if ( (m_prevNumParticles > 0 ) && (nParticles == 0) )
	//	return false;

	//// Control vars
	//m_timeControl			+= 0.01f * elapsedSec;
	//float followSpeed = 0.6f;
	//float spiralWidth	= 60;

	//float speed = 0.7f;
	//speed *= elapsedSec;

	////Vector speed( 0.0f, 0.5f, 0.0f );
	//Vector dir( m_target - m_center );
	//dir.normalise();

	//// Calculate rotation of current direction (respect vertical y)
	//float		angle = angleBetweenVectors( Vector::UNIT_Y, dir );
	//Vector	axis  = (Vector::UNIT_Y.crossProduct( dir )).normalisedCopy();
	//Ogre::Matrix3 m;
	//m.FromAxisAngle( axis, Ogre::Radian( angle ) );

	//// Update center
	//dir				*= speed;
	//m_center	+= dir;

	//// Affect particles -> make all follow the first particle
	//Ogre::ParticleIterator pit = m_pSystem->_getIterator();
	//Ogre::Particle* firstParticle;
	//Ogre::Particle* particle, *prevParticle;
	//int index = 0;
	//while (!pit.end())
	//{
	//	// First
	//	if ( index ==  0)
	//	{
	//		firstParticle = pit.getNext();
	//		firstParticle->timeToLive = 10;
	//		Vector& pos = firstParticle->position;
	//		float xSpiral = cos( m_timeControl ) * spiralWidth;
	//		float ySpiral = sin( m_timeControl ) * spiralWidth;

	//		Vector spiral( xSpiral, 0, 0 );
	//		Vector spiralNorm = spiral.normalisedCopy();
	//		spiral = spiral * m;
	//		//pos.x =  xCenter + val;
	//		//pos.y += upSpeed;
	//		//pos += speed;
	//		

	//		pos.x = m_center.x + xSpiral;
	//		pos.y = m_center.y /*+ ySpiral*/;

	//		prevParticle = firstParticle;
	//	}
	//	// Rest
	//	else
	//	{
	//		particle = pit.getNext();
	//		Vector& pos = particle->position;
	//		//float val = cos( m_timeControl );
	//		//pos.y += upSpeed;
	//		//pos.x +=  val;
	//		pos.x = ((1.0f - followSpeed) * pos.x) + (followSpeed * prevParticle->position.x);
	//		pos.y = ((1.0f - followSpeed) * pos.y) + (followSpeed * prevParticle->position.y);

	//		prevParticle = particle;
	//	}

	//	index++;
	//}

	// Store the number of particles
	//m_prevNumParticles = nParticles;

	return true;
}

/**
 * @internal 
 * @brief 
 *
 * @param
 */
void Ribbon::setPosition( float x, float y, float z )
{
	//m_particleNode->setPosition( x, y, z );
}
