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

// Precompiled headers
#include "Cing-Precompiled.h"


#include "OCVCamera.h"

// Ogre
#include "OgrePixelFormat.h"
#include "OgreImage.h"
#include "OgreSceneManager.h"
#include "OgreManualObject.h"
#include "OgreRenderOperation.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreTexture.h"
#include "OgreMaterial.h"
#include "OgreMaterialManager.h"

// Graphics
#include "graphics/GraphicsManager.h"

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
		m_ocvCamera.m_capture.open( m_ocvCamera.getDeviceId() );
		if ( m_ocvCamera.m_capture.isOpened() )
		{
			LOG_ERROR( "[OCVCamera] No camera found, or no drivers installed" );
			return;
		}
		
		// Set camera res
		m_ocvCamera.m_capture.set( CV_CAP_PROP_FRAME_WIDTH, m_ocvCamera.getWidth() );
		m_ocvCamera.m_capture.set( CV_CAP_PROP_FRAME_HEIGHT, m_ocvCamera.getHeight() );
		m_ocvCamera.m_capture.set( CV_CAP_PROP_FPS, m_ocvCamera.getFPS() );		
		
		while( !get_signaled() )
		{
			// Get New frmae
			//if ( m_ocvCamera.m_capture.grab())
			// Get camera frame from OpenCV -> Note: this is a blocking calle
			m_ocvCamera.m_capture.grab();
			m_ocvCamera.m_capture.retrieve( *m_cvCaptureImage );
			if ( m_cvCaptureImage )
			{
				//m_ocvCamera.m_capture.retrieve( m_ocvCamera.m_newFrame, 0 );
				//m_ocvCamera.m_mutex.lock();
						
				// Get number of channels in the image (looks like in the mac side it is aligned to 4 bytes
				GraphicsType format;
				if ( m_cvCaptureImage->channels() == 1 )
					format = GRAYSCALE;
				else if ( m_cvCaptureImage->channels() == 3 )
					format = RGB;
				else if ( m_cvCaptureImage->channels() == 4 )
					format = BGRA;
				
				// we get the pixels by passing in out buffer which gets 
				m_ocvCamera.setNewFrameData( m_cvCaptureImage->data,  m_ocvCamera.getWidth( ),  m_ocvCamera.getHeight( ), format, m_cvCaptureImage->step, true );
				m_ocvCamera.setNewFrame( true );				

				m_cvCaptureImage = NULL;
				
				// Copy the image
				//cvCopy( m_cvCaptureImage, m_ocvCamera.m_cvResizedImage );
			
				// New frame available
				//m_ocvCamera.setNewFrame( true );
			
				//m_ocvCamera.m_mutex.unlock();
			}
			
		}
	}
	
	void OCVCamera::OCVCaptureThread::cleanup()
	{
		m_ocvCamera.m_capture.release();
	}
	
	
	/**
	 * @internal
	 * @brief Constructor. Initializes class attributes.
	 */
	OCVCamera::OCVCamera():
	//m_capture         ( NULL  ),
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
	bool OCVCamera::init( int deviceId /* = 0 */, int width /* = 320 */, int height /* = 240 */, int fps/* = 25 */, GraphicsType format /* = RGB */, bool multithreaded /*= true*/ )
	{
		// Check if the class is already initialized
		if ( isValid() )
			return true;
		
		// Init capture (from any camera)
		//m_capture.open(deviceId);
		//if(!m_capture.isOpened()) 
		//	return false;
		
/*		m_capture = cvCaptureFromCAM( deviceId );
		if ( !m_capture )
			THROW_EXCEPTION( "No camera found, or no drivers installed" );
		
		// Set camera res
		cvSetCaptureProperty( m_capture, CV_CAP_PROP_FRAME_WIDTH, width );
		cvSetCaptureProperty( m_capture, CV_CAP_PROP_FRAME_HEIGHT, height );
		cvSetCaptureProperty( m_capture, CV_CAP_PROP_FPS, fps );
*/
		//m_capture.set( CV_CAP_PROP_FRAME_WIDTH, width );
		//m_capture.set( CV_CAP_PROP_FRAME_HEIGHT, height );
		
		// Set FPS
		//m_capture.set( CV_CAP_PROP_FPS, CV_CAP_PROP_FPS );		
	
		// Init base class (with actual capture resolution)
		BaseCameraInput::init( deviceId, width, height, fps, format );
	
		//m_newFrame.create( height, width, CV_MAKETYPE(CV_8U,3) );
		
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
		//m_capture.release();
		//cvReleaseCapture(&m_capture);
		
		// Release thread
		Release( m_captureThread );
		
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
		// No new frame -> do nothing
		//if ( !haveNewFrame() ) 
		//	return;
		
		//m_capture.retrieve( m_newFrame, 1 );
		//m_capture >> m_newFrame;
		
		//setNewFrameData(  (unsigned char*)m_newFrame.data,  getWidth( ),  getHeight( ), RGB );
		//setNewFrame( true );		
		
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
																				  Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).staticCast<Ogre::Material>();
		
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
