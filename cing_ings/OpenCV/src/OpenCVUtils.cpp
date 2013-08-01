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

#include "graphics/Image.h"
#include "OpenCVUtils.h"

namespace Cing
{

	/** 
	 * Utility to conver a Cing::Image to a cv::MAt. 
	 * The image buffer is not copied, only the cv::Mat header is created and it points to the Cing::image data, so it's a fast operation.
	 * No need to release memory after using the cv::Mat
	 *
	 * @note After operating with the returned cv::Mat, if the image buffer is altered, setUpdateTexture(true) should be called
	 * if you want to make sure that the texture associated with the Cing::Image is updated before the next draw/render call.
	 *
	 * @param image Image to create a cv::Mat with
	 * @return cv::Mat wrapping the received Cing::Image. 
	 */
	cv::Mat toCVMat( const Image& image )
	{
		// See the incoming image format to find out the equivalent in opencv
		GraphicsType	format		= image.getFormat();
		int				formatCV	= 0;

		switch (format)
		{
			// color image
			case RGB:
			case BGR:
				formatCV = CV_8UC3;
			break;

			// color + alpha image
			case RGBA:
			case BGRA:
				formatCV = CV_8UC4;
			break;

			// grayscale (single channel) image
			case GRAYSCALE:
				formatCV = CV_8UC1;
			break;
		};

		// Create a header for it and return it
		cv::Mat newMat( image.getHeight(), image.getWidth(), formatCV, (void*)image.getData() );
		return newMat;
	}

}