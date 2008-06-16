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

#include "ImageThresholdFilter.h"

// Common
#include "common/Exception.h"

// OpenCv
#include "externLibs/OpenCV/cxcore/include/cxcore.h"
#include "externLibs/OpenCV/cv/include/cv.h"

namespace Graphics
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
  // Check valid filter
  //if ( !isValid() )
  //  THROW_EXCEPTION( "Trying to apply ImageThresholdFilter, with filter not initialized. Init method should be called" );

  // Check both images have same size and it is the same than the filter size
  if( (inImage.imageSize != output.imageSize) )
    THROW_EXCEPTION( "Trying to apply ImageThresholdFilter to images with different size-> they must have the same size!" );

  // Calculate the between both images
  cvThreshold( &inImage, &output, m_threshold, 255, CV_THRESH_BINARY );
}

} // namespace Graphics