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

#ifndef _Cing_GraphicsTypes_H_
#define _Cing_GraphicsTypes_H_

// Precompiled headers
#include "Cing-Precompiled.h"

/**
* @internal
* @file This file contains the common types of the Graphics namespace
*/

// Ogre includes
#include "OgreLight.h"
#include "OgrePixelFormat.h"

// Set to 1 to use the Ogre Native formats (as opposed to Byte formats)
//#define USE_OGRE_NATIVE_FORMATS 1

namespace Cing
{

	/// Image formats
	enum GraphicsType
	{
		// Not defined graphics type (can be used as a "non-set" kind of format)
		UNDEFINED = -1,

		// Image Formats

		// Using Ogre Native Endian Formats
#if (USE_OGRE_NATIVE_FORMATS == 1)
		RGB			= Ogre::PF_R8G8B8,		///< Red, Green and Blue
		COLOR 		= RGB,					///< Same as RGB
		RGBA		= Ogre::PF_R8G8B8A8,	///< Red, Green, Blue and Alpha
		BGR			= Ogre::PF_B8G8R8,		///< Blue, Green and Red
		BGRA		= Ogre::PF_B8G8R8A8,	///< Blue, Green, Red and Alpha
		GRAYSCALE	= Ogre::PF_BYTE_L,		///< Gray scale
#else
		// Using Ogre Byte formats
		RGB			= Ogre::PF_BYTE_RGB,	///< Red, Green and Blue
		COLOR 		= RGB,					///< Same as RGB
		RGBA		= Ogre::PF_BYTE_RGBA,	///< Red, Green, Blue and Alpha
		BGR			= Ogre::PF_BYTE_BGR,	///< Blue, Green and Red
		BGRA		= Ogre::PF_BYTE_BGRA,	///< Blue, Green, Red and Alpha
		GRAYSCALE	= Ogre::PF_BYTE_L,		///< Gray scale
#endif

		// Color Modes
		//RGB,
		HSB = 100, // The 100 is assignmed to avoid enum collisions with the Ogre defined values (to have clear values for debugging)
		// Drawing modes
		POINTS,
		LINES,
		TRIANGLES,
		TRIANGLE_FAN,
		TRIANGLE_STRIP,
		QUADS,
		QUAD_STRIP,
		// Other
		CLOSE,
	    // Coordinate systems
		OPENGL3D,
		PROCESSING,

		// Texture Usage Types. Any Ogre::TextureUsage should work http://www.ogre3d.org/docs/api/html/group__Resources.html#gaf140ec886884a864abc74a7556f8bf67
		// this are just the most used ones
		STATIC							= Ogre::TU_STATIC,							///< Fastest option, but altering the texture contents is very slow (use this if you don't plan to update the texture often)
		DYNAMIC							= Ogre::TU_DYNAMIC,							///< Texture will be frequently updated, and both read/write can be done.
		WRITE_ONLY						= Ogre::TU_WRITE_ONLY,						///< Use it if you plan on (not frequently) updating the texture without reading its content back
		DYNAMIC_WRITE_ONLY_DISCARDABLE	= Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE,	///< Texture can be updated frequently, but no reading of its content can be done.
		RENDERTARGET					= Ogre::TU_RENDERTARGET						///< The texture will be used as a render target (to render scenes on it)
	};

	/// Filter types
	enum ImageProcessingFilters
	{
		BLUR,		///< provides a basic blur.
		GRAY,		///< converts the image to grayscale based on luminance.
		INVERT,		///< will invert the color components in the image.
		//_OPAQUE,	///< set all the high bits in the image to opaque
		THRESHOLD,	///< converts the image to black and white.
		DILATE,		///< grow white/light areas
		ERODE,		///< shrink white/light areas
		FLIP_X,		///< flips the image horizontally (this flip is done by flipping the physical pixels, so it is slower than flipping the texture quad UV texture coordinates, or the vertex positions)
		FLIP_Y,		///< flips the image vertically (this flip is done by flipping the physical pixels, so it is slower than flipping the texture quad UV texture coordinates, or the vertex positions)
		FLIP_XY	///< flips the image horizontally and vertically (this flip is done by flipping the physical pixels, so it is slower than flipping the texture quad UV texture coordinates, or the vertex positions)
	};

	/// Ink types
	enum ImageInkModes
	{
		BLEND,			///< normal mode
		ADD				///< additive mode
	};
	/// Render modes
	enum RenderMode
	{
		DRAW_POINTS			= Ogre::PM_POINTS,
		DRAW_WIREFRAME		= Ogre::PM_WIREFRAME,
		DRAW_SOLID			= Ogre::PM_SOLID
	};

	/// 2d Drawing
	//enum DrawMode -> Defined in CommonConstants.h File
	//{
	//	CORNER,
	//	CORNERS,
	//	CENTER,
	//	RADIUS
	//};

	// Types of Graphic modes
	enum GraphicMode
	{
		OPENGL, ///< Open GL used to render
		DIRECTX ///< DirectX used to render (Only available in windows)
	};


	// Shadow techniques
	enum ShadowTechnique
	{
		// Basic fixed pipeline based techniques
		STENCIL_ADDITIVE	= Ogre::SHADOWTYPE_STENCIL_ADDITIVE,
		STENCIL_MODULATIVE	= Ogre::SHADOWTYPE_STENCIL_MODULATIVE,
		TEXTURE_ADDITIVE	= Ogre::SHADOWTYPE_TEXTURE_ADDITIVE,
		TEXTURE_MODULATIVE	= Ogre::SHADOWTYPE_TEXTURE_MODULATIVE
	};

	/// Lights
	typedef Ogre::Light       		Light;
	typedef Ogre::SceneNode       	SceneNode;



} // namespace Cing

#endif // _GraphicsTypes_H_
