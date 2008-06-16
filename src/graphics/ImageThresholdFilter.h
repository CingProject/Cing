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

#ifndef _ImageThresholdFilter_H_
#define _ImageThresholdFilter_H_

#include "GraphicsPrereqs.h"
#include "BaseImageFilter.h"

namespace Graphics
{

/**
 * @internal
 * Applies a fixed-level threshold filter to an image -> thus polarizing it. This is usually used to 
 * get bi-level (binary) image out of grayscale image
 */
class ImageThresholdFilter: public BaseImageFilter
{
public:

	ImageThresholdFilter(): m_threshold( 50 ) {}

  // Apply the filter (the result is stored in m_outputImage of the base filter class)
  void  apply        ( const IplImage& inImage, IplImage& output );

  // Set image threshold value
  void  setThreshold ( int threshold ) { m_threshold = threshold; }


private:
  int   m_threshold;///< Threshold to apply to the input image
};


} // namespace Graphics

#endif // _ImageThresholdFilter_H_
