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
#include "Image.h"
#include "ImageResourceManager.h"
#include "GraphicsManager.h"

// Common
#include "common/Exception.h"
#include "common/ResourceManager.h"
#include "common/LogManager.h"
#include "common/MathUtils.h"
#include "common/CommonConstants.h"

// Framework
#include "framework/Application.h"

// Ogre
#include "OgreDataStream.h"

// OpenCV
#include "OpenCV/cxcore.h"

namespace Cing
{

	// Static member init
	ImageThresholdFilter	Image::m_imgThresholdFilter;
	ImageDifferenceFilter	Image::m_imgDiffFilter;

	/**
	* @brief Constructor. This constructor does not load or creates any image.
	*/
	Image::Image():
		m_bIsValid( false ),
		m_bVFlipped( false ),
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
		m_bVFlipped( img.isVFlipped() ),
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
		m_bVFlipped( false ),
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
		m_bVFlipped( false ),
		m_bIsValid( false )
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
		Application::getSingleton().checkSubsystemsInit();

		// Create the empty IplImage image
		m_nChannels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );
		m_cvImage.create( height, width, CV_MAKETYPE(CV_8U,m_nChannels) );

		// Create the texture quad (to draw image)
		m_quad.init( m_cvImage.cols, m_cvImage.rows, format );

		// Store the format
		m_format = format;

		// The class is now initialized
		m_bIsValid = true;
		m_bUpdateTexture = true;
	}

	/**
	* @brief Creates an image of the specified size, but does not load it from file.
	* Moreover, this image will be used as render to texture target (to render a scene in to it).
	*
	* @param width  Width of the image to be created
	* @param height Height of the image to be created
	*/
	void Image::initAsRenderTarget( int width, int height )
	{
		// Check if the class is already initialized
		if ( isValid() )
			THROW_EXCEPTION( "Image already initialized" );

		// Format always RGB for render targets (is this necessary?)
		m_format = RGB;

		// Check application correctly initialized (could not be if the user didn't calle size function)
		Application::getSingleton().checkSubsystemsInit();

		// Create the empty IplImage image
		m_nChannels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)m_format );
		m_cvImage.create( height, width, CV_MAKETYPE(CV_8U,m_nChannels) );

		// Create the texture quad (to draw image)
		m_quad.init( m_cvImage.cols, m_cvImage.rows, m_format, true );

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
		Application::getSingleton().checkSubsystemsInit();

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
		Application::getSingleton().checkSubsystemsInit();

		// If the image has already been initialized -> release it first
		if ( isValid () )
			end();

		// Load image from file
		m_image.load( name, ResourceManager::userResourcesGroupName );

		// Check if image was loaded ok
		if ( m_image.getData() )
			LOG( "Image %s succesfully loaded", name.c_str() );
		else
		{
			LOG( "Error loading Image %s", name.c_str() );
			return;
		}

		// Store image format (for images loaded from file, we force it to be RGB or RGBA
		if (m_image.getHasAlpha() )
			m_format = RGBA;
		else
			m_format = RGB;

		// Create the image
		m_nChannels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)m_format );
		m_cvImage.create( m_image.getHeight(), m_image.getWidth(), CV_MAKETYPE(CV_8U,m_nChannels) );

		// Set the data comming from the file image
		cv::Mat imgData(m_cvImage.rows, m_cvImage.cols, CV_MAKETYPE(CV_8U,m_nChannels), m_image.getData(), m_image.getWidth() * m_nChannels);
		imgData.copyTo(m_cvImage);

		// Check if we need to flip the channels red and green (for example png and jpgs are loaded as BGR by Ogre)
		GraphicsType currentImageFormat = (GraphicsType)m_image.getFormat();
		if ( (currentImageFormat == BGR) || (currentImageFormat == BGRA) )
		{
			if ( m_nChannels == 3 )
				cv::cvtColor( m_cvImage, m_cvImage, CV_RGB2BGR );
			else
				cv::cvtColor( m_cvImage, m_cvImage, CV_RGBA2BGRA );
		}

		// Create the texture quad (to draw image)
		m_quad.init( (int)m_image.getWidth(), (int)m_image.getHeight(), m_format );

		// Load image data to texture
		updateTexture();

		// The class is now initialized
		m_bIsValid = true;
		m_bUpdateTexture = false;

		// Now we can release the image used by the image loader
		m_image.freeMemory();
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
		m_image.loadDynamicImage( (Ogre::uchar*)m_cvImage.data, m_cvImage.cols, m_cvImage.rows, 1, (Ogre::PixelFormat)getFormat() );

		// Add the user app data folder to the name
		m_image.save( ResourceManager::userDataPath + name );
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

		// TODO: This is a hack to avoid a crash comming from cvReleaseImage
		// when it is called by a static object destructor (global Image variable created by the user)
		// when there is image copy involved
		Ogre::SceneManager* sceneManager = GraphicsManager::getSingleton().getSceneManagerPtr();
		if ( sceneManager == NULL )
			return;

		//Release IplImage
		m_cvImage.release();

		// Release the quad
		m_quad.end();

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
	void Image::setData( const unsigned char* imageData, int width, int height, GraphicsType format )
	{
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to set data to an invalid image (it has not been initialized)" );
			return;
		}

		// Check dimensions
		int channels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );
		if ( (width != m_cvImage.cols) || (height != m_cvImage.rows) || (channels != m_cvImage.channels()) )
		{
			LOG_ERROR( "Trying to set data with a wrong size of number of channels" );
			return;
		}

		// Set the data
		cv::Mat imgData(height, width, CV_MAKETYPE(CV_8U,channels), (void*)imageData, width*channels);
		imgData.copyTo(m_cvImage);

		// Make the image to be updated to texture in the next draw
		m_bUpdateTexture = true;
	}

	/**
	 * @brief Returns the image data (buffer)
	 * @return the image data (buffer)
	 */	
	unsigned char* Image::getData() 
	{ 
		return isValid()? m_cvImage.data: NULL; 
	}

	/**
	* @brief Returns image width
	* @return the height of the image
	*/
	int Image::getWidth() const
	{
		return m_cvImage.data? (int)m_cvImage.cols: 0;
	}

	/**
	* @brief Returns image height
	* @return the height of the image
	*/
	int Image::getHeight() const
	{
		return m_cvImage.data? (int)m_cvImage.rows: 0;
	}

	/**
	* @brief Returns image format
	* @return the imageFomat of the image
	*/
	GraphicsType Image::getFormat() const
	{
		// Return the stored format (as the image was not loaded from file)
		return m_format;
	}

	/**
	* @brief Method to get the color of a pixel at x-y position in this image.
	* TODO: too slow!
	* @param x x coordinate of the pixel that wants to be retrieved
	* @param y y coordinate of the pixel that wants to be retrieved
	*/
	Color Image::getPixel( int x, int y ) const
	{

		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to get a pixel from an invalid image" );

		// Check boundaries
		if ( x < 0 )				x = 0;
		if ( x > m_cvImage.cols )	x = m_cvImage.cols - 1;
		if ( y < 0 )				y = 0;
		if ( y > m_cvImage.rows)	y = m_cvImage.rows -1 ;

		// Read color
		int			   channels = m_cvImage.channels();
		unsigned char* pixelPtr = m_cvImage.data + m_cvImage.step * y;
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
		m_bVFlipped = !m_bVFlipped;

		// Flip quad's texture coordinates
		m_quad.flipVertical( m_bVFlipped );
	}

	/**
	* @brief Set if texture updates automatically every frame
	*/
	void Image::setUpdateTexture(bool updateTextureFlag )
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
	void Image::draw( float xPos, float yPos )
	{
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
	void Image::draw( float xPos, float yPos, float width, float height )
	{
		// check if texture needs to be updated
		if (m_bUpdateTexture)
		{
			updateTexture();
			m_bUpdateTexture = false;
		}

		m_quad.draw2d( xPos, yPos, width, height );
	}

	/**
	* @brief Draws the image in 2d -> screen coordinates, but specifying the four corners
	* order: top-left, top-right, bottom-right, bottom-left (anti-cloclwise)
	*/
	void Image::draw( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 )
	{
		// check if texture needs to be updated
		if (m_bUpdateTexture)
		{
			updateTexture();
			m_bUpdateTexture = false;
		}

		m_quad.draw2d( x1, y1, x2, y2, x3, y3, x4, y4 );
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
		Application::getSingleton().checkSubsystemsInit();

		// Check if the image is initialized -> if not initialize with "other"'s format and size
		if ( !isValid() )
			init( other.getWidth(), other.getHeight(), other.getFormat() );

		// Check if the size of the image differs
		if ( (other.getWidth() != getWidth()) || (other.getHeight() != getHeight()) || (other.getFormat() != getFormat()) )
		{
			end();
			init( other.getWidth(), other.getHeight(), other.getFormat() );
			
			// Check if the image was v flipped
			if ( m_bVFlipped )
				m_quad.flipVertical();
		}

		// Copy the data
		const unsigned char* imageData = const_cast<Image&>(other).getData();
		setData( imageData, getWidth(), getHeight(), getFormat() );

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
	* @brief Sets the rotation of the image (in relation to its parents)
	*
	* @param[in] axis	rotation axis
	* @param[in] angle rotation angle (degrees)
	*/
	void Image::setOrientation( const Vector& axis, float angle )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to rotate an invalid image" );

		m_quad.setOrientation( axis, angle );
	}


	/**
	* @brief Adds rotation of the image (in relation to its parents)
	*
	* @param[in] axis	rotation axis
	* @param[in] angle rotation angle (degrees)
	*/
	void Image::rotate( const Vector& axis, float angle )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to rotate an invalid image" );

		m_quad.rotate( axis, angle );
	}

	/**
	* @brief Sets the scale of the image (1.0 would be normal scale)
	*/
	void Image::setScale( float xScale, float yScale, float zScale )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to rotate an invalid image" );

		m_quad.setScale( xScale, yScale, zScale );
	}

	/**
	* @brief  Load image data to texture
	*/
	void Image::updateTexture()
	{
		m_quad.updateTexture( m_cvImage.data, m_cvImage.cols, m_cvImage.rows, getFormat());
		m_bUpdateTexture = false;
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
	void Image::triangle( int x1, int y1, int x2, int y2, int x3, int y3 )
	{
		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );

		GraphicsManager& graphManager = GraphicsManager::getSingleton();

		if (graphManager.getFill())
		{
			// Get Fill Color
			Color color        = graphManager.getFillColor();
			cv::Point pts[3] = { cv::Point(x1,y1), cv::Point(x2,y2), cv::Point(x3,y3)};

			if (graphManager.getSmooth())
				cv::fillConvexPoly( m_cvImage , (const cv::Point*)&pts, 3, cv::Scalar( color.r, color.g, color.b, color.a ), CV_AA, 0 );
			else
				cv::fillConvexPoly( m_cvImage , (const cv::Point*)&pts, 3, cv::Scalar( color.r, color.g, color.b, color.a ), 4, 0 );
		}

		if (graphManager.getStroke())
		{
			// Get Stroke Color
			Color color        = graphManager.getStrokeColor();
			int   strokeWeight = graphManager.getStrokeWeight();

			if (graphManager.getSmooth())
			{
				cv::line( m_cvImage,cv::Point(x1,y1),cv::Point(x2,y2),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,CV_AA,0);
				cv::line( m_cvImage,cv::Point(x2,y2),cv::Point(x3,y3),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,CV_AA,0);
				cv::line( m_cvImage,cv::Point(x3,y3),cv::Point(x1,y1),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,CV_AA,0);
			}else{
				cv::line( m_cvImage,cv::Point(x1,y1),cv::Point(x2,y2),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,4,0);
				cv::line( m_cvImage,cv::Point(x2,y2),cv::Point(x3,y3),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,4,0);
				cv::line( m_cvImage,cv::Point(x3,y3),cv::Point(x1,y1),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,4,0);
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
	void Image::line( int x1, int y1, int x2, int y2 )
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
			cv::line(	m_cvImage,
						cv::Point(x1,y1),
						cv::Point(x2,y2),
						cv::Scalar( color.r, color.g, color.b, color.a ),
						strokeWeight,	///< Thickness.
						CV_AA,	///< Type of the ellipse boundary, see cvLine description.
						0); ///< Number of fractional bits in the center coordinates and axes' values.
		}else{
			cv::line(	m_cvImage,
						cv::Point(x1,y1),
						cv::Point(x2,y2),
						cv::Scalar( color.r, color.g, color.b, color.a ),
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
	void Image::arc( int x, int y, int width, int height, float start, float end )
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
				cv::ellipse(	m_cvImage,						///-> Image.
								cv::Point(x,y),					///-> Center of the ellipse.
								cvSize(width,height),			///-> Length of the ellipse axes.
								0,								///->	Rotation angle.
								start,							///-> Starting angle of the elliptic arc.
								end,							///-> Ending angle of the elliptic arc.
								cv::Scalar( color.r, color.g, color.b, color.a ),///-> Ellipse color.
								-1,
								CV_AA);							///-> Thickness of the ellipse arc.
			}else{
				cv::ellipse(	m_cvImage,						///-> Image.
								cv::Point(x,y),					///-> Center of the ellipse.
								cvSize(width,height),			///-> Length of the ellipse axes.
								0,								///->	Rotation angle.
								start,							///-> Starting angle of the elliptic arc.
								end,							///-> Ending angle of the elliptic arc.
								cv::Scalar( color.r, color.g, color.b, color.a ),///-> Ellipse color.
								-1,
								4);								///-> Thickness of the ellipse arc.
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
				cv::ellipse(	m_cvImage,						///-> Image.
								cv::Point(x,y),					///-> Center of the ellipse.
								cvSize(width,height),			///-> Length of the ellipse axes.
								0,								///->	Rotation angle.
								start,							///-> Starting angle of the elliptic arc.
								end,							///-> Ending angle of the elliptic arc.
								cv::Scalar( color.r, color.g, color.b, color.a ),///-> Ellipse color.
								strokeWeight,
								CV_AA );								///-> Thickness of the ellipse arc.
			}else{
				cv::ellipse(	m_cvImage,						///-> Image.
								cv::Point(x,y),					///-> Center of the ellipse.
								cvSize(width,height),			///-> Length of the ellipse axes.
								0,								///->	Rotation angle.
								start,							///-> Starting angle of the elliptic arc.
								end,							///-> Ending angle of the elliptic arc.
								cv::Scalar( color.r, color.g, color.b, color.a ),///-> Ellipse color.
								strokeWeight,
								4 );							///-> Thickness of the ellipse arc.

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
	void Image::point( int x, int y )
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
		cv::rectangle( m_cvImage,
						cv::Point(x,y),
						cv::Point(x,y),
						cv::Scalar( color.r, color.g, color.b, color.a ),
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
	void Image::quad( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 )
	{
		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );

		GraphicsManager& graphManager = GraphicsManager::getSingleton();

		if (graphManager.getFill())
		{
			// Get Fill Color
			Color color		= graphManager.getFillColor();
			cv::Point pts[4]= { cv::Point(x1,y1), cv::Point(x2,y2), cv::Point(x3,y3), cv::Point(x4,y4) };

			if (graphManager.getSmooth())
				cv::fillConvexPoly( m_cvImage , (const cv::Point*)&pts, 4,	cv::Scalar( color.r, color.g, color.b, color.a ), CV_AA, 0 );
			else
				cv::fillConvexPoly( m_cvImage , (const cv::Point*)&pts, 4,	cv::Scalar( color.r, color.g, color.b, color.a ), 4, 0 );
		}

		if (graphManager.getStroke())
		{
			// Get Stroke Color
			// Get Fill Color
			Color color        = graphManager.getStrokeColor();
			int   strokeWeight = graphManager.getStrokeWeight();

			if (graphManager.getSmooth())
			{

				cv::line( m_cvImage,cv::Point(x1,y1),cv::Point(x2,y2),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,CV_AA,0);
				cv::line( m_cvImage,cv::Point(x2,y2),cv::Point(x3,y3),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,CV_AA,0);
				cv::line( m_cvImage,cv::Point(x3,y3),cv::Point(x4,y4),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,CV_AA,0);
				cv::line( m_cvImage,cv::Point(x4,y4),cv::Point(x1,y1),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,CV_AA,0);

			}else{

				cv::line( m_cvImage,cv::Point(x1,y1),cv::Point(x2,y2),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,4,0);
				cv::line( m_cvImage,cv::Point(x2,y2),cv::Point(x3,y3),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,4,0);
				cv::line( m_cvImage,cv::Point(x3,y3),cv::Point(x4,y4),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,4,0);
				cv::line( m_cvImage,cv::Point(x4,y4),cv::Point(x1,y1),cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight,4,0);
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
	void Image::text( int x1, int y1,  const char* text )
	{
		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );

		// Get Stroke and Fill Color
		GraphicsManager& graphManager = GraphicsManager::getSingleton();
		Color color			= graphManager.getStrokeColor();
		int   strokeWeight	= graphManager.getStrokeWeight();
		int	  fontScale		= 2;

		cv::putText(m_cvImage, text, cv::Point(x1,y1), cv::FONT_HERSHEY_SIMPLEX, fontScale, cv::Scalar( color.r, color.g, color.b, color.a ));

		// Update texture when the next drawing call is made by the user
		m_bUpdateTexture = true;
	}

	/**
	* @brief Draws a rectangle inside an image
	*
	* @param x x,		top-left cornder x coordinate
	* @param y y,		top-left cornder y coordinate
	* @param width		width
	* @param height	height
	*/
	void Image::rect( int x, int y, int width, int height )
	{
		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );

		GraphicsManager& graphManager = GraphicsManager::getSingleton();

		float widthDIV2 = (float)width/2.0f;
		float heightDIV2 = (float)height/2.0f;

		if (graphManager.getFill())
		{
			// Get Fill Color
			Color color        = graphManager.getFillColor();

			switch( graphManager.getRectMode() )
			{

			case CORNER:
				if (graphManager.getSmooth())
					cv::rectangle( m_cvImage, cv::Point(x,y), cv::Point(x+width,y+height), cv::Scalar( color.r, color.g, color.b, color.a ), -1, CV_AA);
				else
					cv::rectangle( m_cvImage, cv::Point(x,y), cv::Point(x+width,y+height), cv::Scalar( color.r, color.g, color.b, color.a ), -1, 4);
				break;

			case CORNERS:
				if (graphManager.getSmooth())
					cv::rectangle( m_cvImage, cv::Point(x,y), cv::Point(width,height), cv::Scalar( color.r, color.g, color.b, color.a ), -1, CV_AA);
				else
					cv::rectangle( m_cvImage, cv::Point(x,y), cv::Point(width,height), cv::Scalar( color.r, color.g, color.b, color.a ), -1, 4);
				break;

			case CENTER:
				if (graphManager.getSmooth())
					cv::rectangle( m_cvImage, cv::Point(x-(int)widthDIV2,y-(int)heightDIV2), cv::Point(x+(int)widthDIV2,y+(int)heightDIV2), cv::Scalar( color.r, color.g, color.b, color.a ), -1, CV_AA);
				else
					cv::rectangle( m_cvImage, cv::Point(x-(int)widthDIV2,y-(int)heightDIV2), cv::Point(x+(int)widthDIV2,y+(int)heightDIV2), cv::Scalar( color.r, color.g, color.b, color.a ), -1, 4);
				break;

			case RADIUS:
				if (graphManager.getSmooth())
					cv::rectangle( m_cvImage, cv::Point(x-width,y-height), cv::Point(x+width,y+height), cv::Scalar( color.r, color.g, color.b, color.a ), -1, CV_AA);
				else
					cv::rectangle( m_cvImage, cv::Point(x-width,y-height), cv::Point(x+width,y+height), cv::Scalar( color.r, color.g, color.b, color.a ), -1, 4);
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
					cv::rectangle( m_cvImage, cv::Point(x,y), cv::Point(x+width,y+height), cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, CV_AA);
				else
					cv::rectangle( m_cvImage, cv::Point(x,y), cv::Point(x+width,y+height), cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, 4);
				break;

			case CORNERS:
				if (graphManager.getSmooth())
					cv::rectangle( m_cvImage, cv::Point(x,y), cv::Point(width,height), cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, CV_AA);
				else
					cv::rectangle( m_cvImage, cv::Point(x,y), cv::Point(width,height), cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, 4);
				break;

			case CENTER:
				if (graphManager.getSmooth())
					cv::rectangle( m_cvImage, cv::Point(x-(int)widthDIV2,y-(int)heightDIV2), cv::Point(x+(int)widthDIV2,y+(int)heightDIV2), cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, CV_AA);
				else
					cv::rectangle( m_cvImage, cv::Point(x-(int)widthDIV2,y-(int)heightDIV2), cv::Point(x+(int)widthDIV2,y+(int)heightDIV2), cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, 4);
				break;

			case RADIUS:
				if (graphManager.getSmooth())
					cv::rectangle( m_cvImage, cv::Point(x-width,y-height), cv::Point(x+width,y+height), cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, CV_AA);
				else
					cv::rectangle( m_cvImage, cv::Point(x-width,y-height), cv::Point(x+width,y+height), cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, 4);
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
	* @param angleDegrees Angle in degrees in wich the ellipse will be rotated
	*/
	void Image::ellipse( int x1, int y1, int x2, int y2, float angleDegrees /*= 0*/ )
	{

		//TODO: Hay para cambiar de mod deg a rad?

		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );

		GraphicsManager& graphManager = GraphicsManager::getSingleton();

		float widthDIV2 = (float)x2/2.0f;
		float heightDIV2 = (float)y2/2.0f;

		Color color        = graphManager.getFillColor();

		if (graphManager.getFill())
		{
			// Get Fill Color

			switch( graphManager.getEllipseMode() )
			{
			case CORNER:
				if (graphManager.getSmooth())
					cv::ellipse( m_cvImage, cv::Point(x1,y1), cvSize(x1+x2,y1+y2), angleDegrees,	0, 360,	cv::Scalar( color.r, color.g, color.b, color.a ),-1,CV_AA);
				else
					cv::ellipse(	m_cvImage, cv::Point(x1,y1), cvSize(x1+x2,y1+y2), angleDegrees,	0, 360,	cv::Scalar( color.r, color.g, color.b, color.a ),-1,4 );
				break;

			case CORNERS:
				if (graphManager.getSmooth())
					cv::ellipse( m_cvImage, cv::Point(x1,y1), cvSize(x2,y2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), -1, CV_AA);
				else
					cv::ellipse( m_cvImage, cv::Point(x1,y1), cvSize(x2,y2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), -1, 4);
				break;

			case CENTER:
				if (graphManager.getSmooth())
					cv::ellipse( m_cvImage, cv::Point(x1,y1), cvSize((int)widthDIV2,(int)heightDIV2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), -1, CV_AA);
				else
					cv::ellipse( m_cvImage, cv::Point(x1,y1), cvSize((int)widthDIV2,(int)heightDIV2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), -1, 4);
				break;

			case RADIUS:
				if (graphManager.getSmooth())
					cv::ellipse( m_cvImage, cv::Point(x1-x2,y1-y2), cvSize(x1+x2,y1+y2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), -1, CV_AA);
				else
					cv::ellipse( m_cvImage, cv::Point(x1-x2,y1-y2), cvSize(x1+x2,y1+y2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), -1, 4);
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
					cv::ellipse( m_cvImage, cv::Point(x1,y1), cvSize(x1+x2,y1+y2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, CV_AA);
				else
					cv::ellipse( m_cvImage, cv::Point(x1,y1), cvSize(x1+x2,y1+y2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, 4);
				break;

			case CORNERS:
				if (graphManager.getSmooth())
					cv::ellipse( m_cvImage, cv::Point(x1,y1), cvSize(x2,y2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, CV_AA);
				else
					cv::ellipse( m_cvImage, cv::Point(x1,y1), cvSize(x2,y2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, 4);
				break;

			case CENTER:
				if (graphManager.getSmooth())
					cv::ellipse( m_cvImage, cv::Point(x1,y1), cvSize((int)widthDIV2,(int)heightDIV2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, CV_AA);
				else
					cv::ellipse( m_cvImage, cv::Point(x1,y1), cvSize((int)widthDIV2,(int)heightDIV2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, 4);
				break;

			case RADIUS:
				if (graphManager.getSmooth())
					cv::ellipse( m_cvImage, cv::Point(x1-x2,y1-y2), cvSize(x1+x2,y1+y2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, CV_AA);
				else
					cv::ellipse( m_cvImage, cv::Point(x1-x2,y1-y2), cvSize(x1+x2,y1+y2), angleDegrees,	0, 360, cv::Scalar( color.r, color.g, color.b, color.a ), strokeWeight, 4);
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
	* @param int argument fot the filter that will be applied
	*/
	void Image::filter( ImageProcessingFilters type, int param1 )
	{
		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );

		if (type == BLUR)
			cv::blur(m_cvImage, m_cvImage, cv::Size(param1,param1) );

		if (type == ERODE)
			cv::erode( m_cvImage, m_cvImage, cv::Mat(), cv::Point(-1, -1), param1 );

		if (type == DILATE)
			cv::dilate( m_cvImage, m_cvImage, cv::Mat(), cv::Point(-1, -1), param1 );
			
		if (type == THRESHOLD)
		{
			m_imgThresholdFilter.setThreshold(param1);
			m_imgThresholdFilter.apply(  (IplImage)m_cvImage , (IplImage)m_cvImage);
		}

		if (type == INVERT)
			cv::invert( m_cvImage, m_cvImage);

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
			cv::blur(m_cvImage, m_cvImage, cv::Size(3, 3));

		if (type == ERODE)
			cv::erode( m_cvImage, m_cvImage, cv::Mat() );

		if (type == DILATE)
			cv::erode( m_cvImage, m_cvImage, cv::Mat() );

		if (type == THRESHOLD)
		{
			m_imgThresholdFilter.apply( (IplImage)m_cvImage , (IplImage)m_cvImage);
		}

		if (type == INVERT)
		{
			cv::invert( m_cvImage, m_cvImage );

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
		IplImage* tempImage = ImageResourceManager::getSingleton().getImage( getWidth(), getHeight(), 3 );

		// Convert image
		cv::Mat temp(tempImage);
		cv::cvtColor( m_cvImage, temp, CV_GRAY2RGB );

		// Re-init the image with the new format and se the color data
		end();
		init( tempImage->width, tempImage->height, RGB );
		setData( (unsigned char*)tempImage->imageData, tempImage->width, tempImage->height, RGB );

		// Check if the image was v flipped
		if ( m_bVFlipped )
			m_quad.flipVertical();

		// Release temp image
		ImageResourceManager::getSingleton().releaseImage( tempImage );

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

		// Check current format, if it is already grayscale -> do nothing
		if ( m_nChannels == 1 )
			return;

		// Image Store data temporarily
		IplImage* tempImage = ImageResourceManager::getSingleton().getImage( getWidth(), getHeight(), 1 );

		// Convert image
		cv::Mat temp(tempImage);
		cv::cvtColor( m_cvImage, temp, CV_RGB2GRAY );

		// Re-init the image with the new format and se the grayscale data
		end();
		init( tempImage->width, tempImage->height, GRAYSCALE );
		setData( (unsigned char*)tempImage->imageData, tempImage->width, tempImage->height, GRAYSCALE );

		// Check if the image was v flipped
		if ( m_bVFlipped )
			m_quad.flipVertical();

		// Release temp image
		ImageResourceManager::getSingleton().releaseImage( tempImage );

		// Now this image is valid
		m_bIsValid = true;
	}

	void Image::operator +=	( float scalar ){


		switch( m_cvImage.channels() )
		{
		case 1:
			cv::add( m_cvImage, cv::Scalar(scalar), m_cvImage );
			break;
		case 3:
			cv::add( m_cvImage, cv::Scalar(scalar,scalar,scalar), m_cvImage );
			break;
		case 4:
			cv::add( m_cvImage, cv::Scalar(scalar,scalar,scalar,scalar), m_cvImage );
			break;
		default:
			THROW_EXCEPTION( "Invalid number of channels in image" )
				break;
		}
	}

	void Image::operator -=	( float scalar ){
		switch( m_cvImage.channels() )
		{
		case 1:
			cv::subtract( m_cvImage, cv::Scalar(scalar), m_cvImage );
			break;
		case 3:
			cv::subtract( m_cvImage, cv::Scalar(scalar,scalar,scalar), m_cvImage );
			break;
		case 4:
			cv::subtract( m_cvImage, cv::Scalar(scalar,scalar,scalar,scalar), m_cvImage );
			break;
		default:
			THROW_EXCEPTION( "Invalid number of channels in image" )
				break;
		}
	}

	void Image::operator +=	( const Image& img ){

		cv::add( m_cvImage, &img.getCVImage(), m_cvImage );
		setUpdateTexture(true);
	}

	void Image::operator -=	( const Image& img ){

		cv::subtract( m_cvImage, &img.getCVImage(), m_cvImage );
		setUpdateTexture(true);
	}

	/**
	* @brief Blend two images
	*
	* @param Image& other
	* @param float  percentage
	*/
	void Image::blend( const Image& other, float percentage )
	{
		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to blend an image without initialization" );

		if ( other.getFormat() != getFormat() )
			THROW_EXCEPTION( "Images with different format" );

		if ( other.getWidth() != getWidth() || other.getHeight() != getHeight())
			THROW_EXCEPTION( "Images with different sizes" );

		percentage = map( percentage, 0, 100, 0.0f, 1.0f );
		cv::addWeighted( m_cvImage, 1.0 - percentage, &other.getCVImage(), percentage, 0.0f, m_cvImage );
	}

	void Image::operator = ( float scalar)
	{
		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );

		switch( m_cvImage.channels() )
		{
		case 1:
			m_cvImage = cv::Scalar(scalar);
			break;
		case 3:
			m_cvImage = cv::Scalar(scalar,scalar,scalar);
			break;
		case 4:
			m_cvImage = cv::Scalar(scalar,scalar,scalar,scalar);
			break;
		default:
			THROW_EXCEPTION( "Invalid number of channels in image" )
				break;
		}
	}

	/**
	* @brief Fill the image with an input color
	*
	* @param Color
	*/
	void Image::fill( const Color& color )
	{

		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );

		// Set the entire image 
		switch( m_cvImage.channels() )
		{
		case 1:
			m_cvImage = cv::Scalar(color.r);
			break;
		case 3:
			m_cvImage = cv::Scalar(color.r, color.g, color.b);
			break;
		case 4:
			m_cvImage = cv::Scalar(color.r, color.g, color.b, color.a);
			break;
		default:
			THROW_EXCEPTION( "Invalid number of channels in image" )
				break;
		}

		// Update texture when the next drawing call is made by the user
		m_bUpdateTexture = true;

	}
	/**
	* @brief Copy from image
	* TODO: optimize
	*/
	void Image::copy( const Image& img )
	{
		*this = img;
	}

	// Ink modes
	void Image::setInkMode( ImageInkModes type )
	{

		if (type == ADD)
			m_quad.setAdditiveMode( true );

		if (type == BLEND)
			m_quad.setAdditiveMode( false );

	};

} // namespace Cing
