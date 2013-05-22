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

#ifndef _BlobTracker_H_
#define _BlobTracker_H_

// Precompiled headers
#include "Cing-Precompiled.h"


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
