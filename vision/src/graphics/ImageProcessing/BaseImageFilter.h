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

#ifndef _BaseImageFilter_H_
#define _BaseImageFilter_H_

#include "..\GraphicsPrereqs.h"

// Graphics
#include "graphics/GraphicsTypes.h"

// OpenCV
#include "externLibs/OpenCV/cxcore/include/cxtypes.h"


namespace Graphics
{

/**
 * @internal
 * This class defines the interface for the image filters of the computer vision system
 */
class BaseImageFilter
{
public:

	// Constructor / Destructor
  BaseImageFilter();
  virtual ~BaseImageFilter();

  // Init / Release
  bool          init            ( int width, int height, ImageFormat format );
  void          end             ();

  // Query methods
  bool          isValid         () const  { return m_bIsValid;    } 
  IplImage&     getOutputImage  () const { return *m_outputImage; }

protected:
  IplImage*   m_outputImage;      ///< The result of the filters application will be stored in this image
  int         m_width, m_height;  ///< Resolution of the images that the filter will work with
  int         m_nChannels;        ///< Number of channels of the images that the filter will work with (RGB -> 3 channels, Grayscale -> 1 channel)
  bool        m_bIsValid;	        ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
};

} // namespace Graphics

#endif // _BaseImageFilter_H_


