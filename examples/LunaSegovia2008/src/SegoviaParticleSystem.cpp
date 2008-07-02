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
#include "common/CommonUtils.h"

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
SegoviaParticleSystem::SegoviaParticleSystem( float x, float y, float z, int nLines )
{
	// config
	float xMargin = 0;
	float yMargin = 0;
	float zMargin = 100;
	float phaseMargin = PI;

	for ( int i = 0; i < nLines; i++ )
	{
		m_lines.push_back( new Line(	x + random(-xMargin, xMargin) , 
																	y + random(-yMargin, yMargin), 
																	z + random(-zMargin, zMargin),
																	random(-phaseMargin, phaseMargin)) );
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
	bool alive = false;;

	// Update all lines
	Lines::iterator it = m_lines.begin();
	for (; it != m_lines.end(); ++it )
	{
		alive = (*it)->update() || alive;
	}

	return alive;
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
