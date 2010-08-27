/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

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

#ifndef _PhysicsBox_h_
#define _PhysicsBox_h_

#include "PhysicsPrereqs.h"
#include "PhysicsObject.h"

namespace Cing
{

/**
 * @internal
 * @brief Represents a 3D Cube (a Box) with physics capabilities. It can have an associated texture to draw it.
 *
 * @note this object is equal to the 3d object box, but derives from PhysicsObject, instead of Object3D. Although
 * this is not an elegant solution, it makes the code simpler, and the API more clear for the user
 */
	class PhysicsBox: public PhysicsObject
{
public:

	// Constructor / Destructor
	PhysicsBox();
	virtual ~PhysicsBox();

	// Init / Release
	void	init 			( float width, float height, float depth );
	void	init 			( float size );

	// Query  Methods
	bool	isValid		() { return m_bIsValid; }
	float	getWidth	() { return m_width;		}
	float	getHeight	() { return m_height;		}
	float	getDepth	() { return m_depth;		}

	// Physics control
	virtual void enablePhysics		( bool staticObject = false );


private:

	// Constant attributes
	static const std::string  DEFAULT_MESH;			///< Name of the default mesh used by this object
	static const std::string  DEFAULT_MATERIAL; ///< Name of the default material used by this object

	// Attributes
	float		m_width;			///< Width of the PhysicsBox
	float		m_height;			///< Height of the PhysicsBox
	float		m_depth;			///< Depth of the PhysicsBox
	bool		m_bIsValid;	  ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _PhysicsBox_h_