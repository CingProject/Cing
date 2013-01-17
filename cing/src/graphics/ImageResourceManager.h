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

#ifndef _ImageResourceManager_H_
#define _ImageResourceManager_H_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "GraphicsPrereqs.h"

// OpenCv includes
#include "OpenCV/cxtypes.h"

// Common
#include "common/Singleton.h"

namespace Cing
{

struct ImageResource
{
	ImageResource( IplImage* pImage ): image( pImage ), available( true ) {}

	IplImage* image;
	bool			available;
};

/**
 * @internal
 * Manager of IplImages to optimize performance when temporal images are required
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
	IplImage*				getImage		(int width, int height, int channels) ;
	void					releaseImage	( IplImage* image );

private:

	// private constructor to ensure singleton
	ImageResourceManager();

	// Attributes
	std::vector< ImageResource >	m_imagePool[3];	///< First vector stores 1 channel images and second stores 3 channel images, and then of 4 channels
	bool							m_bIsValid;	    ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called
	};

} // namespace Cing

#endif // _ImageResourceManager_H_
