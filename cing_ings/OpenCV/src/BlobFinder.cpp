/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community 

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

#include "BlobFinder.h"
#include "OpenCVUtils.h"
#include "ImageResourceManager.h"

// Common
#include "common/Exception.h"
#include "common/MathUtils.h"

// opencv
#include "opencv2/core/core.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"

// Graphics
#include "graphics/Image.h"

namespace Cing
{

// Static member init
const unsigned int  BlobFinder::DEFAULT_MAX_BLOBS   = 20;


/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
BlobFinder::BlobFinder():
	m_nBlobs			( 0			),
	m_minBlobArea       ( -1 ),
	m_maxBlobArea       ( -1 ),
	m_maxBlobs          ( DEFAULT_MAX_BLOBS ),
	m_bIsValid          ( false )
{
	// Reserve memory
	m_bIsValid = true;
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
BlobFinder::~BlobFinder()
{
  // Release resources
  end();
}


/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void BlobFinder::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;
 
	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Find the blobs in the received image.
 * What it looks for in an image is bright areas, so typically 
 * the image result of a background subtraction is a good input.
 * 
 * @param[in] inImage image where the blobs will be searched
 * @param[in] sortBlobsBySize if true, the found blobs are sorted by size (bigger to smaller, in terms of area)
 */
void BlobFinder::update( const Image& inImage, bool sortBlobsBySize /*= true*/ )
{
	// Check valid
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to compute blobs, with the BlobFinder not initialized. Init method should be called" );
		return;
	}

	// Check blob area... and if it has not been set, set it to the max and min (no lower than 10, to avoid opencv issues)
	if ( (m_minBlobArea < 0) || (m_maxBlobArea < 0) )
	{
		m_minBlobArea = 10;
		m_maxBlobArea = (float)inImage.getWidth() * (float)inImage.getHeight();
	}

	// Check the image has 1 or 3 channels
	if( (inImage.getNChannels() != 1) && (inImage.getNChannels() != 3) )
	{
		LOG_ERROR( "Trying to compute blobs on images with non supporte format -> only RGB or GRAYSCALE images supported" );
		return;
	}


	// Contour finding needs a one channel image
	cv::Mat* cvTempImage =  ImageResourceManager::getSingleton().getImage( inImage.getWidth(), inImage.getHeight(), 1 );; 

	// If it has 3 channels, copy it to a one channel tempm image
	if ( inImage.getNChannels() == 3 )
	{
		cv::cvtColor( toCVMat(inImage), *cvTempImage, CV_RGB2GRAY );
	}
	// just one channel -> just point the temp image to the actual one, no need for conversion
	else 
	{
		*cvTempImage = toCVMat(inImage).clone();
	}

	// Find blobs (openCV contours)	
	m_contours.clear();
	int retrivalMode = CV_RETR_EXTERNAL; // CV_RETR_CCOMP
	cv::findContours( *cvTempImage, m_contours, retrivalMode, CV_CHAIN_APPROX_SIMPLE );

	// Extract found contours    
	m_blobs.clear();
	for( size_t i = 0; i< m_contours.size(); i++ )
	{
		// Get contour area
		double area = fabs( cv::contourArea( m_contours.at(i) ) );

		// If it has a good size (between min and max)
		if ( ( area > m_maxBlobArea ) || ( area < m_minBlobArea ) )
		  continue;

		// Store new Blob
		m_blobs.push_back( Blob( area ) );

		// Extract more blob info
		Blob& newBlob = m_blobs.back();

		// copy contour
		newBlob.contour.resize( m_contours.at(i).size() );
		std::copy( m_contours.at(i).begin(), m_contours.at(i).end(), newBlob.contour.begin() );

		// bbox
		newBlob.bbox = cv::boundingRect( m_contours.at(i) );

		// center
		cv::Moments moments = cv::moments( m_contours.at(i) );
		newBlob.center.x = (float)(moments.m10 / moments.m00);
		newBlob.center.y = (float)(moments.m01 / moments.m00);
		newBlob.center.z = 0.0f;
	}

	// Release temp image 
	ImageResourceManager::getSingleton().releaseImage( cvTempImage );

	// Order blobs (from bigger to smaller) -> this way the most relevant are at the beginning
	std::sort( m_blobs.begin(), m_blobs.end(), std::greater< Blob >() );

	// Store number of actual blobs
	m_nBlobs = min( (int)m_blobs.size(), (int)m_maxBlobs );
}

} // namespace Cing