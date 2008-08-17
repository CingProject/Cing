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

#include "TexturedQuad.h"

// Graphics
#include "GraphicsManager.h"

// Ogre
#include "externLibs/Ogre3d/include/OgrePixelFormat.h"
#include "externLibs/Ogre3d/include/OgreImage.h"
#include "externLibs/Ogre3d/include/OgreSceneManager.h"
#include "externLibs/Ogre3d/include/OgreManualObject.h"
#include "externLibs/Ogre3d/include/OgreRenderOperation.h"
#include "externLibs/Ogre3d/include/OgreHardwarePixelBuffer.h"
#include "externLibs/Ogre3d/include/OgreTextureManager.h"
#include "externLibs/Ogre3d/include/OgreTexture.h"
#include "externLibs/Ogre3d/include/OgreMaterial.h"
#include "externLibs/Ogre3d/include/OgreMaterialManager.h"

// Framework
#include "framework/UserAppGlobals.h"

// Common
#include "common/Exception.h"
#include "common/MathUtils.h"
#include "common/LogManager.h"

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
	m_quad					( NULL  ),
	m_2dXPos				( -INT_MAX ),
	m_2dYPos				( -INT_MAX ),
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
 * @return true if the initialization was ok | false otherwise
 */
bool TexturedQuad::init( int textureWidth, int textureHeight, ImageFormat format )
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

  // Store the texture data
  m_textWidth     = textureWidth;
  m_textHeight    = textureHeight;
	m_format		= format;
  m_render2D  = false;

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
  // Assign texture to material and set materia properties
  material->getTechnique(0)->getPass(0)->createTextureUnitState( m_ogreTextureName );
  material->getTechnique(0)->getPass(0)->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
	material->getTechnique(0)->getPass(0)->setLightingEnabled( false );

  // Create the manual object (is used to define geometry on the fly)
  Ogre::SceneManager& sceneManager = Graphics::GraphicsManager::getSingleton().getSceneManager();
	m_quad = sceneManager.createManualObject( m_ogreManualObjectName );

  // Generate the geometry of the quad
	m_quad->begin( m_ogreMaterialName );

    // m_quad positions and texture coordinates
    m_quad->position( 0.0, 0.0, 0.0);  m_quad->textureCoord( 0, 1 );
    m_quad->position( 1.0, 0.0, 0.0);  m_quad->textureCoord( 1, 1 );
    m_quad->position( 1.0, 1.0, 0.0);  m_quad->textureCoord( 1, 0 );
    m_quad->position( 0.0, 1.0, 0.0);  m_quad->textureCoord( 0, 0 );

    // m_quad indexes (two triangles)
		m_quad->triangle( 0, 1, 2 );
		m_quad->triangle( 0, 2, 3 );

  // Finish defining geometry
  m_quad->end();

  // Add to the scene
  m_quadSceneNode = sceneManager.getRootSceneNode()->createChildSceneNode();
  m_quadSceneNode->attachObject( m_quad );

	// Register in graphics manager so that it is marked as invisible every frame
	GraphicsManager::getSingleton().addDrawableImage( this );

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
  Ogre::SceneManager* sceneManager = Graphics::GraphicsManager::getSingleton().getSceneManagerPtr();
	if ( sceneManager )
	{
		sceneManager->destroyManualObject( m_ogreManualObjectName );

		// Unregister in graphics manager so that it is marked as invisible every frame
		GraphicsManager::getSingleton().removeDrawableImage( this );
	}

	// The class is not valid anymore
	m_bIsValid = false;
}

/**
 * @brief Sets the position in the scene of the textured quad
 * @param[in] x x coordinate of the new position
 * @param[in] y y coordinate of the new position
 * @param[in] z z coordinate of the new position
 */
void TexturedQuad::setPosition( float x, float y, float z )
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to set position in an invalid textured quad" );
		return;
	}

  m_quadSceneNode->setPosition( x, y, z );
}

/**
 * @brief Sets the position in the scene of the textured quad (in 2 dimensions -> in screen coordinates)
 * @param[in] x x coordinate of the new position
 * @param[in] y y coordinate of the new position
 */
void TexturedQuad::setPosition2d( float x, float y )
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to set position in an invalid textured quad" );
		return;
	}
	
	// Simulate upper left corner to be 0,0 (although in ogre the 0,0 is the lower left corner
	// That is why the y screen coordinate is inverted
	m_2dXPos = (x / (float)Globals::width) * 2.0f - 1;
	m_2dYPos = -((y / (float)Globals::height) * 2.0f - 1);
	m_quadSceneNode->setPosition( m_2dXPos, m_2dYPos - m_2dHeight, 0 );
}

/**
 * @brief Sets the scale of the object
 *
 * @param[in] xScale scale in the x axis
 * @param[in] xScale scale in the y axis
 * @param[in] xScale scale in the z axis
 */
void TexturedQuad::setScale( float xScale, float yScale, float zScale /*= 1.0f*/ )
{
	m_quadSceneNode->setScale( xScale, yScale, zScale );
}


/**
 * @brief Sets the scale of the object in 2d, this is in screen coordinates
 *
 * @param[in] xScale scale in the x axis
 * @param[in] xScale scale in the y axis
 */
void TexturedQuad::setScale2d( float xScale, float yScale )
{
	m_2dWidth = (xScale / (float)Globals::width) * 2.0f;
	m_2dHeight = (yScale / (float)Globals::height) * 2.0f;
	m_quadSceneNode->setScale( m_2dWidth, m_2dHeight, 1 );
}

/**
 * @internal
 * Sets the texture quad as visible/invisible
 * @param[in] visible if true the quad will be visible
 */
void TexturedQuad::setVisible( bool visible )
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to set position in an invalid textured quad" );
		return;
	}

	m_visible = visible;
  m_quadSceneNode->setVisible( visible );
}

/**
 * @brief Draws the texture quad in three dimensions, with the texture size
 *
 * @param x X coordinate where it will be drawn
 * @param y Y coordinate where it will be drawn
 * @param z Z coordinate where it will be drawn
 */
void TexturedQuad::draw( float x, float y, float z )
{
	draw( x, y, z, m_textWidth, m_textHeight );
}

/**
 * @brief Draws the texture quad in three dimensions, with a specific size
 *
 * @param x 			X coordinate where it will be drawn
 * @param y 			Y coordinate where it will be drawn
 * @param z 			Z coordinate where it will be drawn
 * @param width		Width of the quad that will be rendered
 * @param height	Height of the quad that will be rendered
 */
void TexturedQuad::draw( float x, float y, float z, float width, float height )
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to draw a textured quad not initialized" );
		return;
	}

	// If the object is set to render in 2d -> set it to render in 3d
	if ( m_render2D )
		set3dRendering();

	// Set properties of the quad, and set visible -> it will be rendered in the next render
	m_quadSceneNode->setPosition( x, y, z );
	m_quadSceneNode->setScale( width, height, 1 );
	m_quadSceneNode->setVisible( true );
}

/**
 * @brief Draws the texture quad in two dimensions, with the texture size
 * 
 * @param x X coordinate where it will be drawn <b>in screen coordinates</b>
 * @param y Y coordinate where it will be drawn <b>in screen coordinates</b>
 */
void TexturedQuad::draw2d( float x, float y )
{
	draw2d( x, y, m_textWidth, m_textHeight );
}

/**
 * @brief Draws the texture quad in two dimensions, with a specific size
 * 
 * @param x X coordinate where it will be drawn <b>in screen coordinates</b>
 * @param y Y coordinate where it will be drawn <b>in screen coordinates</b>
 * @param width		Width of the quad that will be rendered <b>in screen coordinates</b>
 * @param height	Height of the quad that will be rendered <b>in screen coordinates</b>
 */
void TexturedQuad::draw2d( float x, float y, float width, float height )
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to draw a textured quad not initialized" );
		return;
	}

	// If the object was set to render in 3d -> set it to render in 2d
	if ( !m_render2D )
		set2dRendering();

	// Set properties of the quad, and set visible -> it will be rendered in the next render
	setScale2d( width, height );
	setPosition2d( x, y );
	m_quadSceneNode->setVisible( true );
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
void TexturedQuad::updateTexture( unsigned char* textureData, int width, int height, ImageFormat format )
{
	// Check if the object is valid
	if ( !isValid() )
		THROW_EXCEPTION( "Trying to upload data texture to a not initializad TextureQuad object" );

  // Check resolution
  if ( ( width > m_textWidth ) || ( height > m_textHeight ) )
    THROW_EXCEPTION( "Trying to update texture with too big image data" );

	// Check format
	if ( m_format != format )
		THROW_EXCEPTION( "Trying to update texture with different format" );

	// Update texture
	m_ogreTexture->getBuffer( 0, 0 )->blitFromMemory( Ogre::PixelBox( width, height, 1, (Ogre::PixelFormat)format, textureData ) );
}

/**
 * @brief Flips the texture coordinates vertically
 */
void TexturedQuad::flipVertical()
{
	// Update geometry to flip texture coordinates
	// We recreate the object instead of updating it because this is an extrange use, and for the normal
	// use is better in terms of performance to have static objects (in terms of vertex data)
	m_quad->clear();
	m_quad->begin( m_ogreMaterialName );

	// m_quad positions and texture coordinates
	m_quad->position( 0.0, 0.0, 0.0);  m_quad->textureCoord( 0, 0 );
	m_quad->position( 1.0, 0.0, 0.0);  m_quad->textureCoord( 1, 0 );
	m_quad->position( 1.0, 1.0, 0.0);  m_quad->textureCoord( 1, 1 );
	m_quad->position( 0.0, 1.0, 0.0);  m_quad->textureCoord( 0, 1 );

	// m_quad indexes (two triangles)
	m_quad->triangle( 0, 1, 2 );
	m_quad->triangle( 0, 2, 3 );

	// Finish updating geometry
	m_quad->end();
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
	init( other.m_textWidth, other.m_textHeight, other.m_format );

	// Copy the texture
	other.m_ogreTexture->copyToTexture( m_ogreTexture );

	// Copy attributes
	const Vector& pos = other.m_quadSceneNode->getPosition(); 
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

/**
 * @internal 
 * @brief Sets the necessary properties for the object to be rendered in 2d (in screen coordinates and size in pixels)
 */
void TexturedQuad::set2dRendering()
{
	// Properties to be rendered in 2d
	m_quad->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY -1);
	m_quad->setUseIdentityProjection(true);
	m_quad->setUseIdentityView(true);
	Ogre::AxisAlignedBox aabb;
	aabb.setInfinite();
	m_quad->setBoundingBox(aabb);

	// mark the object as 2d rendering
	m_render2D = true;
}

/**
 * @internal 
 * @brief Sets the necessary properties for the object to be rendered in 3d (in world coordinates and world units)
 */
void TexturedQuad::set3dRendering()
{
	// Set properties for 3d rendering
	m_quad->setRenderQueueGroup( Ogre::RENDER_QUEUE_MAIN );
	m_quad->setUseIdentityProjection( false );
	m_quad->setUseIdentityView( false );
	m_quad->setQueryFlags( m_3dQueryFlags );

	// mark the object as 3d rendering
	m_render2D = false;
}


} // namespace Graphics