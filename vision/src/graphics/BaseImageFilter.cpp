/*
  This source file is part of the Vision project
  For the latest info, see http://www.playthemagic.com/vision

Copyright (c) 2008 Julio Obelleiro and Jorge Cano

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

#include "BaseImageFilter.h"

// OpenCv
#include "externLibs/OpenCV/cxcore/include/cxcore.h"

namespace Graphics
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
BaseImageFilter::BaseImageFilter():
  m_outputImage( NULL )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
BaseImageFilter::~BaseImageFilter()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @param[in] width     width of the images that the filter will work with
 * @param[in] height    height of the images that the filter will work with
 * @param[in] format		format of the image to analyze( RGB or GRAYSCALE)
 * @return true if the initialization was ok | false otherwise
 */
bool BaseImageFilter::init( int width, int height, GraphicsType format )
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Create the output image
	int nChannels = (int)Ogre::PixelUtil::getNumElemBytes( Ogre::PixelFormat( format ) ) ;
  m_outputImage = cvCreateImage( cvSize( width, height ), IPL_DEPTH_8U, nChannels );

  // Store values
  m_width     = width;
  m_height    = height;
  m_nChannels = nChannels;

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void BaseImageFilter::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  cvReleaseImage( &m_outputImage );

	// The class is not valid anymore
	m_bIsValid = false;
}

} // namespace Graphics