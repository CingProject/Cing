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

#ifndef _OCVCamera_H_
#define _OCVCamera_H_

#include "CameraInputPrereqs.h"
#include "BaseCameraInput.h"

// STL
#include <string>

// OpenCV
#include "OpenCV/cxtypes.h"

// Ogre
#include "OgreTexture.h"

// PTypes
#include "PTypes/include/pasync.h"

namespace Cing
{

/**
 * @internal
 * Class to capture camera input based on OpenCV library
 */
class OCVCamera: public BaseCameraInput
{
public:

  // structure to capture the image from the camera in a different thread
  struct OCVCaptureThread: public pt::thread
  {
    OCVCaptureThread( OCVCamera& ocvCamera ): pt::thread( false ), m_cvCaptureImage( NULL  ),m_ocvCamera( ocvCamera ) {};
    void execute();
    void cleanup();

    IplImage*       m_cvCaptureImage; ///< To store the camera image captured through openCv
    OCVCamera&      m_ocvCamera; 
  };

	// Constructor / Destructor
	 OCVCamera();
	virtual ~OCVCamera();

	// BaseCameraInput Interface
	virtual bool  init   ( int width = 320, int height = 240, int fps = 25 );
	virtual void  end    ();
	virtual void  update ();

	// Query methods
	bool  isValid() const { return m_bIsValid; }
	

private:

  // Private Methods
  void  createMesh();

  // Constant attributes
  static const std::string  MANUAL_OBJECT_NAME;   ///< Name of the manual object, which is the quad where the camera could be renderd
  static const std::string  MATERIAL_NAME;        ///< Name of the material where the camera images will be placed
  static const std::string  TEXTURE_NAME;         ///< Name of the texture where the camera images will be placed

	// Attributes
  CvCapture*        m_capture;        ///< OpenCV capture device  
  IplImage*         m_cvResizedImage; ///< Image to store the resized capture image from the camera -> to place it into a texture (power of 2)
  Ogre::Image*      m_ogreImage;      ///< Ogre image to place the captured texture -> to be able to draw it
  Ogre::TexturePtr  m_ogreTexture;    ///< Ogre texture -> to render the captured image
  OCVCaptureThread* m_captureThread;  ///< Thread to capture images from the camera
  pt::mutex         m_mutex;
	bool              m_bIsValid;	      ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _OCVCamera_H_
