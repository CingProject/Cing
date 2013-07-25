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

#ifndef _Blob_H_
#define _Blob_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "ComputerVisionPrereqs.h"
#include <vector>

// Common
#include "common/CommonTypes.h"

// OpenCV
#include "opencv2/core/core.hpp"

namespace Cing
{

/**
 * This structure contains the data of a Blob in the scene.
 * A Blob is an element found in the scene (by computer vision analysis).
 */
struct Blob
{
	typedef std::vector<cv::Point> BlobContourPoints; ///< Contour points of a blob

    // Constructors
    Blob( double _area ) : area( _area ) {}
		Blob(): area(0){}

    /// To compare two Blobs
    bool operator < ( const Blob& other ) const { return area < other.area; }
    bool operator > ( const Blob& other ) const { return area > other.area; }

    double	            area;		///< Blob's area
    CvRect		        bbox;		///< Blob's bounding box
    Point				center;		///< Center of the Blob
	BlobContourPoints	contour;		///< Points of the Blob's contour

	//CvSeq*      contour;///< Internal stuff. OpenCv contour

private:
    // This is done to avoid making contour attribute public
    friend class BlobFinder; 


};

/**
 * This structure contains the data of a TrackedBlob in the scene.
 * A Blob is an element with unique Id assigned, found in the scene (by computer vision analysis).
 */
struct TrackedBlob
{
	// Constructor
	TrackedBlob( int _id) : id( _id ), isActive(false){}

	Blob		blob;
	int			id;            // id of contour
	float		energy;				 // oldest blobs is more energized than other 
	bool		isActive;		   // is active?
	Vector		velocity;      // velocity = position-last_position
};

} // namespace Cing

#endif // _Blob_H_
