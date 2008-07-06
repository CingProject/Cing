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

#ifndef _PathStorage_h_
#define _PathStorage_h_

#include <vector>
#include "Vision.h"
#include "common/CommonTypes.h"


/**
 * @internal
 * @brief 
 */
class PathStorage
{
public:
	
	typedef std::vector< Vector > Path;

	// Constructor / Destructor
	PathStorage( bool debug );
	~PathStorage();

	// Path creation / reset
	void 				reset			();
	void 				addPoint	( float x, float y, float z );
	const Path&	getPath		() const { return m_path; }

	// Queries
	float				getDistance( float x, float y, float z );

	// Debug
	void				setVisible	( bool visible );

	// Save / load
	friend std::ostream&	operator<<	( std::ostream& file, const PathStorage& path );
	friend std::ifstream& operator>>	( std::ifstream& file, PathStorage& path );

private:
	Path									m_path;
	std::vector< Sphere* > m_debugSpheres;
	bool									m_debug; ///< If debug is activated a sphere will be created per each point (to be able to draw it)
};

#endif // _PathStorage_h_