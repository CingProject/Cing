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

#include "OCVCamera.h"

// Ogre
#include "Ogre3d/include/OgrePixelFormat.h"
#include "Ogre3d/include/OgreImage.h"
#include "Ogre3d/include/OgreSceneManager.h"
#include "Ogre3d/include/OgreManualObject.h"
#include "Ogre3d/include/OgreRenderOperation.h"
#include "Ogre3d/include/OgreHardwarePixelBuffer.h"
#include "Ogre3d/include/OgreTexture.h"
#include "Ogre3d/include/OgreMaterial.h"
#include "Ogre3d/include/OgreMaterialManager.h"

// Graphics
#include "graphics/GraphicsManager.h"

// OpenCv
#include "OpenCV/cv.h"
#include "OpenCV/highgui.h"

// Common
#include "common/Exception.h"
#include "common/Release.h"
#include "common/LogManager.h"

namespace Cing
{

// Static member initialization
  const std::string  OCVCamera::MANUAL_OBJECT_NAME  = "OCVQuad";
  const std::string  OCVCamera::MATERIAL_NAME       = "OCVMaterial";
  const std::string  OCVCamera::TEXTURE_NAME        = "OCVTexture";


void OCVCamera::OCVCaptureThread::execute()
{
  while( !get_signaled() )
  {

    // Get camera frame from OpenCV -> Note: this is a blocking calle
    m_cvCaptureImage = cvQueryFrame( m_ocvCamera.m_capture );
    if ( !m_cvCaptureImage )
      THROW_EXCEPTION( "Error getting frame from camera." )

    //m_ocvCamera.m_mutex.lock();

    // Copy the image
    cvCopy( m_cvCaptureImage, m_ocvCamera.m_cvResizedImage );

    // New frame available
    m_ocvCamera.setNewFrame( true );

    //m_ocvCamera.m_mutex.unlock();

  }
}

void OCVCamera::OCVCaptureThread::cleanup()
{
}


/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
OCVCamera::OCVCamera():
  m_capture         ( NULL  ),
  m_cvResizedImage  ( NULL  ),
  m_ogreImage       ( NULL  ),
  m_captureThread   ( NULL  ),
  m_bIsValid        ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
OCVCamera::~OCVCamera()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @param[in] width   Width resolution to capture form camera
 * @param[in] height  Height resolution to capture form camera
 * @param[in] fps     FPS to capture form camera
 * @return true if the initialization was ok | false otherwise
 */
bool OCVCamera::init( int width /*= 320*/, int height /*= 240*/, int fps /*= 25*/ )
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Init capture (from any camera)
  m_capture = cvCaptureFromCAM( 0 );
  if ( !m_capture )
    THROW_EXCEPTION( "No camera found, or no drivers installed" );

  // Set camera res
  cvSetCaptureProperty( m_capture, CV_CAP_PROP_FRAME_WIDTH, width );
  cvSetCaptureProperty( m_capture, CV_CAP_PROP_FRAME_HEIGHT, height );

  // Set FPS
  cvSetCaptureProperty( m_capture, CV_CAP_PROP_FPS, 25 );

  // Create image to store frames in gray scale and color
  // TODO
  //m_frameGray = cvCreateImage( cvSize(width,height), IPL_DEPTH_8U, 1 );

  // Create image to store resized captured image (to fit in a texture -> power of 2)
  m_cvResizedImage = cvCreateImage( cvSize(width, height), IPL_DEPTH_8U, 3 );

  // Create the ogre image
  m_ogreImage = new Ogre::Image;

  // Create the quad mesh (to render the camera)
  createMesh();

  // Create capture thread
  m_captureThread = new OCVCaptureThread( *this );
  m_captureThread->start();

	// The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources.
 * After this method is called the class is not valid anymore.
 */
void OCVCamera::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  // Release camera
  cvReleaseCapture( &m_capture );

  // Release thread
  Release( m_captureThread );

  // Release image data
  cvReleaseImage( &m_cvResizedImage );
  Release( m_ogreImage );

  // Destroy quad mesh
  Ogre::SceneManager& sceneManager = GraphicsManager::getSingleton().getSceneManager();
  sceneManager.destroyManualObject( MANUAL_OBJECT_NAME );

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * @brief Captures new image from camera
 * @note this is a blocking call -> would be good to have it in a separate thread
 */
void OCVCamera::update()
{

  // TODO: check if new frame from thread
  if ( !haveNewFrame() ) return;

  m_mutex.lock();
  setNewFrame( false );

  // Resize the capture image
  // cvResize( m_cvCaptureImage, m_cvResizedImage );
  // Copy to Ogre image
  //m_ogreImage->loadDynamicImage( (unsigned char*)m_cvCaptureImage->imageData, m_cvCaptureImage->width, m_cvCaptureImage->height, 1, Ogre::PF_R8G8B8 );

  // Get the pixel buffer
  Ogre::HardwarePixelBufferSharedPtr pixelBuffer = m_ogreTexture->getBuffer();

  // TODO - replace with lock/unlock
  //pixelBuffer->blitFromMemory( m_ogreImage->getPixelBox() );

  // Lock the pixel buffer and get a pixel box (HBL_DISCARD for best peformance as we don't need to read the pixels
  pixelBuffer->lock( Ogre::HardwareBuffer::HBL_DISCARD );
  const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

  Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);
  //Ogre::uint8* pOrig = static_cast<Ogre::uint8*>(camPixelBox.data);

  static unsigned short color = 0;
  color++;
  if ( color >= 255 )
    color = 0;

  // Fill in some pixel data. This will give a semi-transparent blue,
  // but this is of course dependent on the chosen pixel format.
  size_t x = 0;
  size_t y = 0;
  size_t idx = 0;
  size_t camIdx = 0;
  for( int i = 0; i < m_cvResizedImage->width * m_cvResizedImage->height * m_cvResizedImage->nChannels; i += m_cvResizedImage->nChannels )
  {
    idx     = (x * 4) + y * pixelBox.rowPitch * 4;

    pDest[idx]    = m_cvResizedImage->imageData[i]  ; // B
    pDest[idx+1]  = m_cvResizedImage->imageData[i+1]; // G
    pDest[idx+2]  = m_cvResizedImage->imageData[i+2]; // R
    pDest[idx+3]  = 127; // A

    x++;
    if ( x == m_cvResizedImage->width )
    {
      x = 0;
      y++;
    }
  }

  // Unlock the pixel buffer
  pixelBuffer->unlock();

  m_mutex.unlock();
}

/**
 * @internal
 * @brief Creates the mesh object (quad) where the camera could be displayed
 */
void OCVCamera::createMesh()
{
  // Create the texture
  m_ogreTexture = Ogre::TextureManager::getSingleton().createManual( TEXTURE_NAME,
                                                                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                    Ogre::TEX_TYPE_2D,    // type
                                                                    320, 240,
                                                                    0,                    // number of mipmaps
                                                                    Ogre::PF_BYTE_BGRA,   // pixel format
                                                                    Ogre::TU_DEFAULT);    // usage; should be TU_DYNAMIC_WRITE_ONLY_DISCARDABLE for
                                                                                          // textures updated very often (e.g. each frame)

  // Create a material using the texture
  Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create( MATERIAL_NAME, // name
                                                                             Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

  material->getTechnique(0)->getPass(0)->createTextureUnitState( TEXTURE_NAME );
  material->getTechnique(0)->getPass(0)->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);

  Ogre::SceneManager& sceneManager = GraphicsManager::getSingleton().getSceneManager();
  Ogre::ManualObject* manual = sceneManager.createManualObject( MANUAL_OBJECT_NAME );

  // This would be to render in 2d (screen relative coords)
  //manual->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
  //manual->setUseIdentityProjection(true);
  //manual->setUseIdentityView(true);
  //manual->setQueryFlags(0);

  // Generate the geometry
  manual->begin( MATERIAL_NAME, Ogre::RenderOperation::OT_TRIANGLE_LIST );

    // Quad positions
    // TODO hacer normalizado y escalar
    manual->position( 0.0, 0.0, 0.0); manual->textureCoord( 0, 0 );
    manual->position( 320, 0.0, 0.0); manual->textureCoord( 1, 0 );
    manual->position( 320, 240, 0.0); manual->textureCoord( 1, 1 );
    manual->position( 0.0, 240, 0.0); manual->textureCoord( 0, 1 );

    // Quad indexes
    manual->triangle( 0, 1, 2 );
    manual->triangle( 0, 2, 3 );

  manual->end();

  // Add to the scene
  Ogre::SceneNode* cameraImageNode = sceneManager.getRootSceneNode()->createChildSceneNode();
  if( cameraImageNode )
	  cameraImageNode->attachObject( manual );
  else
	  LOG_ERROR( "OCVCamera::createMesh internal error: Could not create mesh");
}

} // namespace Cing
