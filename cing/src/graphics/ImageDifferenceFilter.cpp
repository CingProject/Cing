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

#include "ImageDifferenceFilter.h"

// Common
#include "common/Exception.h"

// OpenCv
#include "OpenCV/cxcore.h"

namespace Cing
{

/**
 * @internal
 * Applies the image difference filter
 * @note the result is stored in m_outputImage of the base filter class
 * 
 * @param[in] inImage1 First image to calculate the difference
 * @param[in] inImage2 Second image to calculate the difference
 * @param[out] output	 Image where the result will be stored
 */
void ImageDifferenceFilter::apply( const IplImage& inImage1, const IplImage& inImage2, IplImage& output )
{
  // Check both images have same size and it is the same than the filter size
  if( (inImage1.imageSize != inImage2.imageSize) )
    THROW_EXCEPTION( "Trying to apply ImageDifferenceFilter to images with different size-> they must have the same size!" );

  // Calculate the between both images
  cvAbsDiff( &inImage1, &inImage2, &output );
}

} // namespace Cing