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

#ifndef _ImageThresholdFilter_H_
#define _ImageThresholdFilter_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "GraphicsPrereqs.h"

namespace Cing
{

/**
 * @internal
 * Applies a fixed-level threshold filter to an image -> thus polarizing it. This is usually used to 
 * get bi-level (binary) image out of grayscale image
 */
class ImageThresholdFilter
{
public:

	ImageThresholdFilter(): m_threshold( 50 ) {}

  // Apply the filter (the result is stored in m_outputImage of the base filter class)
  void  apply        ( const _IplImage& inImage, _IplImage& output );

  // Set image threshold value
  void  setThreshold ( int threshold ) { m_threshold = threshold; }


private:
  int   m_threshold;///< Threshold to apply to the input image
};


} // namespace Cing

#endif // _ImageThresholdFilter_H_
