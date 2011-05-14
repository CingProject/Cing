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

#ifndef _PhysicsPrereqs_H_
#define _PhysicsPrereqs_H_

/**
 * Prereqs File: This header file contains forward class declarations of the 
 * Physics namespace  to speed up compiling.
 */

// Precompiled headers
#include "Cing-Precompiled.h"

//TODO: Review the need for this warning pragma
#if OGRE_COMPILER == OGRE_COMPILER_MSVC
	#pragma warning (disable : 4748 ) 
#endif

// General includes
#include <cstdio>

// Common
#include "common/CommonTypes.h"

/// Contains all communication with Physics system
namespace Cing
{
}

// Graphics
namespace Cing
{
	class Box;
	class Sphere;
	class Plane;
}

// Ogre
namespace Ogre
{
	class SceneManager;
}

// Ogre Bullet 
namespace OgreBulletDynamics
{
	class DynamicsWorld;
	class RigidBody;
}
namespace OgreBulletCollisions
{
	class DebugDrawer;
	class CollisionShape;
}

// Bullet
class btRigidBody;

#endif // _PhysicsPrereqs_H_
