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

// Precompiled headers
#include "Cing-Precompiled.h"


#include "BaseCameraInput.h"

// Common
#include "common/LogManager.h"

// OpenCV
#include "opencv2/imgproc/imgproc.hpp"

// Common
#include "common/Exception.h"

namespace Cing
{

	/**
	* @internal
	* @brief Constructor. Initializes class attributes.
	*/
	BaseCameraInput::BaseCameraInput():
		m_realFps					( 0			),
		m_newFrame          		( false ), 
		m_width             		( 0     ), 
		m_height            		( 0     ), 
		m_fps               		( 0     ), 
		m_bIsValid          		( false ),
		m_showFps          			( false ),
		m_realFpsAverage			( 3			)
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
bool BaseCameraInput::init( int deviceId /*= 0*/, int width /*= 320*/, int height /*= 240*/, int fps /*= 25*/, GraphicsType format, bool multithreaded /*= true*/  )
{
	// Check if the class is already initialized
	if ( isValid() )
		return true;

	// Check format has no alpha
	if ( format == RGBA )
	{
		format = RGB;
		LOG( "Camera capture with alpha channel not supported. Image format set to RGB" );
	}

	// Store data
	m_deviceId		= deviceId;
	m_width			= width;
	m_height		= height;
	m_format		= format;
	m_fps			= fps;

	// Calculate number of channels of the captured images and the frame size
	m_frameSize	= m_width * m_height * (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );
	m_nChannels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );

	// Create the image to store the camera frames
	m_currentCameraImage.init( width, height, format );

	// Create a temp image of the opposite type to make conversions in case it is necessary.
	// This means: if we are going to work in RGB, this image will be GRAYSCALE, just in case we receive a GRAYSCALE image instead of
	// a RGB image, so we can convert it fast... or vice versa..
	GraphicsType tempFormat	= format == RGB? GRAYSCALE: RGB;
	m_tempImage.init( width, height, tempFormat );

	// Reset fps timer
	m_timer.reset();

	// The class is now initialized
	m_bIsValid = true;
	return true;
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
* @briefUpdates the data of the camera frame.
* This method should be called by subclasses to update the captured images
* 
* @param[in] data		Image data to update (the just captured camera frame)
* @param[in] width		Width in pixels
* @param[in] height		Height in pixels
* @param format			Format to the image passed
* @param widthStep		width step of the passed data
* @param swapRB			swap red and blue channels
*/
void BaseCameraInput::setNewFrameData( unsigned char* data, unsigned int width, unsigned int height, GraphicsType format, int widthStep, bool swapRB /*= false*/ )	
{
	// Get capture fps
	unsigned long elapsedMicroseconds = m_timer.getMicroseconds();
	m_realFps = (double)1000000 / (double)elapsedMicroseconds;
	m_realFpsAverage.addValue( m_realFps );

	// If the received image has the same format... copy it
	if ((width == m_currentCameraImage.getWidth() ) && 
		(height == m_currentCameraImage.getHeight()) && 
		(m_format == format) )
	{
		m_currentCameraImage.setData( data, width, height, format, widthStep );
		
		// Swap red and blue channels
		if ( swapRB )
		{
			//cvtColor( toCVMat(m_currentCameraImage), toCVMat(m_currentCameraImage), CV_RGB2BGR );
			m_currentCameraImage.setUpdateTexture(true);
		}
	}	
	// If we are working in GRAYSCALE and the received image is RGB -> convert it, and then store it
	else if ((width == m_currentCameraImage.getWidth() ) && 
			 (height == m_currentCameraImage.getHeight()) && 
			 (m_format == GRAYSCALE) && (format == RGB) )
	{
		// Set data to temp image to make the conversion
		m_tempImage.setData( data, width, height, format, widthStep );

		// Convert it and mark the image to update to texture next draw
		//cv::cvtColor( toCVMat(m_tempImage), toCVMat(m_currentCameraImage), CV_BGR2GRAY );
		m_currentCameraImage.setUpdateTexture( true );
	}
	// Check if we have an image with 4 channels -> convert it to RGB or GRAY
	else if ((width == m_currentCameraImage.getWidth() ) && 
			 (height == m_currentCameraImage.getHeight()) && 
			 (format == RGBA ) )
	{
		// Convert to RGB (from RGBA)
		if ( m_format == RGB )
		{
			int nChannels = (int)Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );	
			cv::Mat imgData(height, width, CV_8UC(nChannels), data, widthStep);
			
			//cvtColor(imgData, toCVMat(m_currentCameraImage), CV_RGBA2RGB);
			m_currentCameraImage.setUpdateTexture( true );
		}
		// Convert to GRAY (from RGB) -> not supported for now
		else if ( m_format == GRAYSCALE )
		{
			LOG_ERROR( "Trying to set camera image data with a wrong size or format" );	
		}
	}
	// Not compatible format or size
	else
		LOG_ERROR( "Trying to set camera image data with a wrong size or format" );

	// Set fps?
	if ( m_showFps )
	{
		std::ostringstream oss;
		oss << "FPS: " << m_realFpsAverage.getValue();
		m_currentCameraImage.text( 0, m_currentCameraImage.getHeight(), oss.str().c_str() );
	}

	// Reset fps timer
	m_timer.reset();
}


} // namespace Cing