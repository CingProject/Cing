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

#ifndef _PhysicsPlane_h_
#define _PhysicsPlane_h_

#include "PhysicsPrereqs.h"
#include "PhysicsObject.h"

namespace Physics
{

/**
 * @internal
 * @brief Represents a 3D Plane with physics capabilities. It can have an associated texture to draw it.
 */
	class PhysicsPlane: public PhysicsObject
{
public:

	// Constructor / Destructor
	PhysicsPlane();
	virtual ~PhysicsPlane();

	// Init / Release
	void	init 			( float width, float height );
	void	init 			( float size );

	// Query  Methods
	bool	isValid		() { return m_bIsValid; }
	float	getWidth	() { return m_width;		}
	float	getHeight	() { return m_height;		}

	// Physics control
	virtual void enablePhysics		( bool staticObject );

private:

	// Constant attributes
	static const std::string  DEFAULT_MESH;			///< Name of the default mesh used by this object
	static const std::string  DEFAULT_MATERIAL; ///< Name of the default material used by this object

	// Attributes
	float		m_width;			///< Width of the PhysicsPlane
	float		m_height;			///< Height of the PhysicsPlane
	bool		m_bIsValid;	  ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Physics

#endif // _PhysicsPlane_h_