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

#include "BackgroundSubtraction.h"

// Common
#include "common/CommonUtilsIncludes.h"
#include "common/LogManager.h"

// Graphics
#include "graphics/ImageResourceManager.h"

// OpenCv
#include "OpenCV/cxcore.h"

namespace Cing
{

// Static member init
const int BackgroundSubtraction::DEFAULT_THRESHOLD = 50;

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
BackgroundSubtraction::BackgroundSubtraction():
	m_backgroundImage	( NULL ), m_technique( ABS_DIFF )
{
	// Set default threshold
	m_thresholdFilter.setThreshold( DEFAULT_THRESHOLD );
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
BackgroundSubtraction::~BackgroundSubtraction()
{
	// Release resources
	end();
}


/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void BackgroundSubtraction::end()
{
	cvReleaseImage( &m_backgroundImage );
	m_backgroundImage = NULL;
}

/**
 * @brief Performs the background subtraction
 *
 * @param				imgToAnalyze	Image to analyze. It will be compared with the stored background
 * @param[out]	output				The result of the background subtraction will be stored in this image
 */
void BackgroundSubtraction::update( const Image& imgToAnalyze, Image& output )
{
	// If we don't have background stored -> store this image as the background
	if ( !m_backgroundImage )
		storeBackground( imgToAnalyze );

	// Request temporal images to the image resource manager
	IplImage* tempImage = ImageResourceManager::getSingleton().getImage( imgToAnalyze.getWidth(), imgToAnalyze.getHeight(), imgToAnalyze.getNChannels() );
	if ( tempImage == NULL )
	{
		LOG_ERROR( "Could not apply BackgroundSubtraction due to the lack of temporal image to work" );
		return;
	}

	// Check if is valid
	if ( !isValid() )
	{
		LOG_ERROR( "To apply background subtraction, a background image should be added before!" );
		return;
	}

	// Bg Subtraction technique
	if ( m_technique == ABS_DIFF )
	{
		// Calculate the difference between the background and the current image
		m_differenceFilter.apply( *m_backgroundImage, imgToAnalyze.getCVImage(), *tempImage );

		// Threshold the image
		IplImage tempImageHeader = (IplImage)*tempImage;
		IplImage tempOutImageHeader = (IplImage)output.getCVImage();
		m_thresholdFilter.apply(tempImageHeader, tempOutImageHeader );
	}
	else if ( m_technique == BRIGHTNESS )
	{
		// Subtract background from the current image (brighter pixels will remain non black)
		cvSub( &imgToAnalyze.getCVImage(), m_backgroundImage, tempImage );

		// Threshold the image
		IplImage tempImageHeader = (IplImage)*tempImage;
		IplImage tempOutImageHeader = (IplImage)output.getCVImage();
		m_thresholdFilter.apply( tempImageHeader, tempOutImageHeader);
	}

	// Mark the output image to be uploaded to the texture in case it is drawn
	output.setUpdateTexture( true );

	// Release temporal image
	ImageResourceManager::getSingleton().releaseImage( tempImage );
}


/**
 * @brief Stores the background image. It is the image that will be compared with the images
 * passed to the method compute to calculate the result
 *
 * @param backgroundImage	Image that will be compared with the "every frame" image.
 */
void BackgroundSubtraction::storeBackground( const Image& backgroundImage )
{
	// Create the image if necessary
	if (	(!m_backgroundImage) || 
				( backgroundImage.getWidth()!= m_backgroundImage->width) ||
				( backgroundImage.getHeight() != m_backgroundImage->height) ||
				( backgroundImage.getNChannels() != m_backgroundImage->nChannels) )
	{
		LOG_TRIVIAL( "BackgroundSubtraction::storeBackground: WARNING Creating a new image to store the background (we don't have one or it has a different size of number of channels)" );

		// Release old image (if it exists) and create the new one
		cvReleaseImage( &m_backgroundImage );
		m_backgroundImage = cvCloneImage( &backgroundImage.getCVImage() );
	}
	// If we have an ok image -> just copy the data
	else
		cvCopy( &backgroundImage.getCVImage(), m_backgroundImage );
}

} // namespace Cing