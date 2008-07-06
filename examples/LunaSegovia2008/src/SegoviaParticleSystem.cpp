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
#include "common/CommonUtilsIncludes.h"
#include "PathStorage.h"

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
SegoviaParticleSystem::SegoviaParticleSystem( float x, float y, float z, int nLines, const std::list< PathStorage* >& paths )
{
	// config
	float xMargin = 0;
	float yMargin = 0;
	float zMargin = 100;
	float phaseMargin = PI;

	// Get distance to all available paths to select the closest path
	float minDistance = FLT_MAX;
	std::list< PathStorage* >::const_iterator minDistanceIt;		
	for ( std::list< PathStorage* >::const_iterator it = paths.begin(); it != paths.end(); ++it )
	{
		float distance = (*it)->getDistance( x, y, z );
		if ( distance < minDistance )
		{
			minDistance			= distance;
			minDistanceIt		= it;
		}
	}

	// Create lines assigning the selected path
	for ( int i = 0; i < nLines; i++ )
	{
		m_lines.push_back( new Ribbon(x + random(-xMargin, xMargin) , 
																	y + random(-yMargin, yMargin), 
																	z + random(-zMargin, zMargin),
																	random(-phaseMargin, phaseMargin),
																	400, 
																	3,
																	(*minDistanceIt)->getPath() ) );
	}
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
SegoviaParticleSystem::~SegoviaParticleSystem()
{
	//std::for_each( m_lines.begin(), m_lines.end(), Common::ReleaseArrayFunctor< Line* >() );
	Lines::iterator it = m_lines.begin();
	for (; it != m_lines.end(); ++it )
	{
		delete *it;
	}
	m_lines.clear();

}

/**
 * @internal 
 * @brief 
 *
 * @return False if the system is dead, true if it is still alive
 */
bool SegoviaParticleSystem::update()
{
	// Update all lines
	Lines::iterator it = m_lines.begin();
	while( it != m_lines.end() )
	{
		// Update the line and if it is not alive -> delete it
		bool lineAlive = (*it)->update();
		if ( !lineAlive )
		{
			delete *it;
			it = m_lines.erase( it );
		}
		else
			++it;
	}

	// Return false if all the lines are dead
	return !m_lines.empty();
}

/**
 * @internal 
 * @brief 
 *
 * @param
 */
void SegoviaParticleSystem::setPosition( float x, float y, float z )
{
	Lines::iterator it = m_lines.begin();
	for (; it != m_lines.end(); ++it )
	{
		(*it)->setPosition( x, y, z );
	}
}
