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

#ifndef _CommonTypes_H_
#define _CommonTypes_H_

// Ogre
#include "externLibs/Ogre3d/include/OgreVector2.h"
#include "externLibs/Ogre3d/include/OgreVector3.h"
#include "externLibs/Ogre3d/include/OgreCommon.h"
#include "externLibs/Ogre3d/include/OgreAxisAlignedBox.h"

/**
 * @internal
 * @file This file contains common type definitions
 */
namespace Common
{
	// Math
	typedef Ogre::Vector2     		Point2d;
	typedef Ogre::Vector2     		Vector2d;

	typedef Ogre::Vector3     		Point;
	typedef Ogre::Vector3     		Vector;

	typedef Ogre::Vector4     		Point4d;
	typedef Ogre::Vector4     		Vector4d;

	typedef Ogre::Quaternion			Quaternion;

	// Spatial
	typedef Ogre::AxisAlignedBox	AABox;
	typedef Ogre::Box							BoxArea;

} // namespace Common


// To make more handy the use of the common data types
using namespace Common;

#endif // _CommonTypes_H_
