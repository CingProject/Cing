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

#ifndef _GraphicsPrereqs_H_
#define _GraphicsPrereqs_H_

/**
 * Prereqs File: This header file contains forward class declarations of the 
 * Graphics namespace  to speed up compiling.
 */

#include "GraphicsTypes.h"

// General includes
#include <cstdio>

/// Contains all communication with 3d graphics system (Ogre Library) and rendering management
namespace Graphics
{
  class GraphicsManager;
  class Window;
  class Camera3D;
  class Font;
  class Text;
}

// Ogre Forward declarations
namespace Ogre
{
  class Root;
  class SceneManager;
  class RenderWindow;
  class Camera;
  class Viewport;
  class TextAreaOverlayElement;
  class Entity;
}

// Physics (This shouldn't be here, but for the moment, as there is no Logic module that combines graphics and physics....)
namespace Physics
{
	class PhysicsObject;
}

#endif // _GraphicsPrereqs_H_
