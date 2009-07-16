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

#include "BackgroundSubtraction.h"

// Common
#include "common/CommonUtilsIncludes.h"

// Graphics
#include "graphics/ImageResourceManager.h"

// OpenCv
#include "OpenCV/cxcore/include/cxcore.h"

namespace Cing
{

// Static member init
const int BackgroundSubtraction::DEFAULT_THRESHOLD = 50;

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
BackgroundSubtraction::BackgroundSubtraction():
	m_backgroundImage	( NULL )
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

	// Calculate the difference between the background and the current image
	m_differenceFilter.apply( *m_backgroundImage, imgToAnalyze.getCVImage(), *tempImage );

	// Threshold the image
	m_thresholdFilter.apply( *tempImage, output.getCVImage() );

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
		// Release old image (if it exists) and create the new one
		cvReleaseImage( &m_backgroundImage );
		m_backgroundImage = cvCloneImage( &backgroundImage.getCVImage() );
	}
	// If we have an ok image -> just copy the data
	else
		cvCopy( &backgroundImage.getCVImage(), m_backgroundImage );
}

} // namespace Cing