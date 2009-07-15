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

#ifndef _BaseCameraInput_H_
#define _BaseCameraInput_H_

#include "CameraInputPrereqs.h"
#include "common/MathUtils.h"

// OpenCV
#include "OpenCV/cxcore/include/cxtypes.h"

// Graphics
#include "graphics/Image.h"
#include "graphics/GraphicsTypes.h"

namespace CameraInput
{

/**
 * @internal
 * Base class for all the camera input (capture) devices.
 */
class BaseCameraInput
{
public:

	// Constructor / Destructor
  BaseCameraInput ();
  virtual ~BaseCameraInput();
  
	// Init / Release / Update (to be implemented in subclasses)
  virtual void    init            ( int deviceId = 0, int width = 320, int height = 240, int fps = 25, Graphics::GraphicsType format = Graphics::RGB, bool multithreaded = true );
	virtual void    end             ();
  virtual void    update          () = 0;

  // Misc
  void							draw		();
	void							showFps	( bool showFps ) { m_showFps = showFps; }

	// Query methods
  bool            				isValid			() const { return m_bIsValid;   }
  bool            				haveNewFrame    () const { return m_newFrame;   }
  int             				getWidth        () const { return m_width;      }
  int             				getHeight       () const { return m_height;     }
  int             				getFPS          () const { return m_fps;        }
  int             				getNumChannels  () const { return m_nChannels;  }
  Graphics::Image&				getImage        ()		 { return m_currentCameraImage; }
  Graphics::GraphicsType			getFormat		() const { return m_format; }
	 
protected:

  // Protected methods
  void  setNewFrameData         ( char* data, unsigned int width, unsigned int height, Graphics::GraphicsType format );
  void  setNewFrame             ( bool newFrame ) { m_newFrame = newFrame;    }  
  void  setWidth                ( int width   )   { m_width   = width;  }
  void  setHeight               ( int height  )   { m_height  = height; }
  void  setFPS                  ( int fps     )   { m_fps   = fps;  } 

	// Protected attributes
	int						m_deviceId;					///< Id of the device to capture from (starting at 0).

private:
	// Attributes
	Ogre::Timer				m_timer;					///< Timer to check the capture fps
	Average					m_realFpsAverage;			///< Real fps average
	double					m_realFps;					///< Real captured fps
	Graphics::Image			m_currentCameraImage; 		///< Image captured from the camera 
	Graphics::Image			m_tempImage;				///< Image used to convert received frames to working format
	int                     m_width, m_height;    		///< Capture resolution
	int                     m_fps;                		///< Capture frames per second
	int                     m_frameSize;          		///< Size in bytes of a single frame
	int                     m_nChannels;          		///< Number of channels of the captured image
	Graphics::GraphicsType	m_format;					///< Format of the frames (RGB or GRAYSCALE)
	bool                    m_newFrame;           		///< True when the camera has a new frame captured
	bool                    m_bIsValid;	          		///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
	bool					m_showFps;					///< Show real capture fps

};

} // namespace CameraInput

#endif // _BaseCameraInput_H_
