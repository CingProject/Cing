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

#ifndef _Transform_H_
#define _Transform_H_

#include "GraphicsPrereqs.h"
#include "Common/CommonTypes.h"

namespace Graphics
{
	/**
	 * @internal
	 * Class to make easy transformation on 3D/2D Graphics
	 */
	class Transform
	{
	public:

		// Constructor / Destructor
		Transform();

		// Init / Release / Update
		bool		init   ();
		void		end    ();

		// Query methods
		bool		isValid			() const { return m_bIsValid; }

		// Simple transformations methods
		void		translate		( float x, float y, float z );
		void		rotate			( float x, float y, float z );
		void		scale				( float x, float y, float z );

		// Get methods
		Vector& getPosition	() { return m_position; }
		Vector& getRotation	() { return m_rotation; }
		Vector& getScale		() { return m_scale; }

		// Set methods
		void		setPosition	( Vector& newPos )  { m_position = newPos;   }
		void		setRotation	( Vector& newRot )  { m_rotation = newRot;   }
		void		setScale		( Vector& newScale ){ m_scale    = newScale; }

		void		setPosition	( float x, float y, float z ) { m_position = Vector(x,y,z); }
		void		setRotation	( float x, float y, float z ) { m_rotation = Vector(x,y,z); }
		void		setScale		( float x, float y, float z ) { m_scale    = Vector(x,y,z); }

	private:

		// Euler notation
		Vector						m_position;
		Vector						m_rotation;
		Vector						m_scale;

		bool							m_bIsValid;
	};

} // namespace Graphics

#endif // _Text_H_
