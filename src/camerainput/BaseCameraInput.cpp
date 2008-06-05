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
  m_currentCameraImage		( NULL  ), 
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
	if ( format == ARGB )
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
  m_currentCameraImage = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, m_nChannels ); 

  // Init the textured quad (just in case we need to render the camera)
  m_texturedQuad.init( width, height, m_format );

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
 	cvReleaseImage( &m_currentCameraImage );
  m_texturedQuad.end();

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
  m_texturedQuad.setVisible( false );
}

/**
 * @internal
 * @brief Loads the camera image data into the textured quad (just in case it is going to be rendered)
 * TODO: optimize this.
 */
void BaseCameraInput::draw()
{
  m_texturedQuad.setVisible( true );
  m_texturedQuad.setPosition( 0, 0 );
  m_texturedQuad.updateTexture( getImageData(), m_width, m_height, m_format );
}

/**
 * @internal
 * @briefUpdates the data of the camera frame.
 * This method should be called by subclasses to update the captured images
 * 
 * @param[in] data  Image data to update (the just captured camera frame)
 * @param[in] size  Size in bytes of the image data
 */
void BaseCameraInput::setNewFrameData( unsigned char* data, unsigned int size )
{
  //if ( size != m_frameSize )
  //  THROW_EXCEPTION( "Trying to camera image data with a wrong image data size" );

  // Set image data
  cvSetData( m_currentCameraImage, data, m_width * m_nChannels );
}


} // namespace CameraInput