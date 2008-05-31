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

#ifndef _ImageDifferenceFilter_H_
#define _ImageDifferenceFilter_H_

#include "../ComputerVisionPrereqs.h"
#include "BaseImageFilter.h"

namespace ComputerVision
{

/**
 * @internal
 * Computes the difference between two images
 */
class ImageDifferenceFilter: public BaseImageFilter
{
public:

  // Apply the filter (the result is stored in m_outputImage of the base filter class)
  void apply( const IplImage& inImage1, const IplImage& inImage2 );
};


} // namespace ComputerVision

#endif // _ImageDifferenceFilter_H_
