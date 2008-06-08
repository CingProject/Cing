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

#ifndef _Image_H_
#define _Image_H_

#include "GraphicsPrereqs.h"
#include "TexturedQuad.h"
#include "GraphicsManager.h"

// Ogre
#include "externLibs/Ogre3d/include/OgreImage.h"

// OpenCv
#include "externLibs/OpenCV/cxcore/include/cxtypes.h"
#include "externLibs/OpenCV/highgui/include/highgui.h"

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
	Image				( int width, int height, ImageFormat format = RGB );
	Image				( const std::string& name );
	~Image			();

	// Init / Release / Update / Save / Clone
	void		init  ( int width, int height, ImageFormat format = RGB );
	void		init  ( const Image& img );
	void		load  ( const std::string& name );
	void		save	( const std::string& name );
	void		end		();
	// TODO: Check this
	Image*	clone ();

	// Draw on scene
	void	draw	( int xPos, int yPos, int zPos = 0 );

	// 2D Image drawing methods
	void  triangle	( float x1, float y1, float x2, float y2, float x3, float y3 );
	void  line			( float x1, float y1, float x2, float y2 );
	void  arc				( float x, float y,  float width, float height, float start, float stop );
	void  point			( float x, float y);
	void  quad			( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	void  ellipse		( float x, float y, float width, float height);
	void  rect			( float x1, float y1, float x2, float y2 );
	
	// Updates texture data
	void updateTexture();

	// Query methods
	bool  isValid() const { return m_bIsValid; }

	// Operators and operations
	void	operator=	( const Image& other );

	// Getters and Setters
	int								getWidth();
	int								getHeight();
	ImageFormat				getFormat();

	void							setUpdateTexture( bool updateTextureFlag );	
	bool							getUpdateTexture() const;	

private:
	// Attributes
	IplImage*		  m_cvImage;				///< Contains the image compatible with openCV
	Ogre::Image		m_image;					///< Contains the image data (loaded from file or dynamically created)
	TexturedQuad	m_quad;						///< This is the quad (geometry) and texture necessary to be able to render the image
	bool					m_bIsValid;				///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.			
	bool					m_bUpdateTexture;	///< Indicates whether the texture will update to GPU or not.
};

} // namespace Graphics

#endif // _Image_H_
