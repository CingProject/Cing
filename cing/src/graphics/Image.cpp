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
     * This will be a weak copy, so no new data is created. Instead this new image will point to the data of the received image
     * so if you modify any of them, the other will be affected as well.
     *
     * @param
     */
	Image::Image( const Image& other ):
    m_bIsValid( false ),
    m_bUpdateTexture( false ),
    m_loadedFromFile(false),
    m_path( "NOT_LOADED_FROM_FILE")
	{
		*this = other;
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
     * Therefore, the image created is empty (unless image data is passed as a parameter). However, this image can be modified afterwards.
     * @note If data is passed as a parameter, a weak copy is made, meaning that the image data on this image points to the receiced. Therefore
     * if this image is altered, and the passed data comes from other image, that other image will be altered as well. Same in the other direction.
     *
     * @param width  Width of the image to be created
     * @param height Height of the image to be created
     * @param format Format of the image to be created. Possible formats are: RGB, RGBA, GRAYSCALE
     * @param sm		SceneManager point. If NULL, the main Scene Manager will be used. This allows to create the image under another scene manager for separate rendering.
     * @param data	Pointer to data to populate the image (if NULL, new data is created). From now on, this Image object will own that memory, so it is not necessary to
     * delete it from the outside (and actually it should not be deleted).
     */
	void Image::init( int width, int height, GraphicsType format /*= RGB*/, Ogre::SceneManager* sm /*= NULL*/, ImageDataPtr data /*= ImageDataPtr(NULL)*/  )
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
		if ( data )
			m_data = data;
		else
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
        
		// Store the number of channels
		m_nChannels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)m_format );
		
		// Create a pixel box to be able to define the format
		// TODO: optimize this
		size_t bufferSize = Ogre::PixelUtil::getMemorySize( width, height, 1, (Ogre::PixelFormat)m_format );
		
		// Create the memory buffer
		m_data = ImageDataPtr( new unsigned char[bufferSize] );
        
		// Create the Ogre::Image
		m_image.loadDynamicImage( m_data.get(), width, height, (Ogre::PixelFormat)m_format );
        
		// Create the texture quad (to draw image)
		m_quad.init( width, height, m_format, RENDERTARGET );
        
		// The class is now initialized
		m_bIsValid			= true;
		m_bUpdateTexture	= true;
		m_loadedFromFile	= false;
        
	}
    
	/**
     * @brief Creates a copy of an image. This is a weak copy, so both images point to the same data from here on.
     * @note use copyTo if you want to create a deep copy, or call directly setData.
     *
     * @param img Image to be copied
     */
	void Image::init( Image& other )
	{
		// Check application correctly initialized (could not be if the user didn't call size() function)
		Application::getSingleton().checkSubsystemsInit();
        
		// Make sure the other image is valie
		if ( !other.isValid() )
		{
			LOG_ERROR( "Trying to copy from an invalid image (it has not been initialized)" );
			return;
		}
        
		// Create the weak copy of the image
		*this = other;
        
		m_bIsValid			= true;
		m_bUpdateTexture	= true;
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
		m_format = toCingPixelFormat( m_image.getFormat() );
        
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
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to save an invalid image(it has not been initialized)" );
			return;
		}
        
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
        
		// Release the shared pointer
		m_data.reset();
        
		// Release the Ogre::IMage just in case it owns the memory buffer
		m_image.freeMemory();
        
		// TODO: This is a hack to avoid a crash comming from cvReleaseImage
		// when it is called by a static object destructor (global Image variable created by the user)
		// when there is image copy involved
		Ogre::SceneManager* sceneManager = getTexturedQuad().getSceneManager();
		if ( sceneManager == NULL )
			return;
        
		// Release the quad
		m_quad.end();
        
		// The class is not valid anymore
		m_bIsValid = false;
        
	}
    
	/**
     * @brief Sets the data of the image (it copies the data, so it is a fresh new copy)
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
        
        if ( !imageData )
        {
        	LOG_ERROR( "Trying to set data to with a NULL pointer passed as data pointer." );
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
		
		// Copy the image to our data
		// NOTE: this cast is due to Ogre::Image.loadDynamicImage not receiving a const pointer when it should
		std::memcpy( m_data.get(), imageData, imageBufferSize( width, height, format ) );
        
		// Load data into our image
		m_image.loadDynamicImage( m_data.get(), width, height, m_image.getFormat() );
        
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
     * @note You usually want to call getPixel(x, y, color) instead as it is faster.
     *
     * @param x x coordinate of the pixel that wants to be retrieved
     * @param y y coordinate of the pixel that wants to be retrieved
     */
	Color Image::getPixel( int x, int y ) const
	{
        Color temp;
        getPixel(x, y, temp);
        return temp;
    }
    
	/**
     * @brief Method to get the color of a pixel at x-y position in this image.
     * TODO: too slow!
     * @param x x coordinate of the pixel that wants to be retrieved
     * @param y y coordinate of the pixel that wants to be retrieved
     * @param outColor Variable where the color of the pixel will be stored. It is faster to call this instead of getPixel(x, y);
     */
	void Image::getPixel( int x, int y, Color& outColor ) const
	{
        // Check the image is valid
		if ( !isValid() || !m_image.getData() )
			THROW_EXCEPTION( "Trying to get a pixel from an invalid image" );
        
		// Check boundaries
        int imgWidth  = getWidth();
        int imgHeight = getHeight();
		if ( x < 0 )				x = 0;
		if ( x >= imgWidth )        x = imgWidth - 1;
		if ( y < 0 )				y = 0;
		if ( y > imgHeight)         y = imgHeight -1 ;
        
		// Read color
        int step = imgWidth * m_nChannels;
		const unsigned char* pixelPtr = m_image.getData() + step * y;
        
		switch( m_nChannels )
		{
            case 1:
                outColor.b  = (float)((int)pixelPtr[ x*m_nChannels + 0 ]);
                outColor.g  = outColor.b;
                outColor.r  = outColor.b;
                outColor.a  = 255;
                break;
            case 2:
                THROW_EXCEPTION( "Invalid number of channels in image" );
                break;
            case 3:
                outColor.b  = (float)((int)pixelPtr[ x*m_nChannels + 0 ]);
                outColor.g	= (float)((int)pixelPtr[ x*m_nChannels + 1 ]);
                outColor.r  = (float)((int)pixelPtr[ x*m_nChannels + 2 ]);
                outColor.a	= 255;
                break;
            case 4:
                outColor.b  = (float)((int)pixelPtr[ x*m_nChannels + 0 ]);
                outColor.g	= (float)((int)pixelPtr[ x*m_nChannels + 1 ]);
                outColor.r  = (float)((int)pixelPtr[ x*m_nChannels + 2 ]);
                outColor.a	= (float)((int)pixelPtr[ x*m_nChannels + 3 ]);
                break;
            default:
                THROW_EXCEPTION( "Invalid number of channels in image" )
				break;
		}
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
     * @brief Copies the received image into the current image. The format of this image will be the same as the received image
     * but the data won't be copied, that is, this is a weak copy so it references the data on the source Image. Therefore, if you modify
     * this image from now on, it will modify as well the data on the source Image.
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
			init( other.getWidth(), other.getHeight(), other.getFormat(), sm, other.m_data );
		}
		// Check if the size of the image differs or the number of channels... just recreate it
		// TODO: optimize this
		else if ( (other.getWidth() != getWidth()) || (other.getHeight() != getHeight()) || (other.getNChannels() != getNChannels()) )
		{
			LOG( "Image will be recreated as the size of both images or the number of channels differ" );
			end();
			init( other.getWidth(), other.getHeight(), other.getFormat(), sm, other.m_data );
		}
		// If it is the same format, just point the data shared point of this image to the received one
		else
		{
			// Assign the pointer of the other image's data
			m_data.reset();
			m_data = other.m_data;
            
			// Give Ogre::Image the new pointer
			m_image.loadDynamicImage( m_data.get(), other.getWidth(), other.getHeight(), toOgrePixelFormat( other.getFormat() ) );
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
	 * @brief  Duplicates an image, returns new Image object.
	 * @note This is a deep copy, that is, the data is duplicated so this means:
	 * - it is slower than the operator=
	 * - but you can modify the returned image and not affect the origina
	 *
	 * @return The cloned image
	 */
	Image Image::clone()
	{
		// Check image is valid
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to clone an invalid image" );
		}
        
		// Check application correctly initialized (could not be if the user didn't calle size function)
		Application::getSingleton().checkSubsystemsInit();
        
		// Create a new image (so that it has its own data, as we're doing a deep copy here)
		Image newImage;
		newImage.init( getWidth(), getHeight(), getFormat(), m_quad.getSceneManager() );
		newImage.setData( getData() );
        
		return newImage;
	}
    
	/**
     * @brief  Copys the image data to another image
     * @note This is a deep copy, that is, the data is duplicated so this means:
     * - it is slower than the operator=
     * - but you can modify the returned image and not affect the origina
     *
     * @note This should be faster than clone, as no new image is being create, just copying or converting pixels
     * @return The cloned image
     */
	void Image::copyTo( Image& other )
	{
		// Check image is valid
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to clone an invalid image" );
		}
        
		// Make sure format is the same....
		if ( (other.getWidth() != getWidth()) || (other.getHeight() != getHeight()) || (other.getNChannels() != getNChannels()) )
		{
			other.init( *this );
		}
        
		other.setData( getData() );
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
        
		if ( renderer2D )
			renderer2D->point( *this, x, y );
        
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
        
		if ( renderer2D )
			renderer2D->line( *this, x1, y1, x2, y2 );
        
		// Update texture when the next drawing call is made by the user
		m_bUpdateTexture = true;
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
        
		if ( renderer2D )
			renderer2D->triangle( *this, x1, y1, x2, y2, x3, y3 );
        
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
        
		if ( renderer2D )
			renderer2D->rect( *this, x, y, width, height );
        
		// Update texture when the next drawing call is made by the user
		m_bUpdateTexture = true;
	}
    
	/**
	 * @brief Draws a quad, defined by four points
	 *
	 * @param x1 x, first point (top-left)
	 * @param y1 y, first point (top-left)
	 * @param x2 x, second point (top-right)
	 * @param y2 y, second point (top-right)
	 * @param x3 x, third point (bottom-right)
	 * @param y3 y, third point (bottom-right)
	 * @param x4 x, fourth point (bottom-left)
	 * @param y4 y, fourth point (bottom-left)
	 */
	void Image::quad( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 )
	{
		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );
        
		if ( renderer2D )
			renderer2D->quad( *this, x1, y1, x2, y2, x3, y3, x4, y4 );
        
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
	 * @param start start angle (in radians)
	 * @param end end angle of the arc(in radians)
	 */
	void Image::arc( int x, int y, int width, int height, float start, float end )
	{
		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );
        
		if ( renderer2D )
			renderer2D->arc( *this, x, y, width, height, start, end );
        
		// Update texture when t:he next drawing call is made by the user
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
	void Image::ellipse( int x1, int y1, int width, int height, float angleDegrees /*= 0*/ )
	{
		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );
        
		if ( renderer2D )
			renderer2D->ellipse( *this, x1, y1, width, height, angleDegrees );
		
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
	void Image::text( int x1, int y1,  const std::string& text )
	{
		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );
        
		if ( renderer2D )
			renderer2D->text( *this, x1, y1, text );
        
		// Update texture when the next drawing call is made by the user
		m_bUpdateTexture = true;
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
        
		if ( renderer2D )
			renderer2D->filter( *this, type, param );
        
		// Update texture when the next drawing call is made by the user
		m_bUpdateTexture = true;
	}
    
    
	/**
     * @brief Converts an image from GRAYSCALE to Color format (RGB, that is 3 channels, no alpha)
     * @note As this conversion is done "in place", the image buffer is deleted and re-created. If you need
     * this operation to be made faster, you can have 2 Image objects (one for color and one for gray), and use the
     * operator = to convert one into the other. That way no memory will be created.
     */
	void Image::toColor()
	{
		// Check valid
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to convert to gray an invalid image. Maybe it is not initialized" );
			return;
		}
        
		// Check current format, if it is already grayscale -> do nothing
		if ( (m_nChannels == 3) || (m_nChannels == 4) )
			return;
        
		// Create the image memory new buffer to store the gray image and create the new data buffer
		unsigned int newNCHannels	= numberOfChannels( RGB );
		size_t bufferSize			= m_image.getWidth() * m_image.getHeight() * newNCHannels;
		unsigned char* imageBuffer	= new unsigned char[bufferSize] ; // no need to delete it her as it is assigned to a shred pointer in the init call
		unsigned int imageWidth		= m_image.getWidth();
		unsigned int imageHeight	= m_image.getHeight();
		
		// Convert the current image to gray
		Ogre::PixelUtil::bulkPixelConversion( m_image.getData(), m_image.getFormat(), imageBuffer, toOgrePixelFormat( RGB ), imageWidth * imageHeight );
        
		// Re-init the image with the new format and se the grayscale data
		end();
		init( imageWidth, imageHeight, RGB, NULL, ImageDataPtr(imageBuffer) );
        
		// Now this image is valid
		m_bIsValid = true;
	}
    
    
	/**
     * @brief Converts an image from Color (RGB) to GRAYSCALE
     * @note As this conversion is done "in place", the image buffer is deleted and re-created. If you need
     * this operation to be made faster, you can have 2 Image objects (one for color and one for gray), and use the
     * operator = to convert one into the other. That way no memory will be created.
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
        
		// Create the image memory new buffer to store the gray image and create the new data buffer
		size_t bufferSize			= m_image.getWidth() * m_image.getHeight();
		unsigned char* imageBuffer	= new unsigned char[bufferSize] ; // no need to delete it her as it is assigned to a shred pointer in the init call
		unsigned int imageWidth		= m_image.getWidth();
		unsigned int imageHeight	= m_image.getHeight();
		
		// Convert the current image to gray
		Ogre::PixelUtil::bulkPixelConversion( m_image.getData(), m_image.getFormat(), imageBuffer, toOgrePixelFormat( GRAYSCALE ), imageWidth * imageHeight );
        
		// Re-init the image with the new format and se the grayscale data
		end();
		init( imageWidth, imageHeight, GRAYSCALE, NULL, ImageDataPtr(imageBuffer) );
        
		// Now this image is valid
		m_bIsValid = true;
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
		// Check the image is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to paint in an invalid image" );
        
		if ( renderer2D )
			renderer2D->fill( *this, color );
        
		// Update texture when the next drawing call is made by the user
		m_bUpdateTexture = true;
	}
	/**
	 * @brief Copy from image
	 * @param img Image to copy data and format from
	 * TODO: optimize
	 */
	//void Image::copy( const Image& img )
	//{
	//	*this = img;
	//}
    
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
