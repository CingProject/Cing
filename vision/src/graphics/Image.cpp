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
#include "Image.h"
#include "ImageResourceManager.h"

// Open CV
#include "externLibs/OpenCV/cv/include/cv.h"

// Common
#include "common/Exception.h"
#include "common/ResourceManager.h"
#include "common/LogManager.h"
#include "common/MathUtils.h"
#include "common/CommonConstants.h"

// Framework
#include "framework/Application.h"


namespace Graphics
{

// Static member init
ImageThresholdFilter	Image::m_imgThresholdFilter;
ImageDifferenceFilter	Image::m_imgDiffFilter;

/**
 * @brief Constructor. This constructor does not load or creates any image.
 */
Image::Image():
	m_cvImage(NULL),
  m_bIsValid( false ),
	m_bVFlip( false ),
	m_bUpdateTexture( false )
{
}

/**
 * @brief Creates an image with the same data as the received image
 *
 * @param
 */
Image::Image( const Image& img ):
	m_bIsValid( false ),
	m_cvImage(NULL),
	m_bUpdateTexture( false )
{
	init( img );
}

/**
 * @brief Creates an image of the specified size, but does not load it from file.
 * Therefore, the image created is empty. However, this image can be modified afterwards.
 *
 * @param width  Width of the image to be created
 * @param height Height of the image to be created
 * @param format Format of the image to be created. Possible formats are: RGB, ARGB, GRAYSCALE
 */
Image::Image( int width, int height, GraphicsType format /*= RGB*/ ):
	m_bIsValid( false ),
	m_cvImage(NULL),
	m_bUpdateTexture( false )
{
	init( width, height, format );
}

/**
 * @brief Creates an image from a file. The image loaded can be modified afterwards.
 * @note The image file specified should be placed on the data directory of the application
 *
 * @param name  Name of the file to be loaded. It must be placed in the data directory.
 */
Image::Image( const std::string& name ):
	m_bIsValid( false ),
	m_cvImage(NULL)
{
	load( name );
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
Image::~Image()
{
  // Release resources
  end();
}

/**
 * @brief Creates an image of the specified size, but does not load it from file.
 * Therefore, the image created is empty. However, this image can be modified afterwards.
 *
 * @param width  Width of the image to be created
 * @param height Height of the image to be created
 * @param format Format of the image to be created. Possible formats are: RGB, RGBA, GRAYSCALE
 */
void Image::init( int width, int height, GraphicsType format /*= RGB*/  )
{
  // Check if the class is already initialized
  if ( isValid() )
   THROW_EXCEPTION( "Image already initialized" );

  // Check application correctly initialized (could not be if the user didn't calle size function)
  Framework::Application::getSingleton().checkSubsystemsInit();

	// Create the empty IplImage image
	m_nChannels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );
	m_cvImage    = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U, m_nChannels);
		
	// Create the texture quad (to draw image)
	m_quad.init( m_cvImage->width, m_cvImage->height, format );

	// Load image data to texture -> No necessary as it is an empty image
	// updateTexture();

	// The class is now initialized
	m_bIsValid = true;
	m_bUpdateTexture = true;
}

/**
 * @brief Creates a copy of an image
 *
 * @param img Image to be copied
 */
void Image::init( const Image& img )
{
  // Check application correctly initialized (could not be if the user didn't calle size function)
  Framework::Application::getSingleton().checkSubsystemsInit();

	this->operator=( img );

	m_bIsValid = true;
	m_bUpdateTexture = false;
}

/**
 * @brief Creates an image from a file. The image loaded can be modified afterwards.
 * 
 * @note The image file specified should be placed on the data directory of the application
 *
 * @note Supported image formats are: .bmp, .jpg, .gif, .raw, .png, .tga and .dds.
 *
 * @param name  Name of the file to be loaded. It must be placed in the data directory.
 */
void Image::load( const std::string& name  )
{
  // Check application correctly initialized (could not be if the user didn't calle size function)
  Framework::Application::getSingleton().checkSubsystemsInit();


	// Load file from disk
	m_image.load( name, Common::ResourceManager::userResourcesGroupName );
	
	//Copy pixels from the OgreImage to the IplImage	
	if ( m_cvImage != NULL ) 
		cvReleaseImage( &m_cvImage );

	m_nChannels = (int)Ogre::PixelUtil::getNumElemBytes( m_image.getFormat() );
	m_cvImage = cvCreateImage(cvSize((int)m_image.getWidth(),(int)m_image.getHeight()),IPL_DEPTH_8U,m_nChannels);
	m_cvImage->imageData = (char *)m_image.getData();

	// Create the texture quad (to draw image)
	m_quad.init( (int)m_image.getWidth(), (int)m_image.getHeight(), (GraphicsType)m_image.getFormat() );
  
	// Load image data to texture
	updateTexture();

	// The class is now initialized
	m_bIsValid = true;
	m_bUpdateTexture = false;
}

/**
 * @brief Saves the image to a file
 *
 * @note The type of image file that will be created depends on the extension passed
 * For example, if you call myImage.save( "myFile.jpg" ), the image will be saved to a jpeg file
 * with the name myFile.jpg
 *
 * @note The file will be placed in the application data folder
 *
 * @note Supported image formats are: .bmp, .jpg, .gif, .raw, .png, .tga and .dds.
 * @param name Name that will have the image file
 */
void Image::save( const std::string& name )
{
	// TODO: Pass data from IplImage to m_image to save the data
	m_image.loadDynamicImage( (Ogre::uchar*)m_cvImage->imageData, m_cvImage->width, m_cvImage	->height, 1, (Ogre::PixelFormat)getFormat() );

	// Add the user app data folder to the name
	m_image.save( Common::ResourceManager::userDataPath + name );
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void Image::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

	m_quad.end();

	//Release IplImage
	// TODO check why this crashes.. 
	//cvReleaseImage(&m_cvImage);
	m_cvImage = NULL;

	// The class is not valid anymore
	m_bIsValid = false;

}

/**
 * @brief Sets the data of the image 
 *
 * @param imageData Data to set to the image
 * @param width			Width of the passed image data
 * @param height		Height of the passed image data
 * @param format		format Format of the image passed
 */
void Image::setData( char* imageData, int width, int height, GraphicsType format )
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to set data to an invalid image (it has not been initialized)" );
		return;
	}

	// Check dimensions
	int channels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );
	if ( (width != m_cvImage->width) || (height != m_cvImage->height) || (channels != m_cvImage->nChannels) )
	{
		LOG_ERROR( "Trying to set data with a wrong size of number of channels" );
		return;
	}

	// Set the data
	cvSetData( m_cvImage, imageData, m_cvImage->widthStep );

	// Make the image to be updated to texture in the next draw
	m_bUpdateTexture = true;
}


/**
 * @brief Returns image width 
 * @return the height of the image
 */
int Image::getWidth() const
{
  return m_cvImage? (int)m_cvImage->width: 0;
}

/**
 * @brief Returns image height
 * @return the height of the image
 */
int Image::getHeight() const
{
  return m_cvImage? (int)m_cvImage->height: 0;
}

/**
 * @brief Returns image format 
 * @return the imageFomat of the image
 */
GraphicsType Image::getFormat() const
{
	if ( m_image.getFormat() != Ogre::PF_UNKNOWN )
		return (GraphicsType)m_image.getFormat();

	switch(m_nChannels)
	{
	case 1: return (GraphicsType)GRAYSCALE;
		break;
	case 3: return (GraphicsType)RGB;
		break;
	case 4: return (GraphicsType)RGBA;
		break;
	default: THROW_EXCEPTION( "Error in GraphicsType" );
	}
}

/**
 * @brief Method to get the color of a pixel at x-y position in this image.
 * TODO: too slow!
 * @param x x coordinate of the pixel that wants to be retrieved
 * @param y y coordinate of the pixel that wants to be retrieved
 */
Color Image::getPixel( int x, int y )
{

	// Check the image is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	// Check boundaries
	if ( x < 0 )								x = 0;
	if ( x > m_cvImage->width )	x = m_cvImage->width - 1;
	if ( y < 0 )								y = 0;
	if ( y > m_cvImage->height) y = m_cvImage->height -1 ;

	// Read color
	int		channels = m_cvImage->nChannels;
	char* pixelPtr = m_cvImage->imageData + m_cvImage->widthStep * y;
	unsigned char blue, red, green, alpha = 0;

	switch( channels )
	{
	case 1:
		blue  = (abs)((int)pixelPtr[ x*channels + 0 ]);
		green = blue;
		red   = blue;
		alpha = 255;
		break;
	case 2:
		THROW_EXCEPTION( "Invalid number of channels in image" );
		break;
	case 3:
		blue  = ((int)pixelPtr[ x*channels + 0 ]);
		green = ((int)pixelPtr[ x*channels + 1 ]);
		red   = ((int)pixelPtr[ x*channels + 2 ]);
		alpha = 255;
		break;  
	case 4:
		blue  = (abs)((int)pixelPtr[ x*channels + 0 ]);
		green = (abs)((int)pixelPtr[ x*channels + 1 ]);
		red   = (abs)((int)pixelPtr[ x*channels + 2 ]);
		alpha = (abs)((int)pixelPtr[ x*channels + 3 ]);
		break;
	default:
		THROW_EXCEPTION( "Invalid number of channels in image" )
		break;
	}

	// Return read value
	return Color( red, green, blue, alpha );
}

/**
 * @brief Get texture update state
 * @return the m_bUpdateTexture attribute
 */
bool Image::getUpdateTexture() const
{
		return m_bUpdateTexture;
}

/**
 * @brief Flips the texture coordinates vertically
 */
void Image::flipVertical()
{

	//cvFlip(m_cvImage, NULL, 0);
	//m_quad.flipVertical();

	// Make the image to be updated to texture in the next draw
	//m_bUpdateTexture = true;
	// Load image data to texture
	//updateTexture();

	m_bVFlip = true;
}

/**
 * @brief Set if texture updates automatically every frame
 */
void	Image::setUpdateTexture(bool updateTextureFlag ) 
{
	m_bUpdateTexture = updateTextureFlag;
}

/**
 * @brief Draws the image in a specific position
 *
 * @param xPos x coordinate where the image should be drawn
 * @param yPos y coordinate where the image should be drawn
 * @param zPos z coordinate where the image should be drawn. If not passed, z will be set to zero
 */
void Image::draw( float xPos, float yPos, float zPos )
{
	// check if texture needs to be updated
	if (m_bUpdateTexture)
	{
		updateTexture();
		m_bUpdateTexture = false;
	}

	m_quad.draw( xPos, yPos, zPos );
}

/**
 * @brief Draws the image in a specific position
 *
 * @param xPos x coordinate where the image should be drawn
 * @param yPos y coordinate where the image should be drawn
 * @param zPos z coordinate where the image should be drawn. If not passed, z will be set to zero
 * @param width		Width of the quad that will be rendered
 * @param height	Height of the quad that will be rendered
 */
void Image::draw( float xPos, float yPos, float zPos, float width, float height )
{
	// check if texture needs to be updated
	if (m_bUpdateTexture)
	{
		updateTexture();
		m_bUpdateTexture = false;
	}

	m_quad.draw( xPos, yPos, zPos, width, height );
}

/**
 * @brief Draws the image in a specific quad
 *
 * @param xPos x1 
 * @param yPos y1 
 * @param zPos z1 
 * @param xPos x2 
 * @param yPos y2 
 * @param zPos z2 
 * @param xPos x3 
 * @param yPos y3 
 * @param zPos z3 
 * @param xPos x4 
 * @param yPos y4 
 * @param zPos z4 
 */
void Image::draw( float x1, float y1, float z1,
								  float x2, float y2, float z2,
									float x3, float y3, float z3,
									float x4, float y4, float z4)
{

	// check if texture needs to be updated
	if (m_bUpdateTexture)
	{
		updateTexture();
		m_bUpdateTexture = false;
	}

	m_quad.draw( x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4);
}

/**
 * @internal 
 * @brief Draws the image in 2d -> screen coordinates
 *
 * @param xPos x coordinate where the image should be drawn
 * @param yPos y coordinate where the image should be drawn
 */
void Image::draw2d( float xPos, float yPos )
{

	// check if image needs to be mirrored
	if (m_bVFlip)
	{
		cvFlip(m_cvImage, NULL, 0);
	}

	// check if texture needs to be updated
	if (m_bUpdateTexture)
	{
		updateTexture();
		m_bUpdateTexture = false;
	}

	m_quad.draw2d( xPos, yPos );
}

/**
 * @internal 
 * @brief Draws the image in 2d -> screen coordinates
 *
 * @param xPos x coordinate where the image should be drawn
 * @param yPos y coordinate where the image should be drawn
 * @param width		Width of the image that will be rendered <b>in screen coordinates</b>
 * @param height	Height of the image that will be rendered <b>in screen coordinates</b>
 */
void Image::draw2d( float xPos, float yPos, float width, float height )
{
	// check if image needs to be mirrored
	if (m_bVFlip)
	{
		cvFlip(m_cvImage, NULL, 0);
	}

	// check if texture needs to be updated
	if (m_bUpdateTexture)
	{
		updateTexture();
		m_bUpdateTexture = false;
	}

	m_quad.draw2d( xPos, yPos, width, height );
}

void Image::drawBackground( float xPos, float yPos, float width, float height )
	{
		// check if texture needs to be updated
		if (m_bUpdateTexture)
		{
			updateTexture();
			m_bUpdateTexture = false;
		}

		m_quad.drawBackground (xPos, yPos, width, height );
}

/**
 * @brief Copies the received image into the current image
 *
 * @param other Image to copy
 */
void Image::operator=( const Image& other )
{
	// Check the other image is valid
	if ( !other.isValid() )
		THROW_EXCEPTION( "Trying to copy an invalid image" );

  // Check application correctly initialized (could not be if the user didn't calle size function)
  Framework::Application::getSingleton().checkSubsystemsInit();


	// TODO: Check speed of cvCloneImage, maybe is faster to use memcpy?
	// and solve a bug when the loaded image has different size.
	if ( m_cvImage )
		cvReleaseImage( &m_cvImage );

	// Check if the image is initialized
	if ( !isValid() )
		init( other.getWidth(), other.getHeight(), other.getFormat() );

	// Copy the data
	m_cvImage		= cvCloneImage(other.m_cvImage);
	m_quad			= other.m_quad;
	m_nChannels = other.m_nChannels;

	// Load image data to texture
	updateTexture();

	// Now this image is valid
	m_bIsValid = true;
}

/**
 * @brief  Duplicate an image, returns new Image object.
 *
 * @param other Image to copy
 */
Image* Image::clone()
{
	// Check the other image is valid
	if ( !isValid() )	
		THROW_EXCEPTION( "Trying to copy an invalid image" );

	return new Image( *this );
}

/**
 * @brief  Load image data to texture
 */
void Image::updateTexture()
{
	m_quad.updateTexture(	m_cvImage->imageData,
												m_cvImage->width,
												m_cvImage->height,
												getFormat());
}

/**
 * @brief Draws a triangle inside an image
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param x2 x, second point
 * @param y2 y, second point
 * @param x3 x, third point
 * @param y3 y, third point
 */
void Image::triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
{
	// Check the image is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	GraphicsManager& graphManager = GraphicsManager::getSingleton();

	if (graphManager.getFill())
	{
		// Get Fill Color
		Color color        = graphManager.getFillColor();

		CvPoint* pts = new CvPoint[3];
		pts[0] = cvPoint(x1,y1);
		pts[1] = cvPoint(x2,y2);
		pts[2] = cvPoint(x3,y3);

		if (graphManager.getSmooth())
			cvFillConvexPoly( m_cvImage , pts, 3,	CV_RGB(color.r,color.g,color.b), 16, 0 );
		else
			cvFillConvexPoly( m_cvImage , pts, 3,	CV_RGB(color.r,color.g,color.b), 4, 0 );

		delete []pts;
	}

	if (graphManager.getStroke())
	{
		// Get Stroke Color
		Color color        = graphManager.getStrokeColor();
		int   strokeWeight = graphManager.getStrokeWeight();

		if (graphManager.getSmooth())
		{
			cvLine( m_cvImage,cvPoint(x1,y1),cvPoint(x2,y2),CV_RGB(color.r,color.g,color.b),
				strokeWeight,16,0); 
			cvLine( m_cvImage,cvPoint(x2,y2),cvPoint(x3,y3),CV_RGB(color.r,color.g,color.b),
				strokeWeight,16,0);
			cvLine( m_cvImage,cvPoint(x3,y3),cvPoint(x1,y1),CV_RGB(color.r,color.g,color.b),
				strokeWeight,16,0);
		}else{
			cvLine( m_cvImage,cvPoint(x1,y1),cvPoint(x2,y2),CV_RGB(color.r,color.g,color.b),
				strokeWeight,4,0); 
			cvLine( m_cvImage,cvPoint(x2,y2),cvPoint(x3,y3),CV_RGB(color.r,color.g,color.b),
				strokeWeight,4,0);
			cvLine( m_cvImage,cvPoint(x3,y3),cvPoint(x1,y1),CV_RGB(color.r,color.g,color.b),
				strokeWeight,4,0);
		}
	}

	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

/**
 * @brief Draws a line inside an image
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param x2 x, end point
 * @param y2 y, end point
 */
void Image::line( float x1, float y1, float x2, float y2 )
{
	// Check the image is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	GraphicsManager& graphManager = GraphicsManager::getSingleton();
	// Get Stroke and Fill Color
	Color color        = graphManager.getStrokeColor();
	int   strokeWeight = graphManager.getStrokeWeight();
	if (graphManager.getSmooth())
	{
		cvLine(	m_cvImage,
						cvPoint(x1,y1),
						cvPoint(x2,y2),
						CV_RGB(color.r,color.g,color.b),
						strokeWeight,	///< Thickness.
						16,	///< Type of the ellipse boundary, see cvLine description.
						0); ///< Number of fractional bits in the center coordinates and axes' values.
	}else{
		cvLine(	m_cvImage,
						cvPoint(x1,y1),
						cvPoint(x2,y2),
						CV_RGB(color.r,color.g,color.b),
						strokeWeight,	///< Thickness.
						4,	///< Type of the ellipse boundary, see cvLine description.
						0); ///< Number of fractional bits in the center coordinates and axes' values.
	}
	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

/**
 * @brief Draws an arc inside an image. Arcs are drawn along the outer edge of an ellipse defined by the x, y,
 *				 width and height parameters. The start and stop parameters specify the angles at which to draw the arc.
 *
 * @param x x, first point
 * @param y y, first point
 * @param width  width
 * @param height height
 */
void Image::arc( float x, float y, float width, float height, float start, float end )
{
	// Check the image is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	GraphicsManager& graphManager = GraphicsManager::getSingleton();

	if (graphManager.getFill())
	{
		// Get Fill Color
		Color color        = graphManager.getFillColor();
		if (graphManager.getSmooth())
		{
			cvEllipse(	m_cvImage,							///-> Image.
				cvPoint(x,y),						///-> Center of the ellipse.
				cvSize(width,height),		///-> Length of the ellipse axes. 
				0,											///->	Rotation angle.
				start,											///-> Starting angle of the elliptic arc.
				end,										///-> Ending angle of the elliptic arc.
				CV_RGB(color.r,color.g,color.b),///-> Ellipse color.
				-1,
				16);										///-> Thickness of the ellipse arc.
		}else{
			cvEllipse(	m_cvImage,							///-> Image.
				cvPoint(x,y),						///-> Center of the ellipse.
				cvSize(width,height),		///-> Length of the ellipse axes. 
				0,											///->	Rotation angle.
				start,											///-> Starting angle of the elliptic arc.
				end,										///-> Ending angle of the elliptic arc.
				CV_RGB(color.r,color.g,color.b),///-> Ellipse color.
				-1,
				4);										///-> Thickness of the ellipse arc.
		}
	}

	if (graphManager.getStroke())
	{
		// Get Stroke Color
		// Get Fill Color
		Color color        = graphManager.getStrokeColor();
		int   strokeWeight = graphManager.getStrokeWeight();	

		if (graphManager.getSmooth())
		{
			cvEllipse(	m_cvImage,							///-> Image.
									cvPoint(x,y),						///-> Center of the ellipse.
									cvSize(width,height),		///-> Length of the ellipse axes. 
									0,											///->	Rotation angle.
									start,											///-> Starting angle of the elliptic arc.
									end,										///-> Ending angle of the elliptic arc.
									CV_RGB(color.r,color.g,color.b),///-> Ellipse color.
									strokeWeight,
									16
									);										///-> Thickness of the ellipse arc.
		}else{
			cvEllipse(	m_cvImage,							///-> Image.
									cvPoint(x,y),						///-> Center of the ellipse.
									cvSize(width,height),		///-> Length of the ellipse axes. 
									0,											///->	Rotation angle.
									start,											///-> Starting angle of the elliptic arc.
									end,										///-> Ending angle of the elliptic arc.
									CV_RGB(color.r,color.g,color.b),///-> Ellipse color.
									strokeWeight,
									4 );					///-> Thickness of the ellipse arc.

		}
	}


	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}
/**
 * @brief Draws a point ( one pixel )
 *
 * @param x x, point
 * @param y y, point
 */
void Image::point( float x, float y )
{

	// Check the image is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	GraphicsManager& graphManager = GraphicsManager::getSingleton();

	// Get Stroke and Fill Color
	//Color color        = graphManager.getStrokeColor();

	GraphicsType theColorMode = Color::getColorMode();
	Color color;
	switch( theColorMode )
	{
	case RGB:
		color        = graphManager.getStrokeColor();
		break;
	case HSB:
		color        = graphManager.getStrokeColor();
		break;
	default:
		break;
	}

	int   strokeWeight = graphManager.getStrokeWeight();
	
	// Draw a pixel
	cvRectangle( m_cvImage,
							 cvPoint(x,y),
							 cvPoint(x,y),
							 CV_RGB(color.r,color.g,color.b),
							 strokeWeight);		///-> Thickness.

	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

/**
 * @brief Draws a quad, defined by four points
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param x2 x, second point
 * @param y2 y, second point
 * @param x3 x, third point
 * @param y3 y, third point
 * @param x4 x, fourth point
 * @param y4 y, fourth point
 */
void Image::quad( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 )
{
	// Check the image is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	GraphicsManager& graphManager = GraphicsManager::getSingleton();

	if (graphManager.getFill())
	{
		// Get Fill Color
		Color color        = graphManager.getFillColor();

		CvPoint* pts = new CvPoint[4];
		pts[0] = cvPoint(x1,y1);
		pts[1] = cvPoint(x2,y2);
		pts[2] = cvPoint(x3,y3);
		pts[3] = cvPoint(x4,y4);

		if (graphManager.getSmooth())
			cvFillConvexPoly( m_cvImage , pts, 4,	CV_RGB(color.r,color.g,color.b), 16, 0 );
		else
			cvFillConvexPoly( m_cvImage , pts, 4,	CV_RGB(color.r,color.g,color.b), 4, 0 );

		delete []pts;
	}

	if (graphManager.getStroke())
	{
		// Get Stroke Color
		// Get Fill Color
		Color color        = graphManager.getStrokeColor();
		int   strokeWeight = graphManager.getStrokeWeight();	

		if (graphManager.getSmooth())
		{

			cvLine( m_cvImage,cvPoint(x1,y1),cvPoint(x2,y2),CV_RGB(color.r,color.g,color.b),
				strokeWeight,16,0); 
			cvLine( m_cvImage,cvPoint(x2,y2),cvPoint(x3,y3),CV_RGB(color.r,color.g,color.b),
				strokeWeight,16,0);
			cvLine( m_cvImage,cvPoint(x3,y3),cvPoint(x4,y4),CV_RGB(color.r,color.g,color.b),
				strokeWeight,16,0);
			cvLine( m_cvImage,cvPoint(x4,y4),cvPoint(x1,y1),CV_RGB(color.r,color.g,color.b),
				strokeWeight,16,0);

		}else{

			cvLine( m_cvImage,cvPoint(x1,y1),cvPoint(x2,y2),CV_RGB(color.r,color.g,color.b),
				strokeWeight,4,0); 
			cvLine( m_cvImage,cvPoint(x2,y2),cvPoint(x3,y3),CV_RGB(color.r,color.g,color.b),
				strokeWeight,4,0);
			cvLine( m_cvImage,cvPoint(x3,y3),cvPoint(x4,y4),CV_RGB(color.r,color.g,color.b),
				strokeWeight,4,0);
			cvLine( m_cvImage,cvPoint(x4,y4),cvPoint(x1,y1),CV_RGB(color.r,color.g,color.b),
				strokeWeight,4,0);
		}
	}

	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}
/**
 * @brief Draws a text inside an image
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param text  string
 */
void Image::text( float x1, float y1,  const char* text )
{
	// Check the image is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	GraphicsManager& graphManager = GraphicsManager::getSingleton();
	// Get Stroke and Fill Color
	Color color        = graphManager.getStrokeColor();
	int   strokeWeight = graphManager.getStrokeWeight();

	cvPutText(m_cvImage, text, cvPoint(x1,y1), &(graphManager.getCvFont()), CV_RGB(color.r,color.g,color.b));

	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

/**
 * @brief Draws a rectangle inside an image
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param x2 x, end point
 * @param y2 y, end point
 */
void Image::rect( float x1, float y1, float x2, float y2 )
{
	// Check the image is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	GraphicsManager& graphManager = GraphicsManager::getSingleton();

	float widthDIV2 = x2/2;
	float heightDIV2 = y2/2;

	if (graphManager.getFill())
	{
		// Get Fill Color
		Color color        = graphManager.getFillColor();

		switch( graphManager.getRectMode() )
		{

		case CORNER: 
			if (graphManager.getSmooth())
				cvRectangle( m_cvImage, cvPoint(x1,y1), cvPoint(x1+x2,y1+y2), CV_RGB(color.r,color.g,color.b), -1, 16);
			else
				cvRectangle( m_cvImage, cvPoint(x1,y1), cvPoint(x1+x2,y1+y2), CV_RGB(color.r,color.g,color.b), -1, 4);
			break;

		case CORNERS: 
			if (graphManager.getSmooth())
				cvRectangle( m_cvImage, cvPoint(x1,y1), cvPoint(x2,y2), CV_RGB(color.r,color.g,color.b), -1, 16);
			else
				cvRectangle( m_cvImage, cvPoint(x1,y1), cvPoint(x2,y2), CV_RGB(color.r,color.g,color.b), -1, 4);
			break;

		case CENTER:
			if (graphManager.getSmooth())
				cvRectangle( m_cvImage, cvPoint(x1-widthDIV2,y1-heightDIV2), cvPoint(x1+widthDIV2,y1+heightDIV2), CV_RGB(color.r,color.g,color.b), -1, 16);
			else
				cvRectangle( m_cvImage, cvPoint(x1-widthDIV2,y1-heightDIV2), cvPoint(x1+widthDIV2,y1+heightDIV2), CV_RGB(color.r,color.g,color.b), -1, 4);
			break;

		case RADIUS: 
			if (graphManager.getSmooth())
				cvRectangle( m_cvImage, cvPoint(x1-x2,y1-y2), cvPoint(x1+x2,y1+y2), CV_RGB(color.r,color.g,color.b), -1, 16);
			else
				cvRectangle( m_cvImage, cvPoint(x1-x2,y1-y2), cvPoint(x1+x2,y1+y2), CV_RGB(color.r,color.g,color.b), -1, 4);
			break;
		}
	}

	if (graphManager.getStroke())
	{
		// Get Stroke Color
		Color color        = graphManager.getStrokeColor();
		int   strokeWeight = graphManager.getStrokeWeight();
		switch( graphManager.getRectMode() )
		{

		case CORNER: 
			if (graphManager.getSmooth())
				cvRectangle( m_cvImage, cvPoint(x1,y1), cvPoint(x1+x2,y1+y2), CV_RGB(color.r,color.g,color.b), strokeWeight, 16);
			else
				cvRectangle( m_cvImage, cvPoint(x1,y1), cvPoint(x1+x2,y1+y2), CV_RGB(color.r,color.g,color.b), strokeWeight, 4);
			break;

		case CORNERS: 
			if (graphManager.getSmooth())
				cvRectangle( m_cvImage, cvPoint(x1,y1), cvPoint(x2,y2), CV_RGB(color.r,color.g,color.b), strokeWeight, 16);
			else
				cvRectangle( m_cvImage, cvPoint(x1,y1), cvPoint(x2,y2), CV_RGB(color.r,color.g,color.b), strokeWeight, 4);
			break;

		case CENTER: 
			if (graphManager.getSmooth())
				cvRectangle( m_cvImage, cvPoint(x1-widthDIV2,y1-heightDIV2), cvPoint(x1+widthDIV2,y1+heightDIV2), CV_RGB(color.r,color.g,color.b), strokeWeight, 16);
			else
				cvRectangle( m_cvImage, cvPoint(x1-widthDIV2,y1-heightDIV2), cvPoint(x1+widthDIV2,y1+heightDIV2), CV_RGB(color.r,color.g,color.b), strokeWeight, 4);
			break;

		case RADIUS: 
			if (graphManager.getSmooth())
				cvRectangle( m_cvImage, cvPoint(x1-x2,y1-y2), cvPoint(x1+x2,y1+y2), CV_RGB(color.r,color.g,color.b), strokeWeight, 16);
			else
				cvRectangle( m_cvImage, cvPoint(x1-x2,y1-y2), cvPoint(x1+x2,y1+y2), CV_RGB(color.r,color.g,color.b), strokeWeight, 4);
			break;
		}
	}

	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

/**
 * @brief Draws a ellipse inside an image
 *
 * @param x x, first point
 * @param y y, first point
 * @param width  width
 * @param height height
 */
void Image::ellipse( float x1, float y1, float x2, float y2, float angle )
{

	//TODO: Hay para cambiar de mod deg a rad?

	// Check the image is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	GraphicsManager& graphManager = GraphicsManager::getSingleton();

	float widthDIV2 = x2/2;
	float heightDIV2 = y2/2;

	Color color        = graphManager.getFillColor();

	if (graphManager.getFill())
	{
		// Get Fill Color

		switch( graphManager.getEllipseMode() )
		{
		case CORNER: 
			if (graphManager.getSmooth())
				cvEllipse(	m_cvImage, cvPoint(x1,y1), cvSize(x1+x2,y1+y2), degrees(angle),	0, 360,	CV_RGB(color.r,color.g,color.b),-1,16 );
			else
				cvEllipse(	m_cvImage, cvPoint(x1,y1), cvSize(x1+x2,y1+y2), degrees(angle),	0, 360,	CV_RGB(color.r,color.g,color.b),-1,4 );
			break;

		case CORNERS: 
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(x2,y2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), -1, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(x2,y2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), -1, 4);
			break;

		case CENTER:
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(widthDIV2,heightDIV2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), -1, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(widthDIV2,heightDIV2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), -1, 4);
			break;

		case RADIUS: 
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1-x2,y1-y2), cvSize(x1+x2,y1+y2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), -1, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1-x2,y1-y2), cvSize(x1+x2,y1+y2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), -1, 4);
			break;
		}
	}

	if (graphManager.getStroke())
	{
		// Get Stroke Color
		Color color        = graphManager.getStrokeColor();
		int   strokeWeight = graphManager.getStrokeWeight();
		switch( graphManager.getEllipseMode() )
		{

		case CORNER: 
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(x1+x2,y1+y2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(x1+x2,y1+y2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 4);
			break;

		case CORNERS: 
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(x2,y2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(x2,y2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 4);
			break;

		case CENTER: 
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(widthDIV2,heightDIV2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(widthDIV2,heightDIV2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 4);
			break;

		case RADIUS: 
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1-x2,y1-y2), cvSize(x1+x2,y1+y2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1-x2,y1-y2), cvSize(x1+x2,y1+y2), degrees(angle),	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 4);
			break;
		}
	}

	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;

}

/**
 * @brief Draws a ellipse inside an image
 *
 * @param x x, first point
 * @param y y, first point
 * @param width  width
 * @param height height
 */
void Image::ellipse( float x1, float y1, float x2, float y2 )
{
	
	// Check the image is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	GraphicsManager& graphManager = GraphicsManager::getSingleton();

	float widthDIV2 = x2/2;
	float heightDIV2 = y2/2;

	Color color        = graphManager.getFillColor();

	if (graphManager.getFill())
	{
		// Get Fill Color

		switch( graphManager.getEllipseMode() )
		{
		case CORNER: 
			if (graphManager.getSmooth())
				cvEllipse(	m_cvImage, cvPoint(x1,y1), cvSize(x1+x2,y1+y2), 0,	0, 360,	CV_RGB(color.r,color.g,color.b),-1,16 );
			else
				cvEllipse(	m_cvImage, cvPoint(x1,y1), cvSize(x1+x2,y1+y2), 0,	0, 360,	CV_RGB(color.r,color.g,color.b),-1,4 );
			break;

		case CORNERS: 
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(x2,y2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), -1, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(x2,y2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), -1, 4);
			break;

		case CENTER:
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(widthDIV2,heightDIV2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), -1, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(widthDIV2,heightDIV2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), -1, 4);
			break;

		case RADIUS: 
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1-x2,y1-y2), cvSize(x1+x2,y1+y2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), -1, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1-x2,y1-y2), cvSize(x1+x2,y1+y2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), -1, 4);
			break;
		}
	}

	if (graphManager.getStroke())
	{
		// Get Stroke Color
		Color color        = graphManager.getStrokeColor();
		int   strokeWeight = graphManager.getStrokeWeight();
		switch( graphManager.getEllipseMode() )
		{

		case CORNER: 
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(x1+x2,y1+y2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(x1+x2,y1+y2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 4);
			break;

		case CORNERS: 
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(x2,y2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(x2,y2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 4);
			break;

		case CENTER: 
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(widthDIV2,heightDIV2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1,y1), cvSize(widthDIV2,heightDIV2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 4);
			break;

		case RADIUS: 
			if (graphManager.getSmooth())
				cvEllipse( m_cvImage, cvPoint(x1-x2,y1-y2), cvSize(x1+x2,y1+y2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 16);
			else
				cvEllipse( m_cvImage, cvPoint(x1-x2,y1-y2), cvSize(x1+x2,y1+y2), 0,	0, 360, CV_RGB(color.r,color.g,color.b), strokeWeight, 4);
			break;
		}
	}

	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;

}
/**
 * @brief Method to apply a variety of basic filters to this image.
 *
 * @param ImageProcessingFilters type
 * @param float
 */
void Image::filter( ImageProcessingFilters type, float param1 )
{
	// Check the image is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	if (type == BLUR)
		cvSmooth(m_cvImage, m_cvImage, CV_BLUR , 9);

	if (type == ERODE)
		cvErode( m_cvImage, m_cvImage, 0, 1 );

	if (type == DILATE)
		cvDilate( m_cvImage, m_cvImage, 0, 1 );

	if (type == THRESHOLD)
	{
		m_imgThresholdFilter.setThreshold( (int) param1);
		m_imgThresholdFilter.apply(  *m_cvImage , *m_cvImage);
	}

	if (type == INVERT)
		cvNot( m_cvImage, m_cvImage);

	// TODO
	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

/**
 * @brief Method to apply a variety of basic filters to this image.
 *
 * @param ImageProcessingFilters type
 */
void Image::filter( ImageProcessingFilters type )
{
	// Check the image is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	if (type == BLUR)
		cvSmooth(m_cvImage, m_cvImage, CV_BLUR , 3);

	if (type == ERODE)
		cvErode( m_cvImage, m_cvImage, 0, 1 );

	if (type == DILATE)
		cvDilate( m_cvImage, m_cvImage, 0, 1 );

	if (type == THRESHOLD)
	{
		m_imgThresholdFilter.apply(  *m_cvImage , *m_cvImage);
	}

	if (type == INVERT)
	{
		cvNot( m_cvImage, m_cvImage);
	}
	// TODO
	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}


/**
 * @brief Converts an image from GRAYSCALE to Color format
 * TODO: optimize
 */
void Image::toColor()
{
	// Check valid
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to convert to color an invalid image. Maybe it is not initialized" );
		return;
	}

	// Check current format
	if ( m_nChannels >=3 )
		return;

	// Image Store data temporarily
	IplImage* tempImage = Graphics::ImageResourceManager::getSingleton().getImage( getWidth(), getHeight(), 3 );

	// Convert image
	cvCvtColor( m_cvImage, tempImage, CV_GRAY2RGB );

	// Update attributes
	m_nChannels = 3;

	// Release current image
	if ( m_cvImage )
		cvReleaseImage( &m_cvImage );

	// Release and recreate the quad
	m_quad.end();
	m_quad.init( tempImage->width, tempImage->height, getFormat() );

	// Clone the temp
	m_cvImage = cvCloneImage( tempImage );

	// Mark texture to be updated in the next draw call
	setUpdateTexture( true );
	
	// Release temp image
	Graphics::ImageResourceManager::getSingleton().releaseImage( tempImage );

	// Now this image is valid
	m_bIsValid = true;
}


/**
 * @brief Converts an image from Color (RGB) to GRAYSCALE
 * TODO: optimize
 */
void Image::toGray()
{
	// Check valid
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to convert to gray an invalid image. Maybe it is not initialized" );
		return;
	}

	// Check current format
	if ( m_nChannels == 1 )
		return;

	// Image Store data temporarily
	IplImage* tempImage = Graphics::ImageResourceManager::getSingleton().getImage( getWidth(), getHeight(), 1 );

	// Convert image
	cvCvtColor( m_cvImage, tempImage, CV_RGB2GRAY );

	// Update attributes
	m_nChannels = 1;

	// Release current image
	//if ( m_cvImage )
	cvReleaseImage( &m_cvImage );

	// Release and recreate the quad
	m_quad.end();
	m_quad.init( tempImage->width, tempImage->height, getFormat() );

	// Clone the temp
	m_cvImage = cvCloneImage( tempImage );

	// Mark texture to be updated in the next draw call
	setUpdateTexture( true );
	
	// Release temp image
	Graphics::ImageResourceManager::getSingleton().releaseImage( tempImage );

	// Now this image is valid
	m_bIsValid = true;
}

void Image::operator +=	( float scalar ){
	switch( m_cvImage->nChannels )
	{
	case 1:
	cvAddS( m_cvImage, cvScalar(scalar), m_cvImage );
		break;
	case 3:
	cvAddS( m_cvImage, cvScalar(scalar,scalar,scalar), m_cvImage );
		break;
	case 4:
	cvAddS( m_cvImage, cvScalar(scalar,scalar,scalar,scalar), m_cvImage );
		break;
	default:
		THROW_EXCEPTION( "Invalid number of channels in image" )
			break;
	}
}

void Image::operator -=	( float scalar ){
	switch( m_cvImage->nChannels )
	{
	case 1:
		cvSubS( m_cvImage, cvScalar(scalar), m_cvImage );
		break;
	case 3:
		cvSubS( m_cvImage, cvScalar(scalar,scalar,scalar), m_cvImage );
		break;
	case 4:
		cvSubS( m_cvImage, cvScalar(scalar,scalar,scalar,scalar), m_cvImage );
		break;
	default:
		THROW_EXCEPTION( "Invalid number of channels in image" )
			break;
	}
}

void Image::operator +=	( const Image& img ){

	cvAdd( m_cvImage, &img.getCVImage(), m_cvImage );
}

void Image::operator -=	( const Image& img ){

	cvSub( m_cvImage, &img.getCVImage(), m_cvImage );
}

void Image::blend( const Image& other, float percentage )
{
	percentage = constrain( percentage, 0.0f, 1.0f );
	cvAddWeighted( m_cvImage, 1.0 - percentage, &other.getCVImage(), percentage, 0.0f, m_cvImage );
}

void Image::operator = ( float scalar)
{
	switch( m_cvImage->nChannels )
	{
		case 1:
			cvSet( m_cvImage, cvScalar(scalar) );
			break;
		case 3:
			cvSet( m_cvImage, cvScalar(scalar,scalar,scalar) );
			break;
		case 4:
			cvSet( m_cvImage, cvScalar(scalar,scalar,scalar,scalar) );
			break;
		default:
			THROW_EXCEPTION( "Invalid number of channels in image" )
				break;
	}
}

/**
 * @brief Copy from image
 * TODO: optimize
 */
void Image::copy( const Image& img )
{
	// TODO: add check size and formats
	cvCopy( &img.getCVImage(), m_cvImage, NULL );
	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

/**
 * @brief Copy part of image to another
 * TODO: optimize
 */
void Image::copy( const Image& img , int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH )
{

	// TODO: add check size and formats
	cvCopy( &img.getCVImage(), m_cvImage, NULL );
	// Update texture when the next drawing call is made by the user
	m_bUpdateTexture = true;
}

// Ink modes
void Image::setInkMode( ImageInkModes type )
{

	if (type == ADD)
		m_quad.setAdditiveMode( true );

	if (type == BLEND)
		m_quad.setAdditiveMode( false );

};

} // namespace Graphics
