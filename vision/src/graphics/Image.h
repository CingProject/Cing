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

#ifndef _Image_H_
#define _Image_H_

#include "GraphicsPrereqs.h"
#include "TexturedQuad.h"
#include "GraphicsManager.h"

// Ogre
#include "Ogre3d/include/OgreImage.h"

// Image processing
#include "ImageThresholdFilter.h"
#include "ImageDifferenceFilter.h"

namespace Graphics
{

	/**
	* @internal
	* @brief Stores an image which can be loaded from a file, or created dynamically.
	* Supported image formats are: .bmp, .jpg, .gif, .raw, .png, .tga and .dds.
	*/
	class Image
	{
	public:

		// Constructor / Destructor
		Image				();
		Image				( const Image& img );
		Image				( int width, int height, GraphicsType format = RGB );
		Image				( const std::string& name );
		~Image			();

		// Init / Release / Update / Save / Clone
		void		init  ( int width, int height, GraphicsType format = RGB );
		void		init  ( const Image& img );
		void		load  ( const std::string& name );
		void		save	( const std::string& name );
		void		end		();

		// Image data
		void		setData( char* imageData, int width, int height, GraphicsType format );
		char*		getData() { return isValid()? m_cvImage->imageData: NULL; }
		Image*	clone ();

		// Draw on scene
		void	draw	( float xPos, float yPos, float zPos );
		void	draw	( float xPos, float yPos, float zPos, float width, float height );
		void	draw	( float x1, float y1, float z1,	float x2, float y2, float z2,
			float x3, float y3, float z3,	float x4, float y4, float z4);
		void	draw2d( float xPos, float yPos );
		void	draw2d( float xPos, float yPos, float width, float height );

		void  drawBackground( float xPos, float yPos, float width, float height );

		// 2D Image drawing methods
		void  triangle	( float x1, float y1, float x2, float y2, float x3, float y3 );
		void  line			( float x1, float y1, float x2, float y2 );
		void  arc				( float x, float y,  float width, float height, float start, float stop );
		void  point			( float x, float y);
		void  quad			( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 );
		void  ellipse		( float x, float y, float x2, float y2);
		void  ellipse		( float x, float y, float x2, float y2, float angle);
		void  rect			( float x1, float y1, float x2, float y2 );
		void  text			( float x1, float y1, const char* text );
		void  fill          ( Graphics::Color theColor );

		// Image processing
		void	filter	( ImageProcessingFilters type );
		void	filter	( ImageProcessingFilters type, float param1 );
		void	toColor	();
		void	toGray	();

		// Ink modes
		void setInkMode( ImageInkModes type );

		// Updates texture data
		void updateTexture();

		// Query methods
		bool			isValid		() const	{ return m_bIsValid; }
		const IplImage&	getCVImage	() const	{ return *m_cvImage; }
		IplImage&		getCVImage	()			{ return *m_cvImage; }
		int				getWidth	() const;
		int				getHeight	() const;
		GraphicsType	getFormat	() const;
		int				getNChannels() const	{ return m_nChannels; }
		Color			getPixel	( int x, int y );
		Ogre::TexturePtr getOgreTexture() { return m_quad.getOgreTexture(); }

		// Operators and operations
		void operator =	( const Image& other );
		void operator = ( float scalar);
		void operator -=	( float scalar );
		void operator +=	( float scalar );
		void operator -=	( const Image& img );
		void operator +=	( const Image& img );
		void blend				( const Image& other, float percentage );

		// Other
		void copy( const Image& img );
		void copy( const Image& img , int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH);

		// Texture update control
		void					setUpdateTexture( bool updateTextureFlag );	
		bool					getUpdateTexture() const;	

		// Texture coordinate flip
		void					flipVertical		();
		bool					isVFlipped			() const { return m_bVFlip; }

		TexturedQuad	m_quad;						///< This is the quad (geometry) and texture necessary to be able to render the image
	private:
		// Attributes
		static ImageDifferenceFilter   m_imgDiffFilter;      ///< Filter to calculate the difference between two images
		static ImageThresholdFilter    m_imgThresholdFilter; ///< Image to apply thresholding (posterizing) of an image

		IplImage*		  m_cvImage;				///< Contains the image compatible with openCV
		Ogre::Image		m_image;					///< Contains the image data (loaded from file or dynamically created)
		int						m_nChannels;			///< Number of channels of the image
		bool					m_bIsValid;				///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.			
		bool					m_bVFlip;					///< True if the image texture coordinates are flipped vertically 
		bool					m_bUpdateTexture;	///< Indicates whether the texture will update to GPU or not.
	};

} // namespace Graphics

#endif // _Image_H_
