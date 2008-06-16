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

#include "BaseCameraInput.h"

// Common
#include "common/LogManager.h"

// OpenCV
#include "externLibs/OpenCV/cxcore/include/cxcore.h"
#include "externLibs/OpenCV/highgui/include/highgui.h"
#include "externLibs/OpenCV/cv/include/cv.h"

// Common
#include "common/Exception.h"

namespace CameraInput
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
BaseCameraInput::BaseCameraInput():
  m_newFrame          		( false ), 
  m_width             		( 0     ), 
  m_height            		( 0     ), 
  m_fps               		( 0     ), 
  m_bIsValid          		( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
BaseCameraInput::~BaseCameraInput()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @param[in] deviceId	Id of the device to capture from (starting at 0)
 * @param[in] width			width resolution capture
 * @param[in] height		height resolution capture
 * @param[in] fps				frames per second to capture
 * @param[in] format		Format of the image. if RGB the captured images will be color (if supported by the camera), if GRAYSCALE, they will be b/w
 */
void BaseCameraInput::init( int deviceId /*= 0*/, int width /*= 320*/, int height /*= 240*/, int fps /*= 25*/, ImageFormat format )
{
  // Check if the class is already initialized
  if ( isValid() )
    return;

	// Check format has no alpha
	if ( format == RGBA )
	{
		format = RGB;
		LOG( "Camera capture with alpha channel not supported. Image format set to RGB" );
	}

	// Store data
	m_deviceId	= deviceId;
	m_width			= width;
	m_height		= height;
	m_format		= format;

  // Calculate number of channels of the captured images and the frame size
	m_frameSize	= m_width * m_height * (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );
	m_nChannels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );

  // Create the image to store the camera frames
	m_currentCameraImage.init( width, height, format );

	// Create a temp image of the opposite type to make conversions in case it is necessary.
	// This means: if we are going to work in RGB, this image will be GRAYSCALE, just in case we receive a GRAYSCALE image instead of
	// a RGB image, so we can convert it fast... or vice versa..
	ImageFormat tempFormat	= format == RGB? GRAYSCALE: RGB;
	m_tempImage.init( width, height, tempFormat );

	// The class is now initialized
	m_bIsValid = true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void BaseCameraInput::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  // Release resources
	m_currentCameraImage.end();
	m_tempImage.end();

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Updates the class state
 */
void BaseCameraInput::update()
{
  // Invisible except draw method is called
 // m_texturedQuad.setVisible( false );
}

/**
 * @internal
 * @brief Loads the camera image data into the textured quad (just in case it is going to be rendered)
 * TODO: optimize this.
 */
void BaseCameraInput::draw( )
{
	//m_texturedQuad.setVisible( true );
 // m_texturedQuad.setPosition2d( 0, 0 );
 // m_texturedQuad.updateTexture( getImageData(), m_width, m_height, m_format );
}

/**
 * @internal
 * @briefUpdates the data of the camera frame.
 * This method should be called by subclasses to update the captured images
 * 
 * @param[in] data  Image data to update (the just captured camera frame)
 * @param[in] width		Width in pixels
 * @param[in] height  Height in pixels
 * @param format			Format to the image passed
 */
void BaseCameraInput::setNewFrameData( char* data, unsigned int width, unsigned int height, ImageFormat format )	
{
  //if ( size != m_frameSize )
  //  THROW_EXCEPTION( "Trying to camera image data with a wrong image data size" );

	// If the received image has the same format... copy it
	if ( (width == m_currentCameraImage.getWidth() ) && (height == m_currentCameraImage.getHeight()) && (m_format == format) )
		m_currentCameraImage.setData( data, width, height, format );
		//cvSetData( m_currentCameraImage, data, m_width * m_nChannels );

	// If we are working in GRAYSCALE and the received image is RGB -> convert it, and then store it
	else if ( (m_format == GRAYSCALE) && (format == RGB) )
	{
		m_tempImage.setData( data, width, height, format );
		//cvSetData( m_tempImage, data, (m_width * m_nChannels) * 3 );
		cvCvtColor( &m_tempImage.getCVImage(), &m_currentCameraImage.getCVImage(), CV_BGR2GRAY );
		m_currentCameraImage.setUpdateTexture( true );
	}
	else
		LOG_ERROR( "Trying to set camera image data with a wrong format" );
}


} // namespace CameraInput