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

#ifndef _Cing_CommonTypes_H_
#define _Cing_CommonTypes_H_

// Precompiled headers
#include "Cing-Precompiled.h"


// Ogre
#include "OgreVector2.h"
#include "OgreVector3.h"
#include "OgreCommon.h"
#include "OgreAxisAlignedBox.h"
#include "OgreMath.h"

/**
* @internal
* @file This file contains common type definitions
*/
namespace Cing
{
	// Math
	typedef Ogre::Vector2     		Point2;
	typedef Ogre::Vector2     		Vector2;
	typedef Vector2     			Vector2d;

	typedef Ogre::Vector3     		Point;
	typedef Ogre::Vector3     		Vector;
	typedef Vector	     			Vector3;
	typedef Vector3     			Vector3d;

	typedef Ogre::Vector4     		Point4;
	typedef Ogre::Vector4     		Vector4;
	typedef Vector4     			Vector4d;

	typedef Ogre::Quaternion		Quaternion;


	typedef Ogre::Matrix3			Matrix3;
	typedef Ogre::Matrix4			Matrix4;

	// 2D
	typedef Ogre::RealRect			Rect;

	// Spatial
	typedef Ogre::AxisAlignedBox	AABox;
	typedef Ogre::Box				BoxArea;

	// Timing
	typedef Ogre::Timer				Timer;

} // namespace Cing

#endif // _CommonTypes_H_
