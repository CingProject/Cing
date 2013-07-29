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

#include "Renderer2DOpenCV.h"
#include "OpenCVUtils.h"

// opencv
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// cing
#include "graphics/Image.h"

namespace Cing
{
	/**
	 *	Filters an image as defined by the received parameters. Availabe filters
	 *	BLUR, parameter: 
	 *
	 * @param image The image that will be filtered.
	 * @param kind	The type of filter to apply
	 * @param param	An optional parameter related to the type of filter to be applied. Some filters require parameters, others don't.
	 */ 
	void Renderer2DOpenCV::filter ( Image& image, ImageProcessingFilters kind, float param /*= FLT_MAX*/ )
	{
		// Check if the renderer is valid
		if ( !isValid() )
		{
			LOG_ERROR( "Renderer2DOpenCV has not been correctly initialized. Did you call the init() method?" );
			return;
		}

		// Check the image is valid
		if ( !image.isValid() )
		{
			LOG_ERROR( "Trying to filter in an invalid image! It should be initialized before by loading an image from disk, or just calling its init method." );
			return;
		}

		int intParam = (int)param;
		switch ( kind )
		{
			case BLUR:	
				if ( param == FLT_MAX )
					intParam = 3;
				cv::blur(toCVMat(image), toCVMat(image), cv::Size(intParam, intParam) );
			break;

			case ERODE:	
				if ( param == FLT_MAX )
					intParam = 1;
				cv::erode( toCVMat(image), toCVMat(image), cv::Mat(), cv::Point(-1, -1), intParam );
			break;

			case DILATE:	
				if ( param == FLT_MAX )
					intParam = 1;
				cv::dilate( toCVMat(image), toCVMat(image), cv::Mat(), cv::Point(-1, -1), intParam );
			break;

			case THRESHOLD:	
				if ( param != FLT_MAX )
					m_imgThresholdFilter.setThreshold(intParam);
				m_imgThresholdFilter.apply(  toCVMat(image), toCVMat(image) );
			break;

			case INVERT:	
				cv::invert( toCVMat(image), toCVMat(image) );
			break;

			case FLIP_X:	
				cv::flip( toCVMat(image), toCVMat(image), 0 );
			break;

			case FLIP_Y:	
				cv::flip( toCVMat(image), toCVMat(image), 1 );
			break;	

			case FLIP_XY:	
				cv::flip( toCVMat(image), toCVMat(image), -1 );
			break;

		};

		// Mark the image so that it updates contents to texture before being rendered (gpu)
		image.setUpdateTexture(true);
	}

}