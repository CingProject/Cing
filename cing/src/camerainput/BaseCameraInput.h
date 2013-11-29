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

#ifndef _BaseCameraInput_H_
#define _BaseCameraInput_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "CameraInputPrereqs.h"
#include "common/MathUtils.h"

// Graphics
#include "graphics/Image.h"
#include "graphics/GraphicsTypes.h"

// Ogre
#include "OgreTimer.h"

namespace Cing
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
		virtual bool    init            ( int deviceId = 0, int width = 320, int height = 240, int fps = 25, GraphicsType format = RGB, bool multithreaded = true );
		virtual void    end             ();
		virtual void    update          () = 0;

		// Misc
		void			draw		();
		void			showFps	( bool showFps ) { m_showFps = showFps; }

		// Query methods
		bool            isValid			() const { return m_bIsValid;   }
		bool            haveNewFrame    () const { return m_newFrame;   }
		int             getWidth        () const { return m_width;      }
		int             getHeight       () const { return m_height;     }
		int             getFPS          () const { return m_fps;        }
		int             getNumChannels  () const { return m_nChannels;  }
		int             getDeviceId		() const { return m_deviceId;  }
		Image&			getImage        ()		 { return m_currentCameraImage; }
		GraphicsType	getFormat		() const { return m_format; }

	protected:

		// Protected methods
		void  			setNewFrameData ( unsigned char* data, unsigned int width, unsigned int height, GraphicsType format, int widthStep, bool swapRB = false );
		void  			setNewFrame     ( bool newFrame ) { m_newFrame = newFrame;    }  
		void  			setWidth        ( int width   )   { m_width   = width;  }
		void  			setHeight       ( int height  )   { m_height  = height; }
		void  			setFPS          ( int fps     )   { m_fps   = fps;  } 

		// Protected attributes
		int				m_deviceId;					///< Id of the device to capture from (starting at 0).

	private:
		// Attributes
		Ogre::Timer		m_timer;					///< Timer to check the capture fps
		AverageDouble	m_realFpsAverage;			///< Real fps average
		double			m_realFps;					///< Real captured fps
		Image			m_currentCameraImage; 		///< Image captured from the camera 
		Image			m_tempImage;				///< Image used to convert received frames to working format
		int             m_width, m_height;    		///< Capture resolution
		int             m_fps;                		///< Capture frames per second
		int             m_frameSize;          		///< Size in bytes of a single frame
		int             m_nChannels;          		///< Number of channels of the captured image
		GraphicsType	m_format;					///< Format of the frames (RGB or GRAYSCALE)
		bool            m_newFrame;           		///< True when the camera has a new frame captured
		bool            m_bIsValid;	          		///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
		bool			m_showFps;					///< Show real capture fps

	};

} // namespace Cing

#endif // _BaseCameraInput_H_