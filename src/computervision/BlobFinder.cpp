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

#include "BlobFinder.h"

// Common
#include "common/Exception.h"

// opencv
#include "externLibs/OpenCV/highgui/include/highgui.h"

namespace ComputerVision
{

// Static member init
const float         BlobFinder::DEFAULT_MIN_AREA    = 200;
const float         BlobFinder::DEFAULT_MAX_AREA    = 320 * 240 * 0.75f;
const unsigned int  BlobFinder::DEFAULT_MAX_BLOBS   = 20;


/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
BlobFinder::BlobFinder():
  m_findContoursStorage ( NULL  ),
  m_contour             ( NULL  ),
  m_cvImage             ( NULL  ),
  m_minBlobArea         ( DEFAULT_MIN_AREA ),
  m_maxBlobArea         ( DEFAULT_MAX_AREA ),
  m_maxBlobs            ( DEFAULT_MAX_BLOBS ),
  m_bIsValid            ( false )
{
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
 * @brief Initializes the class so it becomes valid.
 *
 * @param[in] width     with of the image to analyze
 * @param[in] height    height of the image to analyze
 * @return true if the initialization was ok | false otherwise
 */
bool BlobFinder::init( int width, int height )
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Allocate memory for the contours and image
  m_findContoursStorage = cvCreateMemStorage();
  m_cvImage             = cvCreateImage( cvSize( width, height ), IPL_DEPTH_8U, 1 );

	// The class is now initialized
	m_bIsValid = true;

	return true;
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
  cvReleaseImage( &m_cvImage );

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
void BlobFinder::computeBlobs( const IplImage& inImage )
{
  // Check valid
  if ( !isValid() )
    THROW_EXCEPTION( "Trying to compute blobs, with the BlobFinder not initialized. Init method should be called" );

  // Check both images have same size and it is the same than the filter size
  if( (inImage.nChannels != 1) && (inImage.nChannels != 3) )
    THROW_EXCEPTION( "Trying to compute blobs on images with non supporte format -> only RGB or GRAY images supported" );

	// If they have different number of channels -> convert them
	if ( inImage.nChannels == 3 )
		cvConvertImage( &inImage, m_cvImage );
  // just one channel -> Copy the input image
	else 
		cvCopy( &inImage, m_cvImage );

  // Find blobs (openCV contours)	
  int retrivalMode = CV_RETR_EXTERNAL; // CV_RETR_CCOMP
  cvFindContours( m_cvImage, m_findContoursStorage, &m_contour, sizeof(CvContour), retrivalMode, CV_CHAIN_APPROX_SIMPLE );

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


  // Extract information of found blobs
  extractBlobsInformation();
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

  // Discard blobs (if there is more than the max
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
      blob.center.x = static_cast< int >( moment.m10 / moment.m00 );
      blob.center.y = static_cast< int >( moment.m01 / moment.m00 );

      // Invert Y coordinate because our Y 0 is at top of the image,
      // and for Opencv is at the bottom of the image
      //blob.center.Y = inImage.GetHeight() - blob.center.Y;

    // Store the contour nodes
    cvStartReadSeq( blob.contour, &contourReader );
    for( int i = 0; i < blob.contour->total; ++i )
    {
	    // Read node of the contour
	    CV_READ_SEQ_ELEM( contourNode, contourReader );
	    blob.nodes.push_back( Pos2D( contourNode.x, contourNode.y ) );
    }
  }

}

} // namespace ComputerVision