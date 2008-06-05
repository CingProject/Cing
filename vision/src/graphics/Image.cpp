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

#include "Image.h"

// Common
#include "common/Exception.h"
#include "common/ResourceManager.h"

namespace Graphics
{

/**
 * @brief Constructor. This constructor does not load or creates any image.
 */
Image::Image():
  m_bIsValid( false )
{
}

/**
 * @brief Creates an image with the same data as the received image
 *
 * @param
 */
 Image::Image( const Image& img )
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
Image::Image( int width, int height, ImageFormat format /*= RGB*/ )
{
	init( width, height, format );
}

/**
 * @brief Creates an image from a file. The image loaded can be modified afterwards.
 * @note The image file specified should be placed on the data directory of the application
 *
 * @param name  Name of the file to be loaded. It must be placed in the data directory.
 */
Image::Image( const std::string& name )
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
 * @param format Format of the image to be created. Possible formats are: RGB, ARGB, GRAYSCALE
 */
void Image::init( int width, int height, ImageFormat format /*= RGB*/  )
{
  // Check if the class is already initialized
  if ( isValid() )
   THROW_EXCEPTION( "Image already initialized" );

	// Create the empty image
	THROW_EXCEPTION( "TODO" );

	// The class is now initialized
	m_bIsValid = true;

}

/**
 * @brief Creates a copy of an image
 *
 * @param img Image to be copied
 */
void Image::init( const Image& img )
{
	this->operator =( img );
	m_bIsValid = true;
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
	// Load file from disk
	m_image.load( name, Common::ResourceManager::userResourcesGroupName );

	// Create the texture quad (to draw image)
	m_quad.init( (int)m_image.getWidth(), (int)m_image.getWidth(), (ImageFormat)m_image.getFormat() );
  
	// Load data to texture
	m_quad.updateTexture( m_image );

	// The class is now initialized
	m_bIsValid = true;
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

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @brief Draws the image in a specific position
 *
 * @param xPos x coordinate where the image should be drawn
 * @param yPos y coordinate where the image should be drawn
 * @param zPos z coordinate where the image should be drawn. If not passed, z will be set to zero
 */
void Image::draw( int xPos, int yPos, int zPos /*= 0*/ )
{
	m_quad.setPosition( xPos, yPos, zPos );
	m_quad.setVisible( true );
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

	// Copy the data
	m_image = other.m_image;
	m_quad  = other.m_quad;

	// Now this image is valid
	m_bIsValid = true;
}

} // namespace Graphics