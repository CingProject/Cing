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

#ifndef _Image_H_
#define _Image_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "GraphicsPrereqs.h"
#include "TexturedQuad.h"
#include "Color.h"

// Ogre
#include "OgreImage.h"

// Image processing
#include "ImageThresholdFilter.h"
#include "ImageDifferenceFilter.h"

// Open CV
#include "OpenCV/cv.h"
#include "OpenCV/cxtypes.h"

namespace Cing
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
		Image				( const Image& img, Ogre::SceneManager* sm = NULL );
		Image				( int width, int height, GraphicsType format = RGB, Ogre::SceneManager* sm = NULL );
		Image				( unsigned char* data, int width, int height, GraphicsType format = RGB, Ogre::SceneManager* sm = NULL );
		Image				( const std::string& name, Ogre::SceneManager* sm = NULL );
		virtual ~Image		();

		// Init / Release / Update / Save / Clone
		void		init				( int width, int height, GraphicsType format = RGB, Ogre::SceneManager* sm = NULL );
		void		initAsRenderTarget	( int width, int height );
		void		init				( const Image& img, Ogre::SceneManager* sm = NULL );
		//bool		load				( const std::string& path );
		bool		load				( const std::string& path, Ogre::SceneManager* sm = NULL );
		void		save				( const std::string& path );
		void		end					();

		// Image data
		void					setData	( const unsigned char* imageData, int width = -1, int height = -1, GraphicsType format = UNDEFINED, int widthStep = -1 );
		unsigned char*			getData	();
		const unsigned char*	getData	() const;
		unsigned char*			pixels	() { return getData(); }
		const unsigned char*	pixels	() const { return getData(); }
		Image*					clone	();

		// Transformations
		void			setOrientation	( const Vector& axis, float angleRadians );
		void			setOrientation	( const Quaternion& orientation );
		void			rotate			( const Vector& axis, float angleRadians ); ///< Rotates around an axis specificed by the provided normalization vector, by an amount in radians provided by the angle specificed
		void			setScale		( float xScale, float yScale, float zScale );

		// Draw on scene
		void	draw	( float xPos, float yPos, float zPos );
		void	draw	( float xPos, float yPos, float zPos, float width, float height );
		void	draw	( float x1, float y1, float z1,	float x2, float y2, float z2,
						  float x3, float y3, float z3,	float x4, float y4, float z4);
		void	draw	( float xPos, float yPos );
		void	draw	( float xPos, float yPos, float width, float height );
		void	draw	( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 );

		void	drawBackground( float xPos, float yPos, float width, float height );

		// 2D Image drawing methods
		void  	triangle( int x1, int y1, int x2, int y2, int x3, int y3 );
		void  	line	( int x1, int y1, int x2, int y2 );
		void  	arc		( int x, int y,  int width, int height, float start, float stop );
		void  	point	( int x, int y);
		void  	quad	( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 );
		void  	ellipse	( int x, int y, int width, int height, float angleDegrees = 0 );
		void  	rect	( int x, int y, int width, int height );
		void  	text	( int x1, int y1, const char* text );
		void  	fill    ( const Color& color );
		void  	fill    ( float gray ) { fill( Color(gray, gray, gray) ); }
		void  	fill    ( float gray, float alpha ) { fill( Color(gray, gray, gray, alpha) ); }
		void  	fill    ( float r, float g, float b ) { fill( Color(r, g, b) ); }
		void  	fill    ( float r, float g, float b, float a ) { fill( Color(r, g, b, a) ); }

		// Image processing
		void	filter	( ImageProcessingFilters type );
		void	filter	( ImageProcessingFilters type, int param1 );
		void	toColor	();
		void	toGray	();

		// Ink modes / Transparency
		void setTransparency( float alpha );
		void setTransparency( int alpha ) { setTransparency( (float)alpha ); }
		void setInkMode		( ImageInkModes type );

		// Updates texture data
		void updateTexture();

		// Query methods
		bool				isValid			() const	{ return m_bIsValid; }
		IplImage			getCVImage		() const	{ return (IplImage)m_cvImage; }
		cv::Mat&			getCVMat		()			{ return m_cvImage; }
		const cv::Mat&		getCVMat		() const	{ return m_cvImage; }
		int					getWidth		() const;
		int					getHeight		() const;
		GraphicsType		getFormat		() const;
		int					getNChannels	() const	{ return m_nChannels; }
		Color				getPixel		( int x, int y ) const;
		Ogre::TexturePtr 	getOgreTexture	() { return m_quad.getOgreTexture(); }
		TexturedQuad&		getTexturedQuad	() { return m_quad; }
		const TexturedQuad&	getTexturedQuad	() const { return m_quad; }
		const std::string&	getPath			() const { return m_path; }

		// Operators and operations
		void operator =	( const Image& other );
		void operator = ( float scalar);
		void operator -=( float scalar );
		void operator +=( float scalar );
		void operator -=( const Image& img );
		void operator +=( const Image& img );
		bool operator ==( const Image& other) const;
		void blend		( const Image& other, float percentage );

		// Other
		void copy( const Image& img );

		// Texture update control
		void	setUpdateTexture( bool updateTextureFlag = true );	
		bool	getUpdateTexture() const;	

		// Texture coordinate flip
		void	flipVertical		();
		bool	isVFlipped			() const { return m_bVFlipped; }

		// Render Queue control: The higher the value (0..105) the later/more on top the quad is rendered
		void	forceRenderQueue	( unsigned int renderQueueId ) { m_quad.forceRenderQueue( renderQueueId ); }
		void	restoreRenderQueue	() { m_quad.restoreRenderQueue(); }

		TexturedQuad	m_quad;						///< This is the quad (geometry) and texture necessary to be able to render the image
	private:
		// Private methods
		bool	loadImageFromDisk	( const std::string& textureName, const std::string& texturePath );

		// Attributes
		static ImageDifferenceFilter   m_imgDiffFilter;      ///< Filter to calculate the difference between two images
		static ImageThresholdFilter    m_imgThresholdFilter; ///< Image to apply thresholding (posterizing) of an image

		std::string						m_path;				///< Path to the image file (relative to data folder)

		cv::Mat 						m_cvImage;			///< Contains the image compatible with openCV
		Ogre::Image						m_image;			///< Contains the image data (loaded from file or dynamically created)
		int								m_nChannels;		///< Number of channels of the image
		bool							m_bIsValid;			///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.			
		bool							m_bVFlipped;		///< True if the image texture coordinates are flipped vertically 
		bool							m_bUpdateTexture;	///< Indicates whether the texture will update to GPU or not.
		bool							m_loadedFromFile;	///< True if the image was loaded from file
		GraphicsType					m_format;			///< Format of the image
		GraphicsType					m_loadedFormat;		///< Format in which the image was loaded (if loaded from file)
	};

} // namespace Cing

#endif // _Image_H_
