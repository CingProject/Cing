/*
  This source file is part of the Vision project
  For the latest info, see http://www.playthemagic.com/vision

Copyright (c) 2008 Julio Obelleiro and Jorge Cano

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

#ifndef _PVCamera_H_
#define _PVCamera_H_

#include "CameraInputPrereqs.h"
#include "BaseCameraInput.h"

// PortVideo
#include "externLibs/PortVideo/common/cameraEngine.h"
#include "externLibs/PortVideo/common/cameraTool.h"
#include "externLibs/PortVideo/common/RingBuffer.h"
#include "externLibs/PortVideo/common/FrameProcessor.h"

// STL
#include <string>

// OpenCV
#include "externLibs/OpenCV/cxcore/include/cxtypes.h"

// PTypes
#include "externLibs/PTypes/include/pasync.h"

namespace CameraInput
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
	virtual void  init   ( int deviceId, int width, int height, int fps, ImageFormat format );
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

} // namespace CameraInput

#endif // _PVCamera_H_
