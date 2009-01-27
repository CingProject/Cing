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

#include "externlibs/ogre3d/include/OgreMatrix3.h"
#include "externlibs/ogre3d/include/OgreMatrix4.h"

#include "common/CommonTypes.h"

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
		Vector getPosition	() 
		{
			return Vector( m_4x4[0][3], m_4x4[1][3], m_4x4[2][3] );
		}

		Vector getRotation	()
		{
			Ogre::Quaternion rot = m_4x4.extractQuaternion();
			return Vector(	rot.getYaw().valueRadians(),
											rot.getPitch().valueRadians(),
											rot.getRoll().valueRadians() );
		}

		Vector getScale		()
		{
			//TODO: arreglar esto
/*
			Vector tempV;
			tempV = Vector( m_4x4[0][0], m_4x4[0][1], m_4x4[0][2] );
			float xScale = tempV.length();

			tempV = Vector( m_4x4[1][0], m_4x4[1][1], m_4x4[1][2] );
			float yScale = tempV.length();

			tempV = Vector( m_4x4[2][0], m_4x4[2][1], m_4x4[2][2] );
			float zScale = tempV.length();

			return Vector(xScale, yScale, zScale);*/

			return Vector( 1, 2, 1.0);
		}

		Vector applyTransform( Vector input )
		{
			return m_4x4*input;
		};

		// Set methods
		void		setPosition	( Vector& newPos );
		void		setRotation	( Vector& newRot );
		void		setScale		( Vector& newScale );

		void		setPosition	( float x, float y, float z );
		void		setRotation	( float x, float y, float z );
		void		setScale		( float x, float y, float z );
   
	private:

		// Euler notation
		Vector						m_position;
		Vector						m_rotation;
		Vector						m_scale;
		
		Ogre::Matrix4     m_4x4;

		bool							m_bIsValid;
	};

} // namespace Graphics

#endif // _Text_H_
