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

#include "CompVision.h"

// OpenCv
#include "externLibs/OpenCV/cxcore/include/cxcore.h"

// Common
#include "common/Exception.h"

namespace ComputerVision
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
CompVision::CompVision():
  m_background                ( NULL  ),
  m_afterBackgroundSubtraction( NULL  ),
  m_backgroundSubtraction     ( true ),
  m_blobDetection             ( true ),
  m_bIsValid                  ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
CompVision::~CompVision()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @param[in] width     width of the images to process by the framework
 * @param[in] height    height of the images to process by the framework
 * @return true if the initialization was ok | false otherwise
 */
bool CompVision::init( int width, int height )
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Init the available filters and computer vision algorithms
  m_blobFinder.init( width, height );
  m_imgDiffFilter.init( width, height );
  m_imgThresholdFilter.init( width, height );

  // Create images (gray scale)
  m_cameraImage                 = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 ); 
  m_background                  = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 ); 
  m_afterBackgroundSubtraction  = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 ); 

  // Configure filters
  // TODO: hacer parametrizable
  m_imgThresholdFilter.setThreshold( 50 );

  // Debug data 
  m_cameraTQ.init( width, height, 1);
  m_cameraTQ.setVisible( false );

  m_backgrounTQ.init( width, height, 1);
  m_backgrounTQ.setVisible( false );

  m_afterBackgroundSubtractionTQ.init( width, height, 1 );
  m_afterBackgroundSubtractionTQ.setVisible( false );

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void CompVision::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  // Release data
  // TODO
  //cvReleaseImage( &m_cameraImage );
  //cvReleaseImage( &m_background );
  //cvReleaseImage( &m_afterBackgroundSubtraction );

  // Release filters
  m_blobFinder.end();
  m_imgDiffFilter.end();
  m_imgThresholdFilter.end();

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Performs the computer vision analysis based on the activated flags
 * @param[in] imgToAnalyze Image that will be analyzed by the system
 */
void CompVision::update( IplImage* imgToAnalyze )
{
  // Check image pointer
  if ( !imgToAnalyze )
    THROW_EXCEPTION( "Error, invalid image received for the CompVision update process" );
  
  // By default hide textured quads (they are only drawn if the method drawComputerVisionProcess is called)
  m_cameraTQ.setVisible( false );
  m_backgrounTQ.setVisible( false );
  m_afterBackgroundSubtractionTQ.setVisible( false );

  // Copy camera image data pointer
  cvSetData( m_cameraImage, imgToAnalyze->imageData, imgToAnalyze->widthStep );

  // Background subtraction
  if ( m_backgroundSubtraction )
  {
    // Calculate the difference between the background and the current image
    m_imgDiffFilter.apply( *m_background, *imgToAnalyze );

    // Threshold the image
    m_imgThresholdFilter.apply( m_imgDiffFilter.getOutputImage() );

    // Store result
    cvCopy( &m_imgThresholdFilter.getOutputImage(), m_afterBackgroundSubtraction, 0 );
  }

  // Blob detection
  if ( m_blobDetection )
    m_blobFinder.computeBlobs( *m_afterBackgroundSubtraction );
}

/**
 * @internal
 * @param[in] backgroundImage Image to store as background. This is the image of the scene with no body in
 */
void CompVision::storeBackground( const IplImage* backgroundImage )
{
  // Store the background image
  cvCopy( backgroundImage, m_background, 0 );
}

/**
 * @internal
 * @brief Debug method to draw the images involved in the computer vision process
 */
void CompVision::drawComputerVisionProcess()
{
  // Make texture quads visibles
  m_cameraTQ.setVisible( true );
  m_backgrounTQ.setVisible( true );
  m_afterBackgroundSubtractionTQ.setVisible( true );

  // Upload data to textures
  m_cameraTQ.updateTexture( m_cameraImage->imageData, m_cameraImage->width, m_cameraImage->height, 1 );
  m_backgrounTQ.updateTexture( m_background->imageData, m_background->width, m_background->height, 1 );
  m_afterBackgroundSubtractionTQ.updateTexture( m_afterBackgroundSubtraction->imageData, m_afterBackgroundSubtraction->width, m_afterBackgroundSubtraction->height, 1 );

  // Set quad positions
  m_backgrounTQ.setPosition( 320, 0 );
  m_afterBackgroundSubtractionTQ.setPosition( 640, 0 );
}

} // namespace CompVision