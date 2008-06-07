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

#ifndef _Blob_H_
#define _Blob_H_

#include "ComputerVisionPrereqs.h"
#include <vector>

// Open CV
#include "externLibs/OpenCV/cv/include/cv.h"

// Common
#include "common/CommonTypes.h"

namespace ComputerVision
{

/**
 * This structure contains the data of a Blob in the scene.
 * A Blob is an element found in the scene (by computer vision analysis).
 */
struct Blob
{
	typedef std::vector<Point> BlobContourPoints; ///< Contour points of a blob

    // Constructor
    Blob( float _area, CvSeq* _contour ) : area( _area ), contour( _contour ) {}

    /// To compare two Blobs
    bool operator < ( const Blob& other ) const { return area < other.area; }
    bool operator > ( const Blob& other ) const { return area > other.area; }

    float	            area;		  ///< Blob's area
    CvRect		        bbox;		  ///< Blob's bounding box
    Point			        center;		///< Center of the Blob
	  BlobContourPoints nodes;		///< Points of the Blob's contour

private:
    // This is done to avoid making contour attribute public
    friend class BlobFinder; 

    CvSeq*      contour;///< Internal stuff. OpenCv contour
};


} // namespace ComputerVision

#endif // _Blob_H_
