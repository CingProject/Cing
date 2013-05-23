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

// Common
#include "common/Exception.h"
#include "common/MathUtils.h"

// opencv
#include "OpenCV/highgui.h"

// Graphics
#include "graphics/Image.h"
#include "graphics/ImageResourceManager.h"

namespace Cing
{

// Static member init
const unsigned int  BlobFinder::DEFAULT_MAX_BLOBS   = 20;


/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
BlobFinder::BlobFinder():
  m_findContoursStorage ( NULL  ),
  m_contour             ( NULL  ),
	m_nBlobs			( 0			),
  m_minBlobArea         ( -1 ),
  m_maxBlobArea         ( -1 ),
  m_maxBlobs            ( DEFAULT_MAX_BLOBS ),
  m_bIsValid            ( false )
{
	// Reserve memory
	m_findContoursStorage = cvCreateMemStorage();
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
  
  // Release data
  cvReleaseMemStorage( &m_findContoursStorage );

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
 */
void BlobFinder::update( const Image& inImage )
{
	// Check valid
	if ( !isValid() )
	THROW_EXCEPTION( "Trying to compute blobs, with the BlobFinder not initialized. Init method should be called" );

	// Check blob area... and if it has not been set, set it to the max and min (no lower than 10, to avoid opencv issues)
	if ( (m_minBlobArea < 0) || (m_maxBlobArea < 0) )
	{
		m_minBlobArea = 10;
		m_maxBlobArea = (float)inImage.getWidth() * (float)inImage.getHeight();
	}

	// Check both images have same size and it is the same than the filter size
	if( (inImage.getNChannels() != 1) && (inImage.getNChannels() != 3) )
	THROW_EXCEPTION( "Trying to compute blobs on images with non supporte format -> only RGB or GRAYSCALE images supported" );

	// Request temp image to work with
	IplImage* cvTempImage = ImageResourceManager::getSingleton().getImage( inImage.getWidth(), inImage.getHeight(), 1 );

	// If they have different number of channels -> convert them
	if ( inImage.getNChannels() == 3 )
		cvConvertImage( &inImage.getCVImage(), cvTempImage );
	// just one channel -> Copy the input image
	else 
		cvCopy( &inImage.getCVImage(), cvTempImage );

	// Find blobs (openCV contours)	
	int retrivalMode = CV_RETR_EXTERNAL; // CV_RETR_CCOMP
	cvFindContours( cvTempImage, m_findContoursStorage, &m_contour, sizeof(CvContour), retrivalMode, CV_CHAIN_APPROX_SIMPLE );

	// Extract found contours    

	// Iterate through found contours and store them..
	m_blobs.clear();
	for( ; m_contour != 0; m_contour = m_contour->h_next )
	{
	// Get contour area
	double area = fabs( cvContourArea( m_contour, CV_WHOLE_SEQ ) );

	// If it has a good size (between min and max)
	if ( ( area > m_maxBlobArea ) || ( area < m_minBlobArea ) )
	  continue;

	// Store new Blob
	m_blobs.push_back( Blob( area, m_contour ) );
	}

	// Release temp image
	ImageResourceManager::getSingleton().releaseImage( cvTempImage );

	// Extract information of found blobs
	extractBlobsInformation();

	// Clear OpenCV contours storage 
	cvClearMemStorage( m_findContoursStorage );
}


/**
 * @internal
 * Extracts the relevant information of the found blobs
 * @note when this method is called, the found blobs should be stored in m_blobs member
 */
void BlobFinder::extractBlobsInformation()
{
  // Order blobs (from bigger to smaller) -> this way the most relevant are at the beginning
  std::sort( m_blobs.begin(), m_blobs.end(), std::greater< Blob >() );

  // Discard blobs (if there is more than the max)
  // TODO

  // To store contour moments
  CvMoments moment;

  // to read contour points
	CvSeqReader contourReader;
	CvPoint		  contourNode;

  // Calculate information about contours
  for( Blobs::size_type i = 0; (i < m_blobs.size()) && (i < m_maxBlobs); ++i )
  {
      // Current blob
      Blob& blob = m_blobs[ i ];

      // Get bbox
      blob.bbox = cvBoundingRect( blob.contour );

      // Get center through moments
      cvMoments( blob.contour, &moment );
      blob.center.x = (float)(moment.m10 / moment.m00);
      blob.center.y = (float)(moment.m01 / moment.m00);

      // Invert Y coordinate because our Y 0 is at top of the image,
      // and for Opencv is at the bottom of the image
      //blob.center.Y = inImage.GetHeight() - blob.center.Y;

    // Store the contour nodes
    cvStartReadSeq( blob.contour, &contourReader );
    for( int j = 0; j < blob.contour->total; ++j )
    {
	    // Read node of the contour
	    CV_READ_SEQ_ELEM( contourNode, contourReader );
	    blob.nodes.push_back( Point( (float)contourNode.x, (float)contourNode.y , 0) );
    }
  }

	// Store number of actual blobs
	m_nBlobs = min( (int)m_blobs.size(), (int)m_maxBlobs );
}

} // namespace Cing