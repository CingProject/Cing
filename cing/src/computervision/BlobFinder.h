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

#ifndef _BlobFinder_H_
#define _BlobFinder_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "ComputerVisionPrereqs.h"
#include "Blob.h"
#include <vector>

// Graphics
#include "graphics/GraphicsTypes.h"
#include "graphics/Image.h"

namespace Cing
{


/**
 * @internal
 * Class that analyzes an image to finds blobs (elements).
 * What it looks for in an image is bright areas, so typically
 * the image result of a background subtraction is a good input.
 * It also gives information about the found blobs.
 */
class BlobFinder
{
public:

	// Public types
	typedef std::vector< Blob > Blobs; ///< Contains a sequence of blobs

	// Constructor / Destructor
	BlobFinder();
	~BlobFinder();

	// Init / Release
	virtual void	end           ();

	// Blob related methods
	virtual void	update				( const Image& inImage, bool sortBlobsBySize = true );

	// Set methods
	void			setMinBlobArea	( float minArea ) { m_minBlobArea = minArea; }
	void			setMaxBlobArea	( float maxArea ) { m_maxBlobArea = maxArea; }
	virtual void	setMaxBlobs   	( unsigned int maxBlobs )	{ m_maxBlobs = maxBlobs; }

	// Query methods
	virtual bool	isValid       () const { return m_bIsValid; }
	int				getNumBlobs   	() const { return m_nBlobs; }

	// TODO check n valid
	Blob&			getBlobN      ( int n )  { return m_blobs[n]; }

	// Constant / static attributes
	static const unsigned int DEFAULT_MAX_BLOBS;

protected:

	// Attributes
	Blobs				m_blobs;                ///< To store the found blobs
	int					m_nBlobs;
	float               m_minBlobArea;          ///< Min area (in pixels) that a blob should have to be considered
	float				m_maxBlobArea;          ///< Max area (in pixels) that a blob should have to be considered
	unsigned int		m_maxBlobs;             ///< Max number of blos to look for

	// OpenCV stuff
	std::vector<std::vector<cv::Point> >		m_contours;  ///< To store contour data (by opencv)

	bool									m_bIsValid;	///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _BlobFinder_H_
