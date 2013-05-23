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

#include "ImageThresholdFilter.h"

// Common
#include "common/Exception.h"

// OpenCv
#include "OpenCV/cxcore.h"
#include "OpenCV/cv.h"

namespace Cing
{

/**
 * @internal
 * Applies a threshold filter to an image -> thus polarizing it
 * @note the result is stored in m_outputImage of the base filter class
 * 
 * @param[in] inImage Image that will be thresholded (polarized)
 * @param[out] output	 Image where the result will be stored
 */
void ImageThresholdFilter::apply( const IplImage& inImage, IplImage& output )
{
  // Check both images have same size and it is the same than the filter size
  if( (inImage.imageSize != output.imageSize) )
    THROW_EXCEPTION( "Trying to apply ImageThresholdFilter to images with different size-> they must have the same size!" );

  // Calculate the between both images
  cvThreshold( &inImage, &output, m_threshold, 255, CV_THRESH_BINARY );
}

} // namespace Cing