/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

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


#include "PVCamera.h"

// OpenCv
#include "OpenCV/cv.h"

// Common
#include "common/Exception.h"
#include "common/Release.h"

namespace Cing
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
 * @param[in] deviceId	Id of the device to capture from (starting at 0)
 * @param[in] width			width resolution capture
 * @param[in] height		height resolution capture
 * @param[in] fps				frames per second to capture
 * @param[in] format		Format of the image. if RGB the captured images will be color (if supported by the camera), if GRAYSCALE, they will be b/w
 */
void PVCamera::init( int deviceId, int width, int height, int fps, GraphicsType format )
{
  // Check if the class is already initialized
  if ( isValid() )
    return;
  
  // Find a camera in the system
  m_pvCamera = cameraTool::findCamera();	
  if ( !m_pvCamera )
    THROW_EXCEPTION( "No camera found in the system." );

  // Init the camera
	bool color = Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format ) == 3? true: false;
  bool success = m_pvCamera->initCamera( width, height, fps, color );

  // Init ok
  if(success) 
  {
		// Init base class (with actual capture resolution)
	  BaseCameraInput::init( deviceId, m_pvCamera->getWidth(), m_pvCamera->getHeight(), m_pvCamera->getFps(), format );

  } 
  // Init error
  else 
  {
    THROW_EXCEPTION( "Error initializing camera" );
    m_pvCamera->closeCamera();
    Release( m_pvCamera );
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
  //Release( sourceBuffer_ );
  Release( m_ringBuffer );
  m_pvCamera->closeCamera();
  Release( m_pvCamera );

  // Release parent class
  BaseCameraInput::end();

  // Delete thread
  Release( m_captureThread );

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
  // Check if we have a new frame
  unsigned char *cameraReadBuffer = m_ringBuffer->getNextBufferToRead();
  if ( cameraReadBuffer == NULL )
    return;

  // Get all the new frames
  while( cameraReadBuffer != NULL )
  {
    // Copy current frame and notify it has been read to the ring buffer
	  setNewFrameData( cameraReadBuffer, getWidth(), getHeight(), getFormat(), getWidth() * getNumChannels());    
    m_ringBuffer->readFinished();

    // Check if there are more new frames
    cameraReadBuffer = m_ringBuffer->getNextBufferToRead();
  }
}

} // namespace Cing