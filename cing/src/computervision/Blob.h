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

#ifndef _Blob_H_
#define _Blob_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "ComputerVisionPrereqs.h"
#include <vector>

// Open CV
#include "OpenCV/cv.h"

// Common
#include "common/CommonTypes.h"

namespace Cing
{

/**
 * This structure contains the data of a Blob in the scene.
 * A Blob is an element found in the scene (by computer vision analysis).
 */
struct Blob
{
	typedef std::vector<Cing::Point> BlobContourPoints; ///< Contour points of a blob

    // Constructors
    Blob( double _area, CvSeq* _contour ) : area( _area ), contour( _contour ) {}
		Blob(): area(0),contour( NULL ){}

    /// To compare two Blobs
    bool operator < ( const Blob& other ) const { return area < other.area; }
    bool operator > ( const Blob& other ) const { return area > other.area; }

    double	            area;		///< Blob's area
    CvRect		        bbox;		///< Blob's bounding box
    Point				center;		///< Center of the Blob
	BlobContourPoints	nodes;		///< Points of the Blob's contour

private:
    // This is done to avoid making contour attribute public
    friend class BlobFinder; 

    CvSeq*      contour;///< Internal stuff. OpenCv contour
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
	Vector  velocity;      // velocity = position-last_position
};

} // namespace Cing

#endif // _Blob_H_
