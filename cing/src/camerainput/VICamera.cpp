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


#include "VICamera.h"
#include "common/CommonUtilsIncludes.h"

namespace Cing
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
VICamera::VICamera():
	m_bIsValid  ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
VICamera::~VICamera()
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
 * @return True if the camera was correctly initialized, false otherwise
 */
bool VICamera::init( int deviceId, int width, int height, int fps, GraphicsType format, bool multithreaded /*= true*/ )
{
	// List connected devices
	int numDevices = videoInput::listDevices();	
	if ( numDevices == 0 )
	{
		LOG_ERROR( "No camera device found" );
		return false;
	}
	else
		LOG( "Number of devices to capture found: %d", numDevices );

	// uncomment for silent setup
	//videosetVerbose(false);

	// multi threaded?
	m_viCamera.setUseCallback( multithreaded );

	// Capture  fps (not waranteed)
	m_viCamera.setIdealFramerate(deviceId, fps);

	// Setup camera
	bool result = m_viCamera.setupDevice( deviceId, width, height, VI_COMPOSITE ); 
	if ( result == false )
	{
		LOG_ERROR( "Error setting up camera device for capture" );
		return false;
	}

	// Init base class (with actual capture resolution)
	result = BaseCameraInput::init( deviceId, m_viCamera.getWidth( deviceId ), m_viCamera.getHeight( deviceId ), fps, format );
	if ( result == false )
	{
		LOG_ERROR( "Internal error initializing BaseCameraInput" );
		end();
		return false;
	}

	// Set capture format (optional)
	//m_viCamera.setFormat(dev, VI_NTSC_M);					//optional set the format

	m_bIsValid = true;
	return true;
}

/**
 * @brief Captures a new frame from the camera (if available)
 */
void VICamera::update()
{
	setNewFrame( false );

	// If we have a new frame
	if( m_viCamera.isFrameNew( m_deviceId ) )	
	{
		// Calculate the format
		int npixels			= m_viCamera.getWidth( m_deviceId ) * m_viCamera.getHeight( m_deviceId );
		size_t frameSize	= m_viCamera.getSize( m_deviceId );
		GraphicsType format = ( npixels == frameSize )? GRAYSCALE: RGB;
		int nChannels = format == RGB? 3: 1;

		// we get the pixels by passing in out buffer which gets 
		setNewFrameData(  m_viCamera.getPixels( m_deviceId, true, true ),  m_viCamera.getWidth( m_deviceId ),  m_viCamera.getHeight( m_deviceId ), format, m_viCamera.getWidth( m_deviceId ) * nChannels );
		setNewFrame( true );
	}
}

/**
 * @internal
 * @brief Stops camera capture
 * After this call no method of this object can be called without calling init method again.
 */
void VICamera::end()
{
	m_viCamera.stopDevice( m_deviceId );
	m_bIsValid = false;
}

} // namespace Cing