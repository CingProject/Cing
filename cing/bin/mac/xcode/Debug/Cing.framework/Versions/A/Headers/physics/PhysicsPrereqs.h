/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community 

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
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
