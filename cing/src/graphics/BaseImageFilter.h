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

#ifndef _BaseImageFilter_H_
#define _BaseImageFilter_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "GraphicsPrereqs.h"

// Graphics
#include "graphics/GraphicsTypes.h"

// OpenCV
#include "OpenCV/cxtypes.h"


namespace Cing
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
  bool          init            ( int width, int height, GraphicsType format );
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

} // namespace Cing

#endif // _BaseImageFilter_H_


