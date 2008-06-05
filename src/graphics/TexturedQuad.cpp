/*
  This source file is part of the Vision project
  For the latest info, see http://www.XXX.org

  Copyright (c) 2008 XXX

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

#include "TexturedQuad.h"

// Graphics
#include "GraphicsManager.h"

// Ogre
#include "OgrePixelFormat.h"
#include "OgreImage.h"
#include "OgreSceneManager.h"
#include "OgreManualObject.h"
#include "OgreRenderOperation.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreTextureManager.h"
#include "OgreTexture.h"
#include "OgreMaterial.h"
#include "OgreMaterialManager.h"

// Framework
#include "framework/UserAppGlobals.h"

// Common
#include "common/Exception.h"
#include "common/MathUtils.h"

namespace Graphics
{

// Static member initialization
const std::string  TexturedQuad::MANUAL_OBJECT_NAME  = "QuadMesh";
const std::string  TexturedQuad::MATERIAL_NAME       = "QuadMaterial";
const std::string  TexturedQuad::TEXTURE_NAME        = "QuadTexture";
long               TexturedQuad::m_quadCounter       = 0;


/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
TexturedQuad::TexturedQuad():
  m_quadSceneNode ( NULL  ),
	m_visible				( true	),
  m_bIsValid      ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
TexturedQuad::~TexturedQuad()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @param[in] textureWidth  Width of the texture to show on the quad
 * @param[in] textureHeight Height of the texture to show on the quad
 * @param[in] format  Format of the image. RGB for color images (red, green and blue), ARGB for color plus alpha channel for transparency
 * GRAYSCALE for greyscale images, this is, black & white
 * @param[in] render2D      If true, the textured quad is rendered in 2d, so it is overlayed to the 3d scene
 * @return true if the initialization was ok | false otherwise
 */
bool TexturedQuad::init( size_t textureWidth, size_t textureHeight, ImageFormat format, bool render2D /*= false*/ )
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Store the texture data
  m_width     = textureWidth;
  m_height    = textureHeight;
	m_format		= format;
  m_render2D  = render2D;

  // Generate unique names for texture, material and ogre manual object
  generateUniqueNames();

  // Create the texture for the quad
  m_ogreTexture = Ogre::TextureManager::getSingleton().createManual( m_ogreTextureName,
                                                                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                    Ogre::TEX_TYPE_2D,																		// type
																																		(Ogre::uint)textureWidth, (Ogre::uint)textureHeight,  // resolution
                                                                    0,                                          					// number of mipmaps
																																		(Ogre::PixelFormat)format,														// pixel format
                                                                    Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE );  					// usage; should be TU_DYNAMIC_WRITE_ONLY_DISCARDABLE for
                                                                                                                					// textures updated very often (e.g. each frame)

  // Create a material for the quad
  Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().create( m_ogreMaterialName,
                                                                             Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
  // Assign texture to material
  material->getTechnique(0)->getPass(0)->createTextureUnitState( m_ogreTextureName );
  material->getTechnique(0)->getPass(0)->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );

  // No lighting
  material->getTechnique(0)->getPass(0)->setLightingEnabled( false );

  // Create the manual object (is used to define geometry on the fly)
  Ogre::SceneManager& sceneManager = Graphics::GraphicsManager::getSingleton().getSceneManager();
  Ogre::ManualObject* quad = sceneManager.createManualObject( m_ogreManualObjectName );

  // Check if we want 2d rendering
  if ( m_render2D )
  {
    quad->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY);
    quad->setUseIdentityProjection(true);
    quad->setUseIdentityView(true);
    quad->setQueryFlags(0);
  }

  // Generate the geometry of the quad
  quad->begin( m_ogreMaterialName, Ogre::RenderOperation::OT_TRIANGLE_LIST );

    // Quad positions and texture coordinates
    quad->position( 0.0, 0.0, 0.0);  quad->textureCoord( 0, 0 );
    quad->position( 1.0, 0.0, 0.0);  quad->textureCoord( 1, 0 );
    quad->position( 1.0, 1.0, 0.0);  quad->textureCoord( 1, 1 );
    quad->position( 0.0, 1.0, 0.0);  quad->textureCoord( 0, 1 );

    // Quad indexes (two triangles)
    quad->triangle( 0, 1, 2 );
    quad->triangle( 0, 2, 3 );

  // Finish defining geometry
  quad->end();

  // Add to the scene
  m_quadSceneNode = sceneManager.getRootSceneNode()->createChildSceneNode();
  m_quadSceneNode->attachObject( quad );

  // By default scale quad to texture resolution
  m_quadSceneNode->setScale( textureWidth, textureHeight, 1 );

  // The class is now initialized
	m_bIsValid = true;

	return true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void TexturedQuad::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

  // Destroy quad quad object
  Ogre::SceneManager& sceneManager = Graphics::GraphicsManager::getSingleton().getSceneManager();
  sceneManager.destroyManualObject( m_ogreManualObjectName );

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @internal
 * Sets the position in the scene of the textured quad
 * @param[in] x x coordinate of the new position
 * @param[in] y y coordinate of the new position
 * @param[in] z z coordinate of the new position
 */
void TexturedQuad::setPosition( float x, float y, float z /*= 0.0f*/ )
{
  // Render 2D
  if ( m_render2D )
  {
    float screenX = Common::map( x, 0, Globals::width, 0, 1 );
    float screenY = Common::map( y, 0, Globals::height, 0, 1 );
    m_quadSceneNode->setPosition( screenX, screenY, 0 );
  }
  // Render 3D
  else 
    m_quadSceneNode->setPosition( x, y, z );
}

/**
 * @internal
 * Sets the texture quad as visible/invisible
 * @param[in] visible if true the quad will be visible
 */
void TexturedQuad::setVisible( bool visible )
{
	m_visible = visible;
  m_quadSceneNode->setVisible( visible );
}

/**
 * @brief Updates the texture pixel information from an Image
 *
 * @param	img Image containing the pixel data to upload to the texture
 */
void TexturedQuad::updateTexture( const Ogre::Image& img )
{
	// Check if the object is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to upload data texture to a not initializad TextureQuad object" );

	// Update texture
	m_ogreTexture->getBuffer( 0, 0 )->blitFromMemory( img.getPixelBox( 0, 0 ) );
}

/**
 * @internal
 * @brief Updates the texture pixel data
 * @param[in] textureData New pixel data to upload to the texture
 * @param[in] width       Width of the new pixel data
 * @param[in] height      Height of the new pixel data
 * @param[in] format  Format of the image. RGB for color images (red, green and blue), ARGB for color plus alpha channel for transparency
 * GRAYSCALE for grayscale images, this is, black & white
 */
void TexturedQuad::updateTexture( unsigned char* textureData, size_t width, size_t height, ImageFormat format )
{
	// Check if the object is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to upload data texture to a not initializad TextureQuad object" );

  // Check resolution
  if ( ( width > m_width ) || ( height > m_height ) )
    THROW_EXCEPTION( "Trying to update texture with too big image data" );

	// Check format
	if ( m_format != format )
		THROW_EXCEPTION( "Trying to update texture with different format" );

	// Update texture
	m_ogreTexture->getBuffer( 0, 0 )->blitFromMemory( Ogre::PixelBox( width, height, 1, (Ogre::PixelFormat)format, textureData ) );



	//Ogre::DataStreamPtr dataPtr( new Ogre::MemoryDataStream( textureData, width * height * channels ) );
	//m_ogreTexture->loadRawData( dataPtr, (Ogre::ushort) width, (Ogre::ushort)height, format );

  ////// Check n channels
  ////if ( m_nChannels != channels )
  ////  THROW_EXCEPTION( "Trying to update texture with an image data that contains different number of channels" );

  //// Get the texture pixel buffer
  //Ogre::HardwarePixelBufferSharedPtr pixelBuffer = m_ogreTexture->getBuffer();
  //  
  //// Lock the pixel buffer and get a pixel box (HBL_DISCARD for best performance as we don't need to read the pixels)
  //pixelBuffer->lock( Ogre::HardwareBuffer::HBL_DISCARD ); 
  //
  //// Get pixel box (to access the actual pixels)
  //const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

  //// Cast to texture data pointer to access the pixels
  //unsigned char* pDest = static_cast< unsigned char* >( pixelBox.data );

  //// Fill in pixel data. 
  //size_t x = 0;
  //size_t y = 0;
  //size_t idx = 0;
  //size_t camIdx = 0;
  //size_t textureChannels = channels == 1? 1: 4;  //<- Always 4 channels, because when we create a texture even without alpha channel, it has 4 channels
  //                             // but in render time the alpha is discarded... (TODO: look why this happens)
  //for( size_t i = 0, x = 0; i < width * height * channels; i += channels )
  //{
  //  idx = (x * textureChannels) + y * pixelBox.rowPitch * textureChannels;

  //  // Write the data
  //  pDest[idx]    = textureData[i]  ; // B
  //  pDest[idx+1]  = textureData[i+1]; // G
  //  pDest[idx+2]  = textureData[i+2]; // R
  //  
  //  // Alpha channel?
  //  if ( channels == 4 )
  //    pDest[idx+3]  = textureData[i+3]; // A
  //  else 
  //    pDest[idx+3]  = 255;

  //  // Increment dest texture indexes
  //  x++;
  //  if ( x == width )
  //  {
  //    x = 0;
  //    y++;
  //  }
  //}

  //// Unlock the pixel buffer
  //pixelBuffer->unlock();
}



/**
 * @brief Copies the data of the received texture quad
 *
 * @param other TextureQuad to copy
 */
void TexturedQuad::operator=( const TexturedQuad& other )
{
	// Check the other texture quad is valid
	if ( !other.isValid() )
		THROW_EXCEPTION( "Trying to copy an invalid TextureQuad" );

	// Init the texture
	init( other.m_width, other.m_height, other.m_format, other.m_render2D );

	// Copy the texture
	other.m_ogreTexture->copyToTexture( m_ogreTexture );

	// Copy attributes
	const Vector3& pos = other.m_quadSceneNode->getPosition(); 
	setPosition( pos.x, pos.y, pos.z );
	setVisible( other.m_visible );

}

/**
 * @internal
 * @brief Generates unique names for this quad's texture, material and ogre manual object
 */
void TexturedQuad::generateUniqueNames()
{
  ++m_quadCounter;
  std::ostringstream oss;

  // Manual object
  oss << MANUAL_OBJECT_NAME << m_quadCounter;
  m_ogreManualObjectName = oss.str();
  oss.str( "" ); 

  // Texture
  oss << TEXTURE_NAME << m_quadCounter;
  m_ogreTextureName = oss.str();
  oss.str( "" ); 

  // Material
  oss << MATERIAL_NAME << m_quadCounter;
  m_ogreMaterialName = oss.str();
}

} // namespace Graphics