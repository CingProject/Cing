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

#include "PathStorage.h"
#include "common/CommonUtils.h"

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
PathStorage::PathStorage( bool debug )
{
	reset();
	m_debug = debug;
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
PathStorage::~PathStorage()
{
	reset();
}

/**
 * @internal 
 * @brief 
 *
 * @param
 */
void PathStorage::reset()
{
	m_path.clear();
	//std::for_each( m_debugSpheres.begin(), m_debugSpheres.end(), Common::ReleaseFunctor< Sphere* >() );
	for ( std::vector< Sphere* >::iterator it = m_debugSpheres.begin(); it != m_debugSpheres.end(); ++it )
	{
		delete (*it);
	}
}

/**
 * @internal 
 * @brief 
 *
 * @param
 */
void PathStorage::addPoint( float x, float y, float z )
{
	// Add the point to the path
	m_path.push_back( Vector( x, y, z ) );

	// if debug is activated create a sphere too
	if ( m_debug )
	{
		Sphere* sphere = new Sphere();
		sphere->init( 10 );
		m_debugSpheres.push_back( sphere );
	}

}

/**
 * @brief Returns the distance from a point to the closest path node
 */
float PathStorage::getDistance( float x, float y, float z )
{
	std::vector< float > distances;

	Path::const_iterator it = m_path.begin();
	for (; it != m_path.end(); ++it )
		distances.push_back( (*it).distance( Vector( x, y, z ) ) );

	return *(std::min( distances.begin(), distances.end() ));
}
