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