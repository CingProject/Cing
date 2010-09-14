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

#ifndef _BlobTracker_H_
#define _BlobTracker_H_

#include "computerVision/BlobFinder.h"
#include "computerVision/Blob.h"

namespace Cing
{

/**
 * @internal
 * Class that analyzes an image to track blobs (elements).
 * What it looks for in an image is bright areas, so typically
 * the image result of a background subtraction is a good input.
 * It also gives information about the found blobs .
 */
class BlobTracker : public BlobFinder
{
public:
	// Public types
	typedef std::vector< TrackedBlob > TrackedBlobs; ///< Contains a sequence of blobs

	// Constructor / Destructor
	BlobTracker();
	virtual ~BlobTracker();
	// Query methods
	bool          isValid       () const { return m_bIsValid; }
	// Init / Release
	void          end           ();
	// Blob related methods
	void          update				( const Image& inImage );

	// TODO check n valid
	TrackedBlob&	getTrackedBlobN      ( int n )  { return m_trackedBlobs[n]; }

	// Get / Set methods
  virtual void  setMaxBlobs   ( int maxBlobs );

	void setCallback(void (*callback)){ m_callback = callback; };

private:
	// Attributes
	TrackedBlobs      m_trackedBlobs;                ///< To store the found blobs
	int								m_nTrackedBlobs;

	void*							m_callback;
	bool							m_bIsValid;


};

} // namespace Cing

#endif // _BlobTracker_H_
