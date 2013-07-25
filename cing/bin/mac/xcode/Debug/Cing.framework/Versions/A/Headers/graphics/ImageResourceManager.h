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

#ifndef _ImageResourceManager_H_
#define _ImageResourceManager_H_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "GraphicsPrereqs.h"

// Common
#include "common/Singleton.h"

// forward declaration
namespace cv
{
	class Mat;
}

namespace Cing
{

struct ImageResource
{
	ImageResource( cv::Mat* pImage ): image( pImage ), available( true ) {}

	cv::Mat*		image;
	bool			available;
};

/**
 * @internal
 * Manager of cv::Mat objects (images) to optimize performance when temporal images are required
 * It maintains a pool of images of difference sizes.
 *
 * @note Important: The images returned have the purpose to be used as temporal working images
 * but they should not be stored nor deleted (as they can be served to another requester in the next frame).
 *
 * @note Currently, width default settings, it consumes around 5Mb or ram memory, but can increase performance
 * and memory consumption significantly in an application with a high rate of image processing or computer vision processes
 */
class ImageResourceManager: public SingletonStatic< ImageResourceManager >
{

public:

	// Required for singleton
	friend class SingletonStatic< ImageResourceManager >;

	// Destructor
	virtual ~ImageResourceManager			();

	// Init / Release
	bool                    init            ();
	void                    end             ();

	// Query methods
	bool					isValid         () const { return m_bIsValid; }
	cv::Mat*				getImage		(int width, int height, int channels) ;
	void					releaseImage	( cv::Mat* image );

private:

	// private constructor to ensure singleton
	ImageResourceManager();

	// Attributes
	std::vector< ImageResource >	m_imagePool[3];	///< First vector stores 1 channel images and second stores 3 channel images, and then of 4 channels
	bool							m_bIsValid;	    ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called
	};

} // namespace Cing

#endif // _ImageResourceManager_H_
