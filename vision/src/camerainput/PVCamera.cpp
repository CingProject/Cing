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

#include "PVCamera.h"

// OpenCv
#include "cv.h"

// Common
#include "common/Exception.h"
#include "common/Release.h"

namespace CameraInput
{

void PVCamera::PVCaptureThread::execute()
{  
  unsigned char *cameraBuffer = NULL;
  unsigned char *cameraWriteBuffer = NULL;

  // Execute until the thread gets signaled
  while( !get_signaled() )
  {    
    // Get new frame from the camera
    cameraBuffer = m_PVCamera.m_pvCamera->getFrame();
    if ( cameraBuffer ) 
    {
      // Get buffer to write the image
      cameraWriteBuffer = m_PVCamera.m_ringBuffer->getNextBufferToWrite();
      if ( cameraWriteBuffer ) 
      {
        // Copy the new frame in the buffer, and notify it
        memcpy( cameraWriteBuffer, cameraBuffer, m_PVCamera.m_ringBuffer->size() );
        m_PVCamera.m_ringBuffer->writeFinished();
      }
    }
  } 
}


/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
PVCamera::PVCamera():
  m_pvCamera        ( NULL  ),
  m_captureThread   ( NULL  ),
  m_bIsValid        ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
PVCamera::~PVCamera()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @param[in] width   Width resolution to capture form camera
 * @param[in] height  Height resolution to capture form camera
 * @param[in] fps     FPS to capture form camera
 * @param[in] color   if true captured images will be color (if supported by the camera), false means b/w
 * @return true if the initialization was ok | false otherwise
 */
bool PVCamera::init( int width /*= 320*/, int height /*= 240*/, int fps /*= 25*/, bool color /*= true */ )
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Init base class
  BaseCameraInput::init( width, height, fps, color );
  
  // Find a camera in the system
  m_pvCamera = cameraTool::findCamera();	
  if ( !m_pvCamera )
    THROW_EXCEPTION( "No camera found in the system." );

  // Init the camera
  bool success = m_pvCamera->initCamera( width, height, color );

  // Init ok
  if(success) 
  {
    // Store real capture info (real means that maybe the requested capture is not supported by the connected camera, so it may differ)
    setWidth( m_pvCamera->getWidth() );
    setHeight( m_pvCamera->getHeight() );
    setFPS( m_pvCamera->getFps() );

  } 
  // Init error
  else 
  {
    THROW_EXCEPTION( "Error initializing camera" );
    m_pvCamera->closeCamera();
    Common::Release( m_pvCamera );
  }

  // Allocate buffers to store images
  unsigned int bufferSize = getWidth() * getHeight() * getNumChannels();
  m_ringBuffer  = new RingBuffer( bufferSize );

  // Start capturing images from camera
  success = m_pvCamera->startCamera();

  // Create capture thread (to retrieve images from the camera in a different execution thread)
  m_captureThread = new PVCaptureThread( *this );
  m_captureThread->start();

	// The class is now initialized
	m_bIsValid = success;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void PVCamera::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  // Release thread
  m_captureThread->signal();  

  // Release resources
  //Common::Release( sourceBuffer_ );
  Common::Release( m_ringBuffer );
  m_pvCamera->closeCamera();
  Common::Release( m_pvCamera );

  // Release parent class
  BaseCameraInput::end();

  // Delete thread
  Common::Release( m_captureThread );

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Captures new image from camera
 * @note this is a blocking call -> would be good to have it in a separate thread
 */
void PVCamera::update()
{
  // Update parent class
  BaseCameraInput::update();

  // Check if we have a new frame
  unsigned char *cameraReadBuffer = m_ringBuffer->getNextBufferToRead();
  if ( cameraReadBuffer == NULL )
    return;

  // Get all the new frames
  while( cameraReadBuffer != NULL )
  {
    // Copy current frame and notify it has been read to the ring buffer
    setNewFrameData( cameraReadBuffer, m_ringBuffer->size() );    
    m_ringBuffer->readFinished();

    // Check if there are more new frames
    cameraReadBuffer = m_ringBuffer->getNextBufferToRead();
  }

  // update the texture
  // TODO: esto sólo debería ser para debug
  //loadCameraImageToTexture();  
}

} // namespace CameraInput