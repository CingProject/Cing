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

#include "BaseImageFilter.h"

// OpenCv
#include "opencv2/core/core.hpp"

namespace Cing
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
  m_outputImage = new cv::Mat( height, width, CV_MAKETYPE(CV_8U, nChannels) );

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

  delete m_outputImage;

	// The class is not valid anymore
	m_bIsValid = false;
}

} // namespace Cing