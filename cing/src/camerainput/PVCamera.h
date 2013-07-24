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

#ifndef _PVCamera_H_
#define _PVCamera_H_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "CameraInputPrereqs.h"
#include "BaseCameraInput.h"

// PortVideo
#include "PortVideo/common/cameraTool.h"
#include "PortVideo/common/RingBuffer.h"
#include "PortVideo/common/FrameProcessor.h"

// STL
#include <string>

// PTypes
#undef nil
#include "PTypes/include/pasync.h"

namespace Cing
{

/**
 * @internal
 * Class to capture camera input based on Port Video library
 */
class PVCamera: public BaseCameraInput
{
public:

  // Structure to capture the image frames from the camera in a different thread
  struct PVCaptureThread: public pt::thread
  {
    PVCaptureThread ( PVCamera& pVCamera ): pt::thread( false ), m_PVCamera( pVCamera ) {};
    void execute    ();

    PVCamera&       m_PVCamera; ///< Camera that will retrieve the images captured in this thread
  };

	// Constructor / Destructor
	PVCamera();
	virtual ~PVCamera();

	// BaseCameraInput Interface
	virtual void  init   ( int deviceId, int width, int height, int fps, GraphicsType format );
	virtual void  end    ();
	virtual void  update ();

	// Query methods
	bool  isValid() const { return m_bIsValid; }
	

private:

	// Attributes
  	cameraEngine*     m_pvCamera;         ///< Object that allows access to the camera through port video library
  	RingBuffer*       m_ringBuffer;       ///< Circular buffer to store camera frames. Shared usage between this class and the PVCaptureThread class.
  	PVCaptureThread*  m_captureThread;    ///< Thread to capture images from the camera
	bool              m_bIsValid;	        ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _PVCamera_H_
