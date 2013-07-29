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

// Precompiled headers
#include "Cing-Precompiled.h"

#include "Image.h"
#include "GraphicsManager.h"
#include "BaseRenderer2D.h"
#include "GraphicsUserAPI.h"

// Common
#include "common/Exception.h"
#include "common/ResourceManager.h"
#include "common/LogManager.h"
#include "common/MathUtils.h"
#include "common/CommonConstants.h"
#include "common/SystemUtils.h"

// Framework
#include "framework/Application.h"

// Ogre
#include "OgreDataStream.h"
#include "OgreSceneManager.h"
#include "OgreImageCodec.h"

namespace Cing
{
	/**
	* @brief Constructor. This constructor does not load or creates any image.
	*/
	Image::Image():
		m_bIsValid( false ),
		m_bUpdateTexture( false ),
		m_loadedFromFile(false),
		m_path( "NOT_LOADED_FROM_FILE")
	{
	}

	/**
	* @brief Creates an image with the same data as the received image
	*
	* @param
	*/
	Image::Image( const Image& img):
		m_bIsValid( false ),
		m_bUpdateTexture( false ),
		m_loadedFromFile(false),
		m_path( "NOT_LOADED_FROM_FILE")
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
	Image::Image( int width, int height, GraphicsType format /*= RGB*/, Ogre::SceneManager* sm /*= NULL*/):
		m_bIsValid( false ),
		m_bUpdateTexture( false ),
		m_loadedFromFile(false),
		m_path( "NOT_LOADED_FROM_FILE")
	{
		init( width, height, format, sm );
	}

	/**
	* @brief Creates an image of the specified size and format, loading initial data
	*
	* @param data	Data to be set to the image once created
	* @param width  Width of the image to be created
	* @param height Height of the image to be created
	* @param format Format of the image to be created. Possible formats are: RGB, ARGB, GRAYSCALE
	*/
	Image::Image( unsigned char* data, int width, int height, GraphicsType format /*= RGB*/, Ogre::SceneManager* sm /*= NULL*/ ):
		m_bIsValid( false ),
		m_bUpdateTexture( false ),
		m_loadedFromFile(false),
		m_path( "NOT_LOADED_FROM_FILE")	
	{
		init( width, height, format, sm );
		setData( data );
	}


	/**
	* @brief Creates an image from a file. The image loaded can be modified afterwards.
	* @note The image file specified should be placed on the data directory of the application
	*
	* @param name  Name of the file to be loaded. It must be placed in the data directory.
	*/
	Image::Image( const std::string& name, Ogre::SceneManager* sm ):
		m_bIsValid( false ),
		m_loadedFromFile(false),
		m_path( "NOT_LOADED_FROM_FILE")
	{
		load( name, sm );
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
	void Image::init( int width, int height, GraphicsType format /*= RGB*/, Ogre::SceneManager* sm /*= NULL*/  )
	{
		// Check if the class is already initialized to free resources first
		// TODO: check if values are really different... 
		if ( isValid() )
			end();

		// Check application correctly initialized (could not be if the user didn't calle size function)
		Application::getSingleton().checkSubsystemsInit();

		// Create the empty image
		m_nChannels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );

		// Create a pixel box to be able to define the format
		// TODO: optimize this
		size_t bufferSize = Ogre::PixelUtil::getMemorySize( width, height, 1, (Ogre::PixelFormat)format );
		
		// Create the memory buffer
		m_data = ImageDataPtr( new unsigned char[bufferSize] );

		// TODO: DEFINE NUMBER OF CHANNELS
		m_image.loadDynamicImage( m_data.get(), width, height, (Ogre::PixelFormat)format );
		
		// Create the texture quad (to draw image)
		m_quad.init( width, height, format, DYNAMIC_WRITE_ONLY_DISCARDABLE, sm );

		// Store the format
		m_format			= format;
		m_loadedFromFile	= false;

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

		// Create the empty opencv Mat image
		m_nChannels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)m_format );
		
		// TODO: DEFINE NUMBER OF CHANNELS
		m_image.resize( width, height);


		// Create the texture quad (to draw image)
		m_quad.init( width, height, m_format, RENDERTARGET );

		// The class is now initialized
		m_bIsValid			= true;
		m_bUpdateTexture	= true;
		m_loadedFromFile	= false;

	}

	/**
	* @brief Creates a copy of an image
	*
	* @param img Image to be copied
	*/
	void Image::init( const Image& img)
	{
		// Check application correctly initialized (could not be if the user didn't call size() function)
		Application::getSingleton().checkSubsystemsInit();

		this->operator=( img );

		m_bIsValid			= true;
		m_bUpdateTexture	= false;
		m_loadedFromFile	= false;
	}

	/**
	 * @brief Loads an image file from disk and loads it into the texture manager so it is accessible just by the file name
	 * @param imagePath  Full path to the texture file
	 */
	bool Image::loadImageFromDisk(const std::string& imagePath) 
	{ 
		bool image_loaded = false; 
	
		// Check if this texture has already been loaded
		// TODO: INTERGRATE THIS TO OPTIMIZE
		//Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().getByName( imagePath );
		//if ( texture.isNull() == false )
		//{
		//	// Load texture into image pixels
		//	texture->convertToImage( m_image );
		//	return true;
		//}

		// Load the file stream (if the image has not previously been loaded)
		std::ifstream ifs(imagePath.c_str(), std::ios::binary|std::ios::in); 
		if (ifs.is_open()) 
		{ 
			// Make sure it has extension to know the file type
			Ogre::String tex_ext; 
			Ogre::String::size_type index_of_extension = imagePath.find_last_of('.'); 
			if (index_of_extension != Ogre::String::npos) 
			{ 
				// Create the ogre data stream from the file stream
				tex_ext = imagePath.substr(index_of_extension+1); 
				Ogre::DataStreamPtr data_stream(new Ogre::FileStreamDataStream(imagePath, &ifs, false)); 
				
				// Process image data from stream to store it in our shared pointer buffer
				// NOTE: this code comes from Ogre::Image::load method but is used directly to be able to store the image data in our shared pointer
                Ogre::Codec * pCodec = 0;

				// derive from magic number
                // read the first 32 bytes or file size, if less
                size_t magicLen = std::min(data_stream->size(), (size_t)32);
                char magicBuf[32];
                data_stream->read(magicBuf, magicLen);
                // return to start
                data_stream->seek(0);
                pCodec = Ogre::Codec::getCodec(magicBuf, magicLen);

				if( !pCodec )
				{
					LOG_ERROR( "[error] Unable to load image [%s]", imagePath.c_str() );
					return false;
				}

                Ogre::Codec::DecodeResult res = pCodec->decode(data_stream);

                Ogre::ImageCodec::ImageData* pData = static_cast<Ogre::ImageCodec::ImageData*>(res.second.getPointer());

                size_t imageWidth	= pData->width;
                size_t imageHeight	= pData->height;
                size_t bufferSize	= pData->size;
                Ogre::PixelFormat imageFormat = pData->format;
                			
				// TODO: optimize this
				// Create the local copy of the data (shared pointer memory). we do this to ensure we control the memory, no the ogre::image class, as in some cases it might be probelematic
				m_data = ImageDataPtr( new unsigned char[bufferSize] );
				std::memcpy( m_data.get(), res.first->getPtr(), bufferSize );

                // Make sure stream deletes
                res.first->setFreeOnClose(true);

				// Init the Ogre::image (with our buffer)
				m_image.loadDynamicImage( m_data.get(), imageWidth, imageHeight, imageFormat );
				
				// Load the texture to make it available in the future just by using the texture name
				// TODO: INTERGRATE THIS TO OPTIMIZE
				//Ogre::TextureManager::getSingleton().loadImage(imagePath, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, m_image); 
				image_loaded = true; 
			} 
			ifs.close(); 
		} 
		return image_loaded; 
	}

	/**
	* @brief Creates an image from a file. The image loaded can be modified afterwards.
	*
	* @note The image file specified should be placed on the data directory of the application
	*
	* @note Supported image formats are: .bmp, .jpg, .gif, .raw, .png, .tga and .dds.
	*
	* @param path  Path of the file to be loaded. It can be relative to the data folder, or absolute.
	* @param path  SceneManager container
	*/
	bool Image::load( const std::string& path, Ogre::SceneManager* sm )
	{
		// Check application correctly initialized (could not be if the user didn't calle size function)
		Application::getSingleton().checkSubsystemsInit();

		// If the image has already been initialized -> release it first
		//if ( isValid () )
		//	end();

		// Build absolute path
		m_path = path;
		if ( isPathAbsolute( m_path ) == false )
		{	
			m_path = dataFolder + m_path;
		}

		// Confirm that it exists
		if ( fileExists( m_path ) == false )
		{
			LOG_ERROR( "The image file %s could not be found", m_path.c_str() );
			return false;
		}

		// Load the image from disk
		std::string basePath, fileName;
		splitFilename( m_path, fileName, basePath );
		bool loaded = loadImageFromDisk( m_path );
		if ( !loaded )
		{
			LOG_ERROR( "The image %s could not be loaded at %s", path.c_str(), m_path.c_str() );
			return false;
		}

		// This file has been loaded from file
		m_loadedFromFile = true;

		// Check if image was loaded ok
		if ( m_image.getData() )
			LOG( "Image succesfully loaded: %s ", m_path.c_str() );
		else
		{
			LOG( "Error loading Image %s", m_path.c_str() );
			return false;
		}

		// Store image format (for images loaded from file, we force it to be RGB or RGBA)
		m_format = toCingFormat( m_image.getFormat() );

		// Create the image
		m_nChannels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)m_format );
		
		// Check if we need to flip the channels red and green (for example png and jpgs are loaded as BGR by Ogre)
		//m_loadedFormat = (GraphicsType)m_image.getFormat();
		//if ( (m_loadedFormat == BGR) || (m_loadedFormat == BGRA) )
		//{
		//	// No alpha channel
		//	if ( m_nChannels == 3 )
		//	{
		//		cv::cvtColor( imgData, m_cvImage, CV_RGB2BGR );
		//	}
		//	// Now, we have an image with Alpha channel -> where is the alpha? (first or last byte)
		//	// Alpha is last
		//	else if (m_image.getFormat() == Ogre::PF_B8G8R8A8 )
		//	{
		//		cv::cvtColor( imgData, m_cvImage, CV_RGBA2BGRA );
		//	}
		//	// Alpha is first byte
		//	else if (m_image.getFormat() == Ogre::PF_A8R8G8B8 )
		//	{
		//		// Flip Red and Blue channels leaving Apha and Green where they are
		//		//int from_to[] = { 0,0,  1,3,  2,2,  3,1 };
		//		int from_to[] = { 0,2,  1,1,  2,0,  3,3 };
		//		cv::mixChannels( &imgData, 1, &m_cvImage, 1, from_to, 4 );
		//	}

		//}// Format not "under control" -> not touching byte order
		//else
		//{
		//	LOG( "Image::load. Format not \"under control\". Not altering byte order, colors could be swapped" );
		//	imgData.copyTo(m_cvImage);
		//}

		//imgData.release();

		if ( sm ==NULL )
		{
			// Create the texture quad (to draw image) or reset its width and height
			if ( !isValid () )
				m_quad.init( (int)m_image.getWidth(), (int)m_image.getHeight(), m_format  );
			else if ( (int)m_image.getWidth() != (int)m_quad.getTextWidth() || (int)m_image.getHeight() != m_quad.getTextHeight() )
				m_quad.reset( (int)m_image.getWidth(), (int)m_image.getHeight(), m_format );
		}else{
			// Create the texture quad (to draw image) or reset its width and height
			if ( !isValid () )
				m_quad.init( (int)m_image.getWidth(), (int)m_image.getHeight(), m_format, DYNAMIC_WRITE_ONLY_DISCARDABLE, sm  );
			else if ( (int)m_image.getWidth() != (int)m_quad.getTextWidth() || (int)m_image.getHeight() != m_quad.getTextHeight() )
				m_quad.reset( (int)m_image.getWidth(), (int)m_image.getHeight(), m_format );
		}


		// Load image data to texture
		updateTexture();

		// The class is now initialized
		m_bIsValid = true;
		m_bUpdateTexture = false;

		return true;
		
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
		// Store the desired path for the new image
		std::string savePath = name;

		// Check the path: if it's relative, it should be relative to the data folder, if it's absolute don't change it
		if ( isPathAbsolute( savePath ) == false  )
			savePath = ResourceManager::userDataPath + name;

		// Make sure the path exists
		std::string folder, fileName;
		splitFilename( savePath, fileName, folder );
		if ( !folderExists(folder) )
		{
			LOG_ERROR( "Image::save. Error, cannot save image to %s. Does folder %s exist?", savePath.c_str(), folder.c_str() );
			return;
		}

		// Folder exists, Save image
		LOG( "Saving image in %s", savePath.c_str() );
		m_image.save( savePath );
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
		Ogre::SceneManager* sceneManager = getTexturedQuad().getSceneManager();
		if ( sceneManager == NULL )
			return;

		//Release opencv Mat
		m_image.freeMemory();

		// Release the quad
		m_quad.end();

		// The class is not valid anymore
		m_bIsValid = false;

	}

	/**
	* @brief Sets the data of the image
	*
	* @param imageData Data to set to the image
	* @param width		Width of the passed image data
	* @param height		Height of the passed image data
	* @param format		format Format of the image passed
	* @param widthStep	optional parameter in case the width step is not width*nChannels. Usually you should not use it.
	*/
	void Image::setData( const unsigned char* imageData, int width /*= -1*/, int height /*= -1*/, GraphicsType format /*= UNDEFINED*/, int widthStep /*= -1*/ )
	{
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to set data to an invalid image (it has not been initialized)" );
			return;
		}

		// if the width/height are not specified, assume the are the same as this image
		if ( width == -1 )
			width = getWidth();
		if ( height == -1 )
			height = getHeight();

		// if the format is not specified, assume is the same
		if ( format == UNDEFINED )
			format = m_format;

		// Check dimensions
		int inChannels		= (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );
		int currentChannels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)m_image.getFormat() );
		if ( (width != m_image.getWidth()) || (height != m_image.getHeight()) || (inChannels != currentChannels) )
		{
			LOG_ERROR( "Trying to set data with a wrong size of number of channels" );
			return;
		}

		// Set the data
		if ( widthStep == -1 )
			widthStep = width*inChannels;
		
		// NOTE: this cast is due to Ogre::Image.loadDynamicImage not receiving a const pointer when it should
		unsigned char* data = const_cast<unsigned char*>(imageData);

		// Load data into our image
		m_image.loadDynamicImage( data, width, height, m_image.getFormat() );

		// Make the image to be updated to texture in the next draw
		m_bUpdateTexture = true;
	}

	/**
	 * @brief Returns the image data (buffer)
	 * @return the image data (buffer)
	 */	
	unsigned char* Image::getData() 
	{ 
		return isValid()? m_image.getData(): NULL; 
	}

	/**
	 * @brief Returns the image data (buffer) - const version
	 * @return the image data (buffer)
	 */	
	const unsigned char* Image::getData() const
	{ 
		return isValid()? m_image.getData(): NULL; 
	}

	/**
	* @brief Returns image width
	* @return the height of the image
	*/
	int Image::getWidth() const
	{
		return m_image.getWidth();
	}

	/**
	* @brief Returns image height
	* @return the height of the image
	*/
	int Image::getHeight() const
	{
		return m_image.getHeight();
	}

	/**
	* @brief Returns image format
	* @return the imageFomat of the image
	*/
	GraphicsType Image::getFormat() const
	{
		// Return the stored format
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
		x = constrain(x, 0, m_image.getWidth()-1);
		y = constrain(y, 0, m_image.getHeight()-1);

		// Read color
		return m_image.getColourAt(x, y, 1);
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
	 * @brief Flips the pixels vertically 
	 * @note This is slower as it physically flips the image pixels. You can just call scale(1, -1) for example to flip vertically before drawing the image, instead
	 * of flipping the pixel content. You can also call flip in the TexturedQuad, which wil just flip the texture coordinates of the quad used to draw the image.
	 */
	void Image::flipVertical()
	{
		m_image.flipAroundY();
	}

	/**
	 * @brief Flips the pixels horizontally
	 * @note This is slower as it physically flips the image pixels. You can just call scale(-1, 1) for example to flip horizontally before drawing the image, instead
	 * of flipping the pixel content. You can also call flip in the TexturedQuad, which wil just flip the texture coordinates of the quad used to draw the image.
	 */
	void Image::flipHorizontal()
	{
		m_image.flipAroundX();
	}

	/**
	* @brief Set if texture updates automatically every frame
	*/
	void Image::setUpdateTexture(bool updateTextureFlag /*= true*/)
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

		// Get the scene manager of the other iamge's texture quad so that both images are in the same scene manager.
		Ogre::SceneManager* sm = ( const_cast< Cing::Image* > ( &other ) )->getTexturedQuad().getSceneManager();

		// Check if the image is initialized -> if not initialize with "other"'s format and size
		if ( !isValid() )
		{
			init( other.getWidth(), other.getHeight(), other.getFormat(), sm );
		}

		// Check if the size of the image differs or the number of channels... just recreate it
		// TODO: optimize this
		if ( (other.getWidth() != getWidth()) || (other.getHeight() != getHeight()) || (other.getNChannels() != getNChannels()) )
		{
			LOG( "Image will be recreated as the size of both images differ" );
			end();
			init( other.getWidth(), other.getHeight(), other.getFormat(), sm );			
		}
		// Looks like we have the same number of channels, just copy over
		else
		{
			setData( other.getData(), getWidth(), getHeight(), getFormat() );
		}

		// Load image data to texture in next draw
		setUpdateTexture();

		// Now this image is valid
		m_bIsValid = true;
	}

	/**
	* @brief  Equality comparison operator, returns a boolean.
	*
	* @param other Image to compare
	*/
	bool Image::operator==(const Image& other) const 
	{
		// First check format
		if( getWidth() != other.getWidth() || getHeight() != other.getHeight() || getNChannels() != other.getNChannels() )
		{
			return false;
		}
		// if still equal compare at a pixel level
		else
		{
			if( 0 == memcmp( m_image.getData(), other.m_image.getData(), other.getWidth() * other.getHeight() * other.getNChannels() ) )
				return true;
			else
				return false;
		}
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
	void Image::setOrientation( const Vector& axis, float angleRadians )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to rotate an invalid image" );

		m_quad.setOrientation( axis, angleRadians );
	}

	/**
	* @brief Sets the rotation of the image (in relation to its parents)
	*
	* @param[in] orientation quaternion containing the orientation information
	*/
	void Image::setOrientation( const Quaternion& orientation )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to rotate an invalid image" );

		m_quad.setOrientation( orientation );
	}

	/**
	* @brief Adds rotation of the image (in relation to its parents)
	*
	* @param[in] axis	rotation axis
	* @param[in] angle rotation angle (degrees)
	*/
	void Image::rotate( const Vector& axis, float angleRadians )
	{
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to rotate an invalid image" );

		m_quad.rotate( axis, angleRadians );
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
		m_quad.updateTexture( m_image.getData(), getWidth(), getHeight(), getFormat() );
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
	//	// Check the image is valid
	//	if ( !isValid() )
	//		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	//	// Get Stroke and Fill Color
	//	GraphicsManager& graphManager	= GraphicsManager::getSingleton();
	//	Color strokeColor				= graphManager.getStrokeColor();
	//	Color fillColor					= graphManager.getFillColor();
	//	int   strokeWeight				= graphManager.getStrokeWeight();

	//	// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
	//	// as opencv does not support transparent drawing
	//	cv::Mat* canvasImage = &m_cvImage;
	//	cv::Mat* alphaCanvasImage = NULL;

	//	// If there is transparency involved
	//	if ( (graphManager.getStroke() && (strokeColor.a < 255)) || ( graphManager.getFill() && (fillColor.a < 255) ) )
	//	{
	//		// Request a temporary image to draw the transparent shape
	//		alphaCanvasImage = ImageResourceManager::getSingleton().getImage( getWidth(), getHeight(), getNChannels() );
	//	
	//		// Fill it with the current image content
	//		m_cvImage.copyTo( *alphaCanvasImage );

	//		// The canvas will be the temp image to draw on it and later on blend it with the current cv image
	//		canvasImage = alphaCanvasImage;

	//	}

	//	// Draw Fill
	//	if (graphManager.getFill())
	//	{
	//		// Get Fill Color
	//		Color color        = graphManager.getFillColor();
	//		cv::Point pts[3] = { cv::Point(x1,y1), cv::Point(x2,y2), cv::Point(x3,y3)};

	//		if (graphManager.getSmooth())
	//			cv::fillConvexPoly( *canvasImage, (const cv::Point*)&pts, 3, cv::Scalar( color.r, color.g, color.b ), CV_AA, 0 );
	//		else
	//			cv::fillConvexPoly( *canvasImage, (const cv::Point*)&pts, 3, cv::Scalar( color.r, color.g, color.b ), 4, 0 );
	//	}

	//	// Draw stroke
	//	if (graphManager.getStroke())
	//	{
	//		if (graphManager.getSmooth())
	//		{
	//			cv::line( *canvasImage, cv::Point(x1,y1), cv::Point(x2,y2), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
	//			cv::line( *canvasImage, cv::Point(x2,y2), cv::Point(x3,y3), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
	//			cv::line( *canvasImage, cv::Point(x3,y3), cv::Point(x1,y1), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
	//		}else{																														 
	//			cv::line( *canvasImage, cv::Point(x1,y1), cv::Point(x2,y2), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
	//			cv::line( *canvasImage, cv::Point(x2,y2), cv::Point(x3,y3), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
	//			cv::line( *canvasImage, cv::Point(x3,y3), cv::Point(x1,y1), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
	//		}																														 
	//	}																															  	

	//	// If there is transparency involved -> blend the result and release the temp image used for transparency
	//	if ( graphManager.getFill() && (fillColor.a < 255) )
	//	{
	//		cv::addWeighted( m_cvImage, (fillColor.getHighRange()-fillColor.a)/fillColor.getHighRange(), *canvasImage, fillColor.a/fillColor.getHighRange(), 0, m_cvImage );
	//		ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );

	//		// Note: Now theres is a limitation (opencv does not support transparency in the drawing API)
	//		// So we limit it to only 1 kind (fill or stroke), as we fake it blending the whole drawing with the current image, which is slow.
	//		// This warning is to nofity it
	//		if ( graphManager.getStroke() && (strokeColor.a < 255) )
	//			LOG_ERROR_NTIMES( 3, "Trying to draw with transparency in both fill and stroke is not supported. Only fill transparency will be applied" );

	//	}
	//	else if ( graphManager.getStroke() && (strokeColor.a < 255) )
	//	{
	//		cv::addWeighted( m_cvImage, (strokeColor.getHighRange()-strokeColor.a)/strokeColor.getHighRange(), *canvasImage, strokeColor.a/strokeColor.getHighRange(), 0, m_cvImage );
	//		ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
	//	}

	//	// Update texture when the next drawing call is made by the user															  
	//	m_bUpdateTexture = true;
	}

	///**
	//* @brief Draws a line inside an image
	//*
	//* @param x1 x, first point
	//* @param y1 y, first point
	//* @param x2 x, end point
	//* @param y2 y, end point
	//*/
	void Image::line( int x1, int y1, int x2, int y2 )
	{
	//	// Check the image is valid
	//	if ( !isValid() )
	//		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	//	// Get Stroke and Fill Color
	//	GraphicsManager& graphManager = GraphicsManager::getSingleton();
	//	Color color        = graphManager.getStrokeColor();
	//	int   strokeWeight = graphManager.getStrokeWeight();

	//	// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
	//	// as opencv does not support transparent drawing
	//	cv::Mat* canvasImage = &m_cvImage;
	//	cv::Mat* alphaCanvasImage = NULL;

	//	// If there is transparency involved
	//	if ( color.a < 255 )
	//	{
	//		// Request a temporary image to draw the transparent shape
	//		alphaCanvasImage = ImageResourceManager::getSingleton().getImage( getWidth(), getHeight(), getNChannels() );
	//	
	//		// Fill it with the current image content
	//		m_cvImage.copyTo( *alphaCanvasImage );

	//		// The canvas will be the temp image to draw on it and later on blend it with the current cv image
	//		canvasImage = alphaCanvasImage;

	//	}

	//	// Draw 
	//	if (graphManager.getSmooth())
	//	{
	//		cv::line(	*canvasImage,
	//					cv::Point(x1,y1),
	//					cv::Point(x2,y2),
	//					cv::Scalar( color.r, color.g, color.b ),
	//					strokeWeight,	///< Thickness.
	//					CV_AA,			///< Type of the ellipse boundary, see cvLine description.
	//					0);				///< Number of fractional bits in the center coordinates and axes' values.
	//	}else{
	//		cv::line(	*canvasImage,
	//					cv::Point(x1,y1),
	//					cv::Point(x2,y2),
	//					cv::Scalar( color.r, color.g, color.b ),
	//					strokeWeight,	///< Thickness.
	//					4,				///< Type of the ellipse boundary, see cvLine description.
	//					0); 			///< Number of fractional bits in the center coordinates and axes' values.
	//	}

	//	// If there is transparency involved -> blend the result
	//	if ( color.a < 255 )
	//	{
	//		// Blend the result
	//		cv::addWeighted( m_cvImage, (color.getHighRange()-color.a)/color.getHighRange(), *canvasImage, color.a/color.getHighRange(), 0, m_cvImage );

	//		// Release temp image
	//		ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
	//	}

	//	// Update texture when the next drawing call is made by the user
	//	m_bUpdateTexture = true;
	}

	/**
	* @brief Draws an arc inside an image. Arcs are drawn along the outer edge of an ellipse defined by the x, y,
	*				 width and height parameters. The start and stop parameters specify the angles at which to draw the arc.
	*
	* @param x x, first point
	* @param y y, first point
	* @param width  width
	* @param height height
	* @param start start angle (in radians)
	* @param end end angle of the arc(in radians)
	*/
	void Image::arc( int x, int y, int width, int height, float start, float end )
	{
	//	// Check the image is valid
	//	if ( !isValid() )
	//		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	//	// Convert from Radians to degrees(to keep compatibility with processing)
	//	// TODO: should keep the 360 flip only in Processing Mode?
	//	start	= 360 - degrees(start);
	//	end		= 360 - degrees(end);

	//	// Get Stroke and Fill Color
	//	GraphicsManager& graphManager	= GraphicsManager::getSingleton();
	//	Color strokeColor				= graphManager.getStrokeColor();
	//	Color fillColor					= graphManager.getFillColor();
	//	int   strokeWeight				= graphManager.getStrokeWeight();

	//	// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
	//	// as opencv does not support transparent drawing
	//	cv::Mat* canvasImage = &m_cvImage;
	//	cv::Mat* alphaCanvasImage = NULL;

	//	// If there is transparency involved
	//	if ( (graphManager.getStroke() && (strokeColor.a < 255)) || ( graphManager.getFill() && (fillColor.a < 255) ) )
	//	{
	//		// Request a temporary image to draw the transparent shape
	//		alphaCanvasImage = ImageResourceManager::getSingleton().getImage( getWidth(), getHeight(), getNChannels() );
	//	
	//		// Fill it with the current image content
	//		m_cvImage.copyTo( *alphaCanvasImage );

	//		// The canvas will be the temp image to draw on it and later on blend it with the current cv image
	//		canvasImage = alphaCanvasImage;

	//	}

	//	if (graphManager.getFill())
	//	{
	//		if (graphManager.getSmooth())
	//		{
	//			cv::ellipse(	*canvasImage,					///-> Image.
	//							cv::Point(x,y),					///-> Center of the ellipse.
	//							cvSize(width/2,height/2),	///-> Length of the ellipse axes.
	//							0,								///->	Rotation angle.
	//							start,							///-> Starting angle of the elliptic arc.
	//							end,							///-> Ending angle of the elliptic arc.
	//							cv::Scalar(fillColor.r, fillColor.g, fillColor.b),///-> Ellipse color.
	//							-1,
	//							CV_AA);							///-> Thickness of the ellipse arc.
	//		}else{
	//			cv::ellipse(	*canvasImage,					///-> Image.
	//							cv::Point(x,y),					///-> Center of the ellipse.
	//							cvSize(width/2,height/2),	///-> Length of the ellipse axes.
	//							0,								///->	Rotation angle.
	//							start,							///-> Starting angle of the elliptic arc.
	//							end,							///-> Ending angle of the elliptic arc.
	//							cv::Scalar(fillColor.r, fillColor.g, fillColor.b),///-> Ellipse color.
	//							-1,
	//							4);								///-> Thickness of the ellipse arc.
	//		}
	//	}

	//	if (graphManager.getStroke())
	//	{			
	//		if (graphManager.getSmooth())
	//		{
	//			cv::ellipse(	*canvasImage,					///-> Image.
	//							cv::Point(x,y),					///-> Center of the ellipse.
	//							cvSize(width/2,height/2),///-> Length of the ellipse axes.
	//							0,								///->	Rotation angle.
	//							start,							///-> Starting angle of the elliptic arc.
	//							end,							///-> Ending angle of the elliptic arc.
	//							cv::Scalar(strokeColor.r, strokeColor.g, strokeColor.b ),///-> Ellipse color.
	//							strokeWeight,
	//							CV_AA );								///-> Thickness of the ellipse arc.
	//		}else{
	//			cv::ellipse(	*canvasImage,					///-> Image.
	//							cv::Point(x,y),					///-> Center of the ellipse.
	//							cvSize(width/2,height/2),	///-> Length of the ellipse axes.
	//							0,								///->	Rotation angle.
	//							start,							///-> Starting angle of the elliptic arc.
	//							end,							///-> Ending angle of the elliptic arc.
	//							cv::Scalar(strokeColor.r, strokeColor.g, strokeColor.b ),///-> Ellipse color.
	//							strokeWeight,
	//							4 );							///-> Thickness of the ellipse arc.

	//		}
	//	}

	//	// If there is transparency involved -> blend the result and release the temp image used for transparency
	//	if ( graphManager.getFill() && (fillColor.a < 255) )
	//	{
	//		cv::addWeighted( m_cvImage, (fillColor.getHighRange()-fillColor.a)/fillColor.getHighRange(), *canvasImage, fillColor.a/fillColor.getHighRange(), 0, m_cvImage );
	//		ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );

	//		// Note: Now theres is a limitation (opencv does not support transparency in the drawing API)
	//		// So we limit it to only 1 kind (fill or stroke), as we fake it blending the whole drawing with the current image, which is slow.
	//		// This warning is to nofity it
	//		if ( graphManager.getStroke() && (strokeColor.a < 255) )
	//			LOG_ERROR_NTIMES( 3, "Trying to draw with transparency in both fill and stroke is not supported. Only fill transparency will be applied" );

	//	}
	//	else if ( graphManager.getStroke() && (strokeColor.a < 255) )
	//	{
	//		cv::addWeighted( m_cvImage, (strokeColor.getHighRange()-strokeColor.a)/strokeColor.getHighRange(), *canvasImage, strokeColor.a/strokeColor.getHighRange(), 0, m_cvImage );
	//		ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
	//	}

	//	// Update texture when t:he next drawing call is made by the user
	//	m_bUpdateTexture = true;
	}

	/**
	* @brief Draws a point ( one pixel )
	*
	* @param x x, point
	* @param y y, point
	*/
	void Image::point( int x, int y )
	{

	//	// Check the image is valid
	//	if ( !isValid() )
	//		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	//	// Get Stroke and Fill Color
	//	GraphicsManager& graphManager	= GraphicsManager::getSingleton();
	//	Color strokeColor				= graphManager.getStrokeColor();
	//	int   strokeWeight				= graphManager.getStrokeWeight();

	//	// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
	//	// as opencv does not support transparent drawing
	//	cv::Mat* canvasImage = &m_cvImage;
	//	cv::Mat* alphaCanvasImage = NULL;

	//	// If there is transparency involved
	//	if ( strokeColor.a < 255 )
	//	{
	//		// Request a temporary image to draw the transparent shape
	//		alphaCanvasImage = ImageResourceManager::getSingleton().getImage( getWidth(), getHeight(), getNChannels() );
	//	
	//		// Fill it with the current image content
	//		m_cvImage.copyTo( *alphaCanvasImage );

	//		// The canvas will be the temp image to draw on it and later on blend it with the current cv image
	//		canvasImage = alphaCanvasImage;

	//	}

	//	// Draw a pixel
	//	cv::rectangle(  *canvasImage,
	//					cv::Point(x,y),
	//					cv::Point(x,y),
	//					cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ),
	//					strokeWeight);		///-> Thickness.


	//	// If there is transparency involved -> blend the result
	//	if ( strokeColor.a < 255 )
	//	{
	//		// Blend the result
	//		cv::addWeighted( m_cvImage, (strokeColor.getHighRange()-strokeColor.a)/strokeColor.getHighRange(), *canvasImage, strokeColor.a/strokeColor.getHighRange(), 0, m_cvImage );

	//		// Release temp image
	//		ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
	//	}
	//	// Update texture when the next drawing call is made by the user
	//	m_bUpdateTexture = true;
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
	//	// Check the image is valid
	//	if ( !isValid() )
	//		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	//	// Get Stroke and Fill Color
	//	GraphicsManager& graphManager	= GraphicsManager::getSingleton();
	//	Color strokeColor				= graphManager.getStrokeColor();
	//	Color fillColor					= graphManager.getFillColor();
	//	int   strokeWeight				= graphManager.getStrokeWeight();

	//	// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
	//	// as opencv does not support transparent drawing
	//	cv::Mat* canvasImage = &m_cvImage;
	//	cv::Mat* alphaCanvasImage = NULL;

	//	// If there is transparency involved
	//	if ( (graphManager.getStroke() && (strokeColor.a < 255)) || ( graphManager.getFill() && (fillColor.a < 255) ) )
	//	{
	//		// Request a temporary image to draw the transparent shape
	//		alphaCanvasImage = ImageResourceManager::getSingleton().getImage( getWidth(), getHeight(), getNChannels() );
	//	
	//		// Fill it with the current image content
	//		m_cvImage.copyTo( *alphaCanvasImage );

	//		// The canvas will be the temp image to draw on it and later on blend it with the current cv image
	//		canvasImage = alphaCanvasImage;
	//	}

	//	// Draw fill
	//	if (graphManager.getFill())
	//	{
	//		cv::Point pts[4]= { cv::Point(x1,y1), cv::Point(x2,y2), cv::Point(x3,y3), cv::Point(x4,y4) };

	//		if (graphManager.getSmooth())
	//			cv::fillConvexPoly( *canvasImage , (const cv::Point*)&pts, 4,	cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), CV_AA, 0 );
	//		else
	//			cv::fillConvexPoly( *canvasImage , (const cv::Point*)&pts, 4,	cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), 4, 0 );
	//	}

	//	// Draw stroke
	//	if (graphManager.getStroke())
	//	{
	//		if (graphManager.getSmooth())
	//		{
	//			cv::line( *canvasImage, cv::Point(x1,y1), cv::Point(x2,y2), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
	//			cv::line( *canvasImage, cv::Point(x2,y2), cv::Point(x3,y3), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
	//			cv::line( *canvasImage, cv::Point(x3,y3), cv::Point(x4,y4), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
	//			cv::line( *canvasImage, cv::Point(x4,y4), cv::Point(x1,y1), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
	//																																	 
	//		}else{																														 
	//																																	 
	//			cv::line( *canvasImage, cv::Point(x1,y1), cv::Point(x2,y2), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
	//			cv::line( *canvasImage, cv::Point(x2,y2), cv::Point(x3,y3), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
	//			cv::line( *canvasImage, cv::Point(x3,y3), cv::Point(x4,y4), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
	//			cv::line( *canvasImage, cv::Point(x4,y4), cv::Point(x1,y1), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
	//		}																														 
	//	}																															 
	//		
	//	// If there is transparency involved -> blend the result and release the temp image used for transparency
	//	if ( graphManager.getFill() && (fillColor.a < 255) )
	//	{
	//		cv::addWeighted( m_cvImage, (fillColor.getHighRange()-fillColor.a)/fillColor.getHighRange(), *canvasImage, fillColor.a/fillColor.getHighRange(), 0, m_cvImage );
	//		ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );

	//		// Note: Now theres is a limitation (opencv does not support transparency in the drawing API)
	//		// So we limit it to only 1 kind (fill or stroke), as we fake it blending the whole drawing with the current image, which is slow.
	//		// This warning is to nofity it
	//		if ( graphManager.getStroke() && (strokeColor.a < 255) )
	//			LOG_ERROR_NTIMES( 3, "Trying to draw with transparency in both fill and stroke is not supported. Only fill transparency will be applied" );

	//	}
	//	else if ( graphManager.getStroke() && (strokeColor.a < 255) )
	//	{
	//		cv::addWeighted( m_cvImage, (strokeColor.getHighRange()-strokeColor.a)/strokeColor.getHighRange(), *canvasImage, strokeColor.a/strokeColor.getHighRange(), 0, m_cvImage );
	//		ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
	//	}

	//	// Update texture when the next drawing call is made by the user															  
	//	m_bUpdateTexture = true;																									  
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
	//	// Check the image is valid
	//	if ( !isValid() )
	//		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	//	// Calcuate half sizes
	//	float widthDIV2 = (float)width/2.0f;
	//	float heightDIV2 = (float)height/2.0f;

	//	// Get Stroke and Fill Color
	//	GraphicsManager& graphManager	= GraphicsManager::getSingleton();
	//	Color strokeColor				= graphManager.getStrokeColor();
	//	Color fillColor					= graphManager.getFillColor();
	//	int   strokeWeight				= graphManager.getStrokeWeight();

	//	// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
	//	// as opencv does not support transparent drawing
	//	cv::Mat* canvasImage = &m_cvImage;
	//	cv::Mat* alphaCanvasImage = NULL;

	//	// If there is transparency involved
	//	if ( (graphManager.getStroke() && (strokeColor.a < 255)) || ( graphManager.getFill() && (fillColor.a < 255) ) )
	//	{
	//		// Request a temporary image to draw the transparent shape
	//		alphaCanvasImage = ImageResourceManager::getSingleton().getImage( getWidth(), getHeight(), getNChannels() );
	//	
	//		// Fill it with the current image content
	//		m_cvImage.copyTo( *alphaCanvasImage );

	//		// The canvas will be the temp image to draw on it and later on blend it with the current cv image
	//		canvasImage = alphaCanvasImage;
	//	}

	//	// Draw fill
	//	if (graphManager.getFill())
	//	{
	//		switch( graphManager.getRectMode() )
	//		{

	//		case CORNER:
	//			if (graphManager.getSmooth())
	//				cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(x+width,y+height), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
	//			else
	//				cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(x+width,y+height), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
	//			break;

	//		case CORNERS:
	//			if (graphManager.getSmooth())
	//				cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(width,height), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
	//			else
	//				cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(width,height), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
	//			break;

	//		case CENTER:
	//			if (graphManager.getSmooth())
	//				cv::rectangle( *canvasImage, cv::Point(x-(int)widthDIV2,y-(int)heightDIV2), cv::Point(x+(int)widthDIV2,y+(int)heightDIV2), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
	//			else
	//				cv::rectangle( *canvasImage, cv::Point(x-(int)widthDIV2,y-(int)heightDIV2), cv::Point(x+(int)widthDIV2,y+(int)heightDIV2), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
	//			break;

	//		case RADIUS:
	//			if (graphManager.getSmooth())
	//				cv::rectangle( *canvasImage, cv::Point(x-width,y-height), cv::Point(x+width,y+height), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
	//			else
	//				cv::rectangle( *canvasImage, cv::Point(x-width,y-height), cv::Point(x+width,y+height), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
	//			break;
	//		}
	//	}

	//	// Draw Stroke
	//	if (graphManager.getStroke())
	//	{
	//		switch( graphManager.getRectMode() )
	//		{

	//		case CORNER:
	//			if (graphManager.getSmooth())
	//				cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(x+width,y+height), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
	//			else
	//				cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(x+width,y+height), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
	//			break;

	//		case CORNERS:
	//			if (graphManager.getSmooth())
	//				cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(width,height), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
	//			else
	//				cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(width,height), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
	//			break;

	//		case CENTER:
	//			if (graphManager.getSmooth())
	//				cv::rectangle( *canvasImage, cv::Point(x-(int)widthDIV2,y-(int)heightDIV2), cv::Point(x+(int)widthDIV2,y+(int)heightDIV2), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
	//			else
	//				cv::rectangle( *canvasImage, cv::Point(x-(int)widthDIV2,y-(int)heightDIV2), cv::Point(x+(int)widthDIV2,y+(int)heightDIV2), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
	//			break;

	//		case RADIUS:
	//			if (graphManager.getSmooth())
	//				cv::rectangle( *canvasImage, cv::Point(x-width,y-height), cv::Point(x+width,y+height), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
	//			else
	//				cv::rectangle( *canvasImage, cv::Point(x-width,y-height), cv::Point(x+width,y+height), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
	//			break;
	//		}
	//	}

	//	// If there is transparency involved -> blend the result and release the temp image used for transparency
	//	if ( graphManager.getFill() && (fillColor.a < 255) )
	//	{
	//		cv::addWeighted( m_cvImage, (fillColor.getHighRange()-fillColor.a)/fillColor.getHighRange(), *canvasImage, fillColor.a/fillColor.getHighRange(), 0, m_cvImage );
	//		ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );

	//		// Note: Now theres is a limitation (opencv does not support transparency in the drawing API)
	//		// So we limit it to only 1 kind (fill or stroke), as we fake it blending the whole drawing with the current image, which is slow.
	//		// This warning is to nofity it
	//		if ( graphManager.getStroke() && (strokeColor.a < 255) )
	//			LOG_ERROR_NTIMES( 3, "Trying to draw with transparency in both fill and stroke is not supported. Only fill transparency will be applied" );

	//	}
	//	else if ( graphManager.getStroke() && (strokeColor.a < 255) )
	//	{
	//		cv::addWeighted( m_cvImage, (strokeColor.getHighRange()-strokeColor.a)/strokeColor.getHighRange(), *canvasImage, strokeColor.a/strokeColor.getHighRange(), 0, m_cvImage );
	//		ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
	//	}

	//	// Update texture when the next drawing call is made by the user
	//	m_bUpdateTexture = true;
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
	void Image::ellipse( int x1, int y1, int width, int height, float angleDegrees /*= 0*/ )
	{

	//	//TODO: Hay para cambiar de mod deg a rad?

	//	// Check the image is valid
	//	if ( !isValid() )
	//		THROW_EXCEPTION( "Trying to paint in an invalid image" );
	//
	//	// Check scale
	//	if ( (width < 0) || (height < 0) )
	//	{
	//		LOG_ERROR( "Image::ellipse. widht and height should be positive" );
	//		return;
	//	}
	//		
	//	float widthDIV2 = (float)width/2.0f;
	//	float heightDIV2 = (float)height/2.0f;

	//	// Get Stroke and Fill Color
	//	GraphicsManager& graphManager	= GraphicsManager::getSingleton();
	//	Color strokeColor				= graphManager.getStrokeColor();
	//	Color fillColor					= graphManager.getFillColor();
	//	int   strokeWeight				= graphManager.getStrokeWeight();

	//	// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
	//	// as opencv does not support transparent drawing
	//	cv::Mat* canvasImage = &m_cvImage;
	//	cv::Mat* alphaCanvasImage = NULL;

	//	// If there is transparency involved
	//	if ( (graphManager.getStroke() && (strokeColor.a < 255)) || ( graphManager.getFill() && (fillColor.a < 255) ) )
	//	{
	//		// Request a temporary image to draw the transparent shape
	//		alphaCanvasImage = ImageResourceManager::getSingleton().getImage( getWidth(), getHeight(), getNChannels() );
	//	
	//		// Fill it with the current image content
	//		m_cvImage.copyTo( *alphaCanvasImage );

	//		// The canvas will be the temp image to draw on it and later on blend it with the current cv image
	//		canvasImage = alphaCanvasImage;
	//	}


	//	// Draw fill
	//	if (graphManager.getFill())
	//	{
	//		switch( graphManager.getEllipseMode() )
	//		{
	//		case CORNER:
	//			if (graphManager.getSmooth())
	//				cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(x1+width,y1+height), angleDegrees,	0, 360,	cv::Scalar( fillColor.r, fillColor.g, fillColor.b ),-1,CV_AA);
	//			else
	//				cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(x1+width,y1+height), angleDegrees,	0, 360,	cv::Scalar( fillColor.r, fillColor.g, fillColor.b ),-1,4 );
	//			break;

	//		case CORNERS:
	//			if (graphManager.getSmooth())
	//				cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(width,height), angleDegrees,	0, 360, cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
	//			else
	//				cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(width,height), angleDegrees,	0, 360, cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
	//			break;

	//		case CENTER:
	//			if (graphManager.getSmooth())
	//				cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize((int)widthDIV2,(int)heightDIV2), angleDegrees,	0, 360, cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
	//			else
	//				cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize((int)widthDIV2,(int)heightDIV2), angleDegrees,	0, 360, cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
	//			break;

	//		case RADIUS:
	//			if (graphManager.getSmooth())
	//				cv::ellipse( *canvasImage, cv::Point(x1-width,y1-height), cvSize(x1+width,y1+height), angleDegrees,	0, 360, cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
	//			else
	//				cv::ellipse( *canvasImage, cv::Point(x1-width,y1-height), cvSize(x1+width,y1+height), angleDegrees,	0, 360, cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
	//			break;
	//		}
	//	}

	//	// Draw stroke
	//	if (graphManager.getStroke())
	//	{
	//		switch( graphManager.getEllipseMode() )
	//		{

	//		case CORNER:
	//			if (graphManager.getSmooth())
	//				cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(x1+width,y1+height), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
	//			else
	//				cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(x1+width,y1+height), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
	//			break;

	//		case CORNERS:
	//			if (graphManager.getSmooth())
	//				cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(width,height), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
	//			else
	//				cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(width,height), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
	//			break;

	//		case CENTER:
	//			if (graphManager.getSmooth())
	//				cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize((int)widthDIV2,(int)heightDIV2), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
	//			else
	//				cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize((int)widthDIV2,(int)heightDIV2), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
	//			break;

	//		case RADIUS:
	//			if (graphManager.getSmooth())
	//				cv::ellipse( *canvasImage, cv::Point(x1-width,y1-height), cvSize(x1+width,y1+height), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
	//			else
	//				cv::ellipse( *canvasImage, cv::Point(x1-width,y1-height), cvSize(x1+width,y1+height), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
	//			break;
	//		}
	//	}

	//	// If there is transparency involved -> blend the result and release the temp image used for transparency
	//	if ( graphManager.getFill() && (fillColor.a < 255) )
	//	{
	//		cv::addWeighted( m_cvImage, (fillColor.getHighRange()-fillColor.a)/fillColor.getHighRange(), *canvasImage, fillColor.a/fillColor.getHighRange(), 0, m_cvImage );
	//		ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );

	//		// Note: Now theres is a limitation (opencv does not support transparency in the drawing API)
	//		// So we limit it to only 1 kind (fill or stroke), as we fake it blending the whole drawing with the current image, which is slow.
	//		// This warning is to nofity it
	//		if ( graphManager.getStroke() && (strokeColor.a < 255) )
	//			LOG_ERROR_NTIMES( 3, "Trying to draw with transparency in both fill and stroke is not supported. Only fill transparency will be applied" );

	//	}
	//	else if ( graphManager.getStroke() && (strokeColor.a < 255) )
	//	{
	//		cv::addWeighted( m_cvImage, (strokeColor.getHighRange()-strokeColor.a)/strokeColor.getHighRange(), *canvasImage, strokeColor.a/strokeColor.getHighRange(), 0, m_cvImage );
	//		ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
	//	}

	//	// Update texture when the next drawing call is made by the user
	//	m_bUpdateTexture = true;

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
	//	// Check the image is valid
	//	if ( !isValid() )
	//		THROW_EXCEPTION( "Trying to paint in an invalid image" );

	//	// Get Stroke and Fill Color
	//	GraphicsManager& graphManager = GraphicsManager::getSingleton();
	//	Color color			= graphManager.getStrokeColor();
	//	int   strokeWeight	= graphManager.getStrokeWeight();
	//	int	  fontScale		= 2;

	//	cv::putText(m_cvImage, text, cv::Point(x1,y1), cv::FONT_HERSHEY_SIMPLEX, fontScale, cv::Scalar( color.r, color.g, color.b, color.a ));

	//	// Update texture when the next drawing call is made by the user
	//	m_bUpdateTexture = true;
	}

	/**
	* @brief Method to apply a variety of basic filters to this image.
	*
	* @param ImageProcessingFilters type
	* @param int argument fot the filter that will be applied
	*/
	void Image::filter( ImageProcessingFilters type, float param )
	{
		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );

		if ( render2DBackend )
			render2DBackend->filter( *this, type, param );

		// Update texture when the next drawing call is made by the user
		m_bUpdateTexture = true;
	}


	/**
	* @brief Converts an image from GRAYSCALE to Color format
	* TODO: optimize
	*/
	void Image::toColor()
	{
		//// Check valid
		//if ( !isValid() )
		//{
		//	LOG_ERROR( "Trying to convert to color an invalid image. Maybe it is not initialized" );
		//	return;
		//}

		//// Check current format
		//if ( m_nChannels >=3 )
		//	return;

		//// Image to store data temporarily
		//cv::Mat* tempImage = ImageResourceManager::getSingleton().getImage( getWidth(), getHeight(), 3 );

		//// Convert image
		//cv::cvtColor( m_cvImage, *tempImage, CV_GRAY2RGB );

		//// Re-init the image with the new format and se the color data
		//end();
		//init( tempImage->cols, tempImage->rows, RGB );
		//setData( (unsigned char*)tempImage->data, tempImage->cols, tempImage->rows, RGB );

		//// Check if the image was v flipped
		//if ( m_bVFlipped )
		//	m_quad.flipVertical();

		//// Release temp image
		//ImageResourceManager::getSingleton().releaseImage( tempImage );

		//// Now this image is valid
		//m_bIsValid = true;
	}


	/**
	* @brief Converts an image from Color (RGB) to GRAYSCALE
	* TODO: optimize
	*/
	void Image::toGray()
	{
		//// Check valid
		//if ( !isValid() )
		//{
		//	LOG_ERROR( "Trying to convert to gray an invalid image. Maybe it is not initialized" );
		//	return;
		//}

		//// Check current format, if it is already grayscale -> do nothing
		//if ( m_nChannels == 1 )
		//	return;

		//// Image Store data temporarily
		//cv::Mat* tempImage = ImageResourceManager::getSingleton().getImage( getWidth(), getHeight(), 1 );

		//// Convert image
		//cv::cvtColor( m_cvImage, *tempImage, CV_RGB2GRAY );

		//// Re-init the image with the new format and se the grayscale data
		//end();
		//init( tempImage->cols, tempImage->rows, GRAYSCALE );
		//setData( (unsigned char*)tempImage->data, tempImage->cols, tempImage->rows, GRAYSCALE );

		//// Check if the image was v flipped
		//if ( m_bVFlipped )
		//	m_quad.flipVertical();

		//// Release temp image
		//ImageResourceManager::getSingleton().releaseImage( tempImage );

		//// Now this image is valid
		//m_bIsValid = true;
	}

	void Image::operator +=	( float scalar )
	{
		//switch( m_cvImage.channels() )
		//{
		//case 1:
		//	cv::add( m_cvImage, cv::Scalar(scalar), m_cvImage );
		//	break;
		//case 3:
		//	cv::add( m_cvImage, cv::Scalar(scalar,scalar,scalar), m_cvImage );
		//	break;
		//case 4:
		//	cv::add( m_cvImage, cv::Scalar(scalar,scalar,scalar,scalar), m_cvImage );
		//	break;
		//default:
		//	THROW_EXCEPTION( "Invalid number of channels in image" )
		//		break;
		//}
	}

	void Image::operator -=	( float scalar ){
		//switch( m_cvImage.channels() )
		//{
		//case 1:
		//	cv::subtract( m_cvImage, cv::Scalar(scalar), m_cvImage );
		//	break;
		//case 3:
		//	cv::subtract( m_cvImage, cv::Scalar(scalar,scalar,scalar), m_cvImage );
		//	break;
		//case 4:
		//	cv::subtract( m_cvImage, cv::Scalar(scalar,scalar,scalar,scalar), m_cvImage );
		//	break;
		//default:
		//	THROW_EXCEPTION( "Invalid number of channels in image" )
		//		break;
		//}
	}

	void Image::operator +=	( const Image& img ){

		//cv::add( m_cvImage, img.getCVMat(), m_cvImage );
		//setUpdateTexture(true);
	}

	void Image::operator -=	( const Image& img ){

		//cv::subtract( m_cvImage, img.getCVMat(), m_cvImage );
		//setUpdateTexture(true);
	}

	/**
	* @brief Blend two images
	*
	* @param Image& other
	* @param float  percentage
	*/
	void Image::blend( const Image& other, float percentage )
	{
		//// Check the image is valid
		//if ( !isValid() )
		//	THROW_EXCEPTION( "Trying to blend an image without initialization" );

		//if ( other.getFormat() != getFormat() )
		//	THROW_EXCEPTION( "Images with different format" );

		//if ( other.getWidth() != getWidth() || other.getHeight() != getHeight())
		//	THROW_EXCEPTION( "Images with different sizes" );

		//percentage = map( percentage, 0, 100, 0.0, 1.0 );
		//cv::addWeighted( m_cvImage, 1.0 - percentage, other.getCVMat(), percentage, 0.0f, m_cvImage );
	}

	void Image::operator = ( float scalar)
	{
		//// Check the image is valid
		//if ( !isValid() )
		//	THROW_EXCEPTION( "Trying to paint in an invalid image" );

		//switch( m_cvImage.channels() )
		//{
		//case 1:
		//	m_cvImage = cv::Scalar(scalar);
		//	break;
		//case 3:
		//	m_cvImage = cv::Scalar(scalar,scalar,scalar);
		//	break;
		//case 4:
		//	m_cvImage = cv::Scalar(scalar,scalar,scalar,scalar);
		//	break;
		//default:
		//	THROW_EXCEPTION( "Invalid number of channels in image" )
		//		break;
		//}

		//// Update texture when the next drawing call is made by the user
		//m_bUpdateTexture = true;
	}

	/**
	* @brief Fill the image with an input color
	*
	* @param Color
	*/
	void Image::fill( const Color& color )
	{

		//// Check the image is valid
		//if ( !isValid() )
		//	THROW_EXCEPTION( "Trying to paint in an invalid image" );

		//// Set the entire image 
		//switch( m_cvImage.channels() )
		//{
		//case 1:
		//	m_cvImage = cv::Scalar(color.r);
		//	break;
		//case 3:
		//	m_cvImage = cv::Scalar(color.r, color.g, color.b);
		//	break;
		//case 4:
		//	m_cvImage = cv::Scalar(color.r, color.g, color.b, color.a);
		//	break;
		//default:
		//	THROW_EXCEPTION( "Invalid number of channels in image" )
		//		break;
		//}

		//// Update texture when the next drawing call is made by the user
		//m_bUpdateTexture = true;

	}
	/**
	 * @brief Copy from image
	 * @param img Image to copy data and format from
	 * TODO: optimize
	 */
	void Image::copy( const Image& img )
	{
		*this = img;
	}

	/**
	 * @brief Copy from image
	 * @param alpha Transparency value for the image. Range 0..255
	 */
	void Image::setTransparency( float alpha )
	{
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to set transparency on an image that hast no been initialized" );
			return;
		}

		m_quad.setTransparency( alpha );
	}
	

	/**
	 * @brief Sets the blending mode of the image
	 * @param img Image to copy data and format from
	 * TODO: optimize
	 */
	void Image::setInkMode( ImageInkModes type )
	{
		if (type == ADD)
			m_quad.setAdditiveMode( true );

		if (type == BLEND)
			m_quad.setAdditiveMode( false );

	};

} // namespace Cing
