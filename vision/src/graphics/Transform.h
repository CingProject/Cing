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
		void		scale			( float x, float y, float z );

		// Get methods
		Vector			getPosition	(); 
		Vector			getRotation	();
		Quaternion		getRotQuaternion();
		Vector			getScale	();

		// Set methods
		// Todo: Implementar
		void	setPosition	( Vector& newPos );
		void	setPosition	( float x, float y, float z );
		void	setRotation	( Vector& newRot );
		void	setRotation	( float x, float y, float z );
		void	setScale	( Vector& newScale );
		void	setScale	( float x, float y, float z );

		// Other
		Vector	applyTransform( Vector input );
		void	identity();
		void	printMatrix();
		void	applyMatrix( float m00, float m01, float m02, float m03,
							float m10, float m11, float m12, float m13,
							float m20, float m21, float m22, float m23,
							float m30, float m31, float m32, float m33 );
	private:
		// The transform data is stored here
		Ogre::Matrix4     m_4x4;
		bool							m_bIsValid;
	};

} // namespace Graphics

#endif // _Transform_H_
