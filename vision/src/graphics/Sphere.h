/*
  This source file is part of the Vision project
  For the latest info, see http://www.playthemagic.com/vision

Copyright (c) 2008 Julio Obelleiro and Jorge Cano

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

#ifndef _Sphere_h_
#define _Sphere_h_

#include "GraphicsPrereqs.h"
#include "Object3D.h"

namespace Graphics
{

/**
 * @internal
 * @brief Represents a 3D Sphere. It can have an associated texture to draw it.
 */
	class Sphere: public Object3D
{
public:

	// Constructor / Destructor
	Sphere();
	virtual ~Sphere();

	// Init / Release
	void	init 			( float diameter );

	// Query  Methods
	bool	isValid			() { return m_bIsValid; }
	float	getRadius		() { return m_radius;		}
	float	getDiameter	() { return m_radius * 2;		}

private:

	// Constant attributes
	static const std::string  DEFAULT_MESH;			///< Name of the default mesh used by this object
	static const std::string  DEFAULT_MATERIAL; ///< Name of the default material used by this object

	// Attributes
	float		m_radius;			///< Radius of the sphere
	bool		m_bIsValid;	  ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Graphics

#endif // _Sphere_h_
