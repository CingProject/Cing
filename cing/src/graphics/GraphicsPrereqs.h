/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

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

#ifndef _Cing_GraphicsPrereqs_H_
#define _Cing_GraphicsPrereqs_H_

// Precompiled headers
#include "Cing-Precompiled.h"

/**
 * Prereqs File: This header file contains forward class declarations of the
 * Graphics namespace  to speed up compiling.
 */

#include "GraphicsTypes.h"

/// Contains all communication with 3d graphics system (Ogre Library) and rendering management
namespace Cing
{
	class GraphicsManager;
	class Window;
	class Camera3D;
	class Font;
	class TextArea;
	class ImageResourceManager;
	class Image;
	class TexturedQuad;
	class Style;
	class ShapeManager;
	class Shape;
	class Color;
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
  class Node;
  class SceneNode;
}

// Physics (This shouldn't be here, but for the moment, as there is no Logic module that combines graphics and physics....)
namespace Cing
{
	class PhysicsObject;
}

// Open CV
struct _IplImage;

#endif // _GraphicsPrereqs_H_
