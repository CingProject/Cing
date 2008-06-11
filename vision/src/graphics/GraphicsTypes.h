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

#ifndef _GraphicsTypes_H_
#define _GraphicsTypes_H_

/**
 * @internal
 * @file This file contains the common types of the Graphics namespace
 */

#include "Color.h"

// Ogre includes
#include "externLibs/Ogre3d/include/OgreLight.h"
#include "externLibs/Ogre3d/include/ogrepixelformat.h"


/// Image formats
enum ImageFormat
{
	RGB				= Ogre::PF_R8G8B8,	///< Red, Green and Blue
	COLOR 		= Ogre::PF_R8G8B8,	///< Same as RGB
	RGBA			= Ogre::PF_R8G8B8A8,///< Red, Green, Blue and Alpha
	GRAYSCALE	= Ogre::PF_BYTE_L		///< Gray scale
};

/// Filter types
enum ImageProcessingFilters
{
	BLUR,					///< provides a basic blur.
	GRAY,					///< converts the image to grayscale based on luminance. 
	INVERT,				///< will invert the color components in the image. 
	//_OPAQUE,		///< set all the high bits in the image to opaque 
	THRESHOLD,		///< converts the image to black and white. 
	DILATE,				///< grow white/light areas 
	ERODE				  ///< shrink white/light areas 
};

namespace Graphics
{

/// Render modes
enum RenderMode
{
	DRAW_POINTS				= Ogre::PM_POINTS,
	DRAW_WIREFRAME		= Ogre::PM_WIREFRAME,
	DRAW_SOLID				= Ogre::PM_SOLID
};

// Lights
typedef Ogre::Light       		Light;

} // namespace Graphics

#endif // _GraphicsTypes_H_
