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

#include "TexturedQuad.h"

#include <limits.h>
#include <ostream>

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
#include "framework/Application.h"

// Common
#include "common/Exception.h"
#include "common/MathUtils.h"
#include "common/LogManager.h"
#include "common/CommonConstants.h"

namespace Cing
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
		m_2dXPos				( -FLT_MAX ),
		m_2dYPos				( -FLT_MAX ),
		m_xScale				( 1.0f ),
		m_yScale				( 1.0f ),
		m_zScale				( 1.0f ),
		m_bIsValid				( false ),
		m_renderQueueForced		( false ),
		m_sm					( NULL )
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
	* @param[in] usage of the texture (allows to customize is usage depending on whether you plan to update the texture only, or also read its content, or even use it as a render target)
	* @return true if the initialization was ok | false otherwise
	*/
	bool TexturedQuad::init( int textureWidth, int textureHeight, GraphicsType format, GraphicsType usage /*= DYNAMIC*/, Ogre::SceneManager* sm )
	{
		// Check if the class is already initialized
		if ( isValid() )
			return true;

		// Check application correctly initialized (could not be if the user didn't calle size function)
		Application::getSingleton().checkSubsystemsInit();

		// Get power of 2 texture size
		//m_textWidthP2 = firstPO2From( textureWidth );
		//m_textHeightP2 = firstPO2From( textureHeight );
		m_textWidthP2 = (float)textureWidth;
		m_textHeightP2 = (float)textureHeight;

		// Store the texture data
		m_textWidth     = (float)textureWidth;
		m_textHeight    = (float)textureHeight;	
		m_format		= format;
		m_render2D		= false;
		m_alpha			= 255;

		// Generate unique names for texture, material and ogre manual object
		generateUniqueNames();

		// Texture Usage
		Ogre::TextureUsage textureUsage = (Ogre::TextureUsage)usage;

		// Create the texture for the quad

		// If it is a render target FSAA (anti-aliasing can be set)
		if ( usage == Ogre::TU_RENDERTARGET )
		{
			m_ogreTexture = Ogre::TextureManager::getSingleton().createManual(m_ogreTextureName,
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				Ogre::TEX_TYPE_2D,				// type
				(Ogre::uint)m_textWidthP2,		// width
				(Ogre::uint)m_textHeightP2,		// height
				0,								// number of mipmaps
				(Ogre::PixelFormat)format,		// pixel format
				textureUsage,					// usage as render target
				0,
				false,
				0);								// FSAA value
		}
		else
		{
			m_ogreTexture = Ogre::TextureManager::getSingleton().createManual(m_ogreTextureName,
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
				Ogre::TEX_TYPE_2D,				// type
				(Ogre::uint)m_textWidthP2,		// width
				(Ogre::uint)m_textHeightP2,		// height
				0,								// number of mipmaps
				(Ogre::PixelFormat)format,		// pixel format
				textureUsage );						// usage; should be TU_DYNAMIC_WRITE_ONLY_DISCARDABLE for textures updated very often (e.g. each frame)
		}

		// Create a material for the quad
		m_ogreMaterial = Ogre::MaterialManager::getSingleton().create( m_ogreMaterialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		
		// Assign texture to material and set materia properties
		Ogre::TextureUnitState* texUnit = m_ogreMaterial->getTechnique(0)->getPass(0)->createTextureUnitState( m_ogreTextureName );
		texUnit->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP); // note: clamp fixes glitch in edges of 2d images

		m_ogreMaterial->getTechnique(0)->getPass(0)->setCullingMode( Ogre::CULL_NONE );
		//m_ogreMaterial->getTechnique(0)->getPass(0)->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
//		m_ogreMaterial->getTechnique(0)->getPass(0)->setAlphaRejectSettings( Ogre::CMPF_GREATER_EQUAL, 1 );


		/*

		TODO: We need an easy way to change images transparency!!
		//Modified by Jorge 25/feb/2009

		//m_ogreMaterial->getTechnique(0)->getPass(0)->setDepthWriteEnabled( false );
		//m_ogreMaterial->getTechnique(0)->getPass(0)->setSceneBlending( Ogre::SBT_TRANSPARENT_COLOUR );
		//m_ogreMaterial->getTechnique(0)->getPass(0)->setDiffuse(Ogre::ColourValue(1,1,0.5,0.5));


		static_cast<MaterialPtr>( MaterialManager::getSingleton( ).getByName( "Overlay/TwoTextures" ) )
		->getTechnique(0)->getPass(0)->getTextureUnitState("MixTexture")->
		setColourOperationEx(LBX_BLEND_MANUAL, LBS_TEXTURE, LBS_CURRENT, 1,1, add);
		*/

		m_sbType = Ogre::SBT_REPLACE;

		// Create the manual object (is used to define geometry on the fly)
		if ( sm == NULL )
			m_sm = GraphicsManager::getSingleton().getSceneManagerPtr();
		else
			m_sm = sm;

		m_quad = m_sm->createManualObject( m_ogreManualObjectName );

		// Generate the geometry of the quad
		m_quad->begin( m_ogreMaterialName );

		// m_quad positions and texture coordinates
		//m_quad->position( 0.0, 0.0, 0.0);  m_quad->textureCoord( 0, m_textHeight / m_textHeightP2 );
		//m_quad->position( 1.0, 0.0, 0.0);  m_quad->textureCoord( m_textWidth / m_textWidthP2, m_textHeight / m_textHeightP2 );
		//m_quad->position( 1.0, 1.0, 0.0);  m_quad->textureCoord( m_textWidth / m_textWidthP2, 0 );
		//m_quad->position( 0.0, 1.0, 0.0);  m_quad->textureCoord( 0, 0 );

		// Center mode (Julio -> TODO)
		m_quad->position( -0.5, -0.5, 0.0);	m_quad->textureCoord( 0, m_textHeight / m_textHeightP2 );
		m_quad->position( 0.5, -0.5, 0.0);	m_quad->textureCoord( m_textWidth / m_textWidthP2, m_textHeight / m_textHeightP2 );
		m_quad->position( 0.5, 0.5, 0.0);	m_quad->textureCoord( m_textWidth / m_textWidthP2, 0 );
		m_quad->position( -0.5, 0.5, 0.0);  m_quad->textureCoord( 0, 0 );


		// m_quad indexes (two triangles)
		m_quad->triangle( 0, 1, 2 );
		m_quad->triangle( 0, 2, 3 );

		// Finish defining geometry
		m_quad->end();

		// Create scene nodes (pivot and quadSceneNode) and add the manual object
		m_pivotSceneNode = m_sm->getRootSceneNode()->createChildSceneNode();
		m_quadSceneNode  = m_pivotSceneNode->createChildSceneNode();
		m_quadSceneNode->attachObject( m_quad );

		// Register in graphics manager so that it is marked as invisible every frame
		GraphicsManager::getSingleton().addDrawableImage( this );

		// By default: 3d render
		set3dRendering();

		// calculate the necessary scale 2d values for scale 1 (initial) - so basically initializes m_2dWidth and m_2dHeight in case the image is rendered
		// in screen space in the first frame without setting the scale
		m_2dWidth = (m_textWidth / (float)width) * 2.0f;
		m_2dHeight = (m_textHeight / (float)height) * 2.0f;

		// The class is now initialized
		m_bIsValid = true;

		// No shadow casting in textured quads by default
		enableCastShadows(false);

		return true;
	}
	/**
	* @brief Change ink drawing mode
	*/
	void  TexturedQuad::setAdditiveMode ( bool value )
	{

		if ( value )
		{
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(m_ogreMaterialName);
			material->getTechnique(0)->getPass(0)->setSceneBlending( Ogre::SBT_ADD);
		}
		else
		{
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(m_ogreMaterialName);
			material->getTechnique(0)->getPass(0)->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );
		}

	};

	/**
	* @brief Set blend operation
	*/
	void TexturedQuad::setSceneBlendType( Ogre::SceneBlendType type )
	{
		m_sbType = type;
	};


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

		// Destroy quad object
		//Ogre::SceneManager* sceneManager = GraphicsManager::getSingleton().getSceneManagerPtr();

		if ( m_sm != NULL )
		{
			// TODO: Hack to avoid crash on exit
			if( m_sm->getName().compare( "" ) == 0 )
			{
				return;
			}

			// Remove the nodes
			m_sm->getRootSceneNode()->removeChild( m_pivotSceneNode );

			// Destroy the manual object
			m_sm->destroyManualObject( m_ogreManualObjectName );		

			// Destroy material
			Ogre::MaterialManager::getSingleton().remove( m_ogreMaterialName );

			// Destroy texture
			Ogre::TextureManager::getSingleton().remove( m_ogreTextureName );

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
	* @brief Sets the position in the scene of the textured quad (in 2 dimensions -> in screen coordinates: range -1..1)
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
		m_2dXPos = (x / (float)width) * 2.0f - 1;
		m_2dYPos = -((y / (float)height) * 2.0f - 1);
		m_pivotSceneNode->setPosition( m_2dXPos , m_2dYPos, 0 );
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
		m_xScale = xScale;
		m_yScale = yScale;
		m_zScale = zScale;
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
		m_2dWidth = (xScale / (float)width) * 2.0f;
		m_2dHeight = (yScale / (float)height) * 2.0f;
		m_quadSceneNode->setScale( m_2dWidth, m_2dHeight, 1 );
	}

	/**
	* @brief Sets the orientation of the quad (in relation to its parents)
	*
	* @param[in] axis	rotation axis
	* @param[in] angle rotation angle (degrees)
	*/
	void TexturedQuad::setOrientation( const Vector& axis, float angle )
	{
		Quaternion q( Ogre::Radian( Ogre::Degree( angle ) ), axis );
		m_quadSceneNode->setOrientation( q );
	}

	/**
	* @brief Sets the orientation of the quad (in relation to its parents)
	*
	* @param[in] orientation quaternion containing the orientation information
	*/
	void TexturedQuad::setOrientation( const Quaternion& orientation )
	{
		m_quadSceneNode->setOrientation( orientation );
	}

	/**
	* @brief adds the rotation of the quad (in relation to its parents)
	*
	* @param[in] axis	rotation axis
	* @param[in] angle rotation angle (degrees)
	*/
	void TexturedQuad::rotate( const Vector& axis, float angle )
	{
		Quaternion q( Ogre::Radian( Ogre::Degree( angle ) ), axis );
		m_quadSceneNode->rotate( q );
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
		draw( x, y, z, m_textWidth * m_xScale, m_textHeight * m_yScale );
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
			LOG_ERROR_NTIMES( 1, "Trying to draw a textured quad not initialized" );
			return;
		}

		// If the object is set to render in 2d -> set it to render in 3d
		if ( m_render2D )
			set3dRendering();

		// Set transparency settings
		configureSceneBlending();

		// Apply current transformations to the pivot node
		applyTransformations(x, y, z, width, height);

		m_quadSceneNode->setVisible( true );
	}

	/**
	* @brief Draws the texture quad in three dimensions, specifying four points
	* order: top-left, top-right, bottom-right, bottom-left (anti-cloclwise)
	*
	* @param xPos x1
	* @param yPos y1
	* @param zPos z1
	* @param xPos x2
	* @param yPos y2
	* @param zPos z2
	* @param xPos x3
	* @param yPos y3
	* @param zPos z3
	* @param xPos x4
	* @param yPos y4
	* @param zPos z4
	*/
	void TexturedQuad::draw(	float x1, float y1, float z1,
								float x2, float y2, float z2,
								float x3, float y3, float z3,
								float x4, float y4, float z4)
	{
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 1, "Trying to draw a textured quad not initialized" );
			return;
		}

		// If the object is set to render in 2d -> set it to render in 3d
		if ( m_render2D )
			set3dRendering();

		// Set transparency settings
		configureSceneBlending();

		// Find center
		float xCenter = (x1 + x2 + x3 + x4)/4.0f;
		float yCenter = (y1 + y2 + y3 + y4)/4.0f;
		float zCenter = (z1 + z2 + z3 + z4)/4.0f;

		// Apply current transformations to the pivot node
		applyTransformations(xCenter, yCenter, zCenter, 1, 1);
		
		// Generate the geometry of the quad
		m_quad->beginUpdate( 0 );

		// Coordinate systems: in normal system invert y coordinates (y grows in top direction)
		//if ( GraphicsManager::getSingleton().isProcessingMode() == false )
		//{
		//	y1 = height - y1;
		//	y2 = height - y2;
		//	y3 = height - y3;
		//	y4 = height - y4;
		//}

		// m_quad positions and texture coordinates
		// TODO revisar esto...por qué hay q voltear las coordenadas uv?
		m_quad->position( x1-xCenter, y1-yCenter, z1-zCenter );  m_quad->textureCoord( 0, 0 );
		m_quad->position( x2-xCenter, y2-yCenter, z2-zCenter );  m_quad->textureCoord( 1, 0 );
		m_quad->position( x3-xCenter, y3-yCenter, z3-zCenter );  m_quad->textureCoord( 1, 1 );
		m_quad->position( x4-xCenter, y4-yCenter, z4-zCenter );  m_quad->textureCoord( 0, 1 );

		// m_quad indexes (two triangles)
		m_quad->triangle( 0, 2, 1 );
		m_quad->triangle( 0, 3, 2 );

		// Finish defining geometry
		m_quad->end();

		// Set properties of the quad, and set visible -> it will be rendered in the next render
		m_quadSceneNode->setVisible( true );
	}

	/**
	* @brief Draws the texture quad in two dimensions, with a specific size
	*
	* @param x X coordinate where it will be drawn <b>in screen coordinates</b>
	* @param y Y coordinate where it will be drawn <b>in screen coordinates</b>
	* @param width		Width of the quad that will be rendered <b>in screen coordinates</b>
	* @param height	Height of the quad that will be rendered <b>in screen coordinates</b>
	*/
	void TexturedQuad::draw2d( float x, float y, float imgWidth, float imgHeight )
	{
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 1, "Trying to draw a textured quad not initialized" );
			return;
		}

		// If the object was set to render in 3d -> set it to render in 2d
		if ( !m_render2D )
			set2dRendering();

		// Set transparency settings
		configureSceneBlending();

		// Apply transformations to the pivot node
 		applyTransformations2D(x, y, imgWidth, imgHeight);

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
		draw2d( x, y, m_textWidth * m_xScale, m_textHeight * m_yScale );
	}

	/**
	* @brief Draws the texture quad in two dimensions, specifying the four corner points
	* order: top-left, top-right, bottom-right, bottom-left (anti-cloclwise)
	*/
	void TexturedQuad::draw2d( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 )
	{
		if ( !isValid() )
		{
			LOG_ERROR_NTIMES( 1, "Trying to draw a textured quad not initialized" );
			return;
		}

		// If the object was set to render in 3d -> set it to render in 2d
		if ( !m_render2D )
			set2dRendering();

		// Set transparency settings
		configureSceneBlending();

		// Apply current transformations to the pivot node
		applyTransformations2D(0, 0, 1, 1);

		// Generate the geometry of the quad
		m_quad->beginUpdate( 0 );

		// Coordinate systems: in normal system invert y coordinates (y increasis in top direction)
		if ( GraphicsManager::getSingleton().isProcessingMode() )
		{
			y1 = height - y1;
			y2 = height - y2;
			y3 = height - y3;
			y4 = height - y4;
		}

		// m_quad positions and texture coordinates
		// TODO revisar esto...por qué hay q voltear las coordenadas uv?
		m_quad->position( (x1 / (float)width) * 2.0f - 1, -((y1 / (float)height) * 2.0f - 1), 0 );  m_quad->textureCoord( 0, 0 );
		m_quad->position( (x2 / (float)width) * 2.0f - 1, -((y2 / (float)height) * 2.0f - 1), 0 );  m_quad->textureCoord( 1, 0 );
		m_quad->position( (x3 / (float)width) * 2.0f - 1, -((y3 / (float)height) * 2.0f - 1), 0 );  m_quad->textureCoord( 1, 1 );
		m_quad->position( (x4 / (float)width) * 2.0f - 1, -((y4 / (float)height) * 2.0f - 1), 0 );  m_quad->textureCoord( 0, 1 );

		// m_quad indexes (two triangles)
		m_quad->triangle( 0, 2, 1 );
		m_quad->triangle( 0, 3, 2 );

		// Finish defining geometry
		m_quad->end();

		// Set properties of the quad, and set visible -> it will be rendered in the next render
		m_quadSceneNode->setVisible( true );
	}

	/**
	 * @brief Draws the texture quad in two dimensions as a background
	 * 
	 * @param x X coordinate where it will be drawn <b>in screen coordinates</b>
	 * @param y Y coordinate where it will be drawn <b>in screen coordinates</b>
	 */
	void TexturedQuad::drawBackground( float x, float y )
	{
		// Forward the call -> draw it to cover the whole window 
		drawBackground(x, y, (float)width, (float)height);
	}


	/**
	 * @brief Draws the texture quad in two dimensions as a background
	 * 
	 * @param x X coordinate where it will be drawn <b>in screen coordinates</b>
	 * @param y Y coordinate where it will be drawn <b>in screen coordinates</b>
	 * @parma imgWidth width of the rendered quad in screen coordinates
	 * @parma imgHeight height of the rendered quad in screen coordinates
	 */
	void TexturedQuad::drawBackground( float x, float y, float imgWidth, float imgHeight )
	{
		if ( !isValid() )
		{
			LOG_ERROR( "Trying to draw a textured quad not initialized" );
			return;
		}

		// Set rendering properties
		setbackgroundRendering();

		// Apply current transformations to the pivot node
		applyTransformations2D(x, y, imgWidth, imgHeight);

		m_quadSceneNode->setVisible( true );
	}


	/**
	 * Sets the alpha value (transparency of the image). Range: 0 (transparent) -> 255 (opaque)
	 * @param alpha new alpha value to use when rendering this texture
	 */
	void TexturedQuad::setTransparency( float alpha )
	{
		// Check quad is valid
		if ( !isValid() )
		{
			LOG_ERROR( "setTransparency. Error: set transparency value as Image/Texture has not been initialized. Please call load or init before setting the render queue" );
			return;
		}

		// Store alpha value
		m_alpha = alpha;

		// Move into 0..1 range
		float alphaNormalized = map( alpha, 0, 255, 0.0f, 1.0f );

		// Has alpha channel?
		if ( Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)m_format ) == 4 )
		{
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName( m_ogreMaterialName );
			material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setAlphaOperation( Ogre::LBX_MODULATE, Ogre::LBS_MANUAL, Ogre::LBS_TEXTURE, alphaNormalized );
		}
		else
		{
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName( m_ogreMaterialName );
			material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_MANUAL, Ogre::LBS_CURRENT, alphaNormalized);
		}
	}

	/*
	 * Sets the Ogre Scene Manager render queue for this quad. The higher the value (0..105) the later/more on top the quad is rendered
	 * The method is called "force" as this class usually handles automatically the render queue group for the object depending 
	 * on whether it is being rendered in 2d (x, y) or 3d (x, y, z). When this method is called, that is not set automatically anymore.
	 * You can to go back to the auto behaviour by calling restoreRenderQueue();
	 * @param group render queue. Important, the range is 0..105. There are no more render queues in Ogre.
	 */
	void TexturedQuad::forceRenderQueue( unsigned int renderQueueId )
	{
		// Check quad is valid
		if ( !isValid() )
		{
			LOG_ERROR( "forceRenderQueue. Error: Cannot force render queue as Image/Texture has not been initialized. Please call load or init before setting the render queue" );
			return;
		}

		// Check range
		if ( (renderQueueId < Ogre::RENDER_QUEUE_BACKGROUND) || (renderQueueId > Ogre::RENDER_QUEUE_MAX) )
		{
			LOG_WARNING( "TexturedQuad::forceRenderQueue. Render queue range in 0..105. Constraining received value, which is out of range" );
			renderQueueId = (unsigned int)constrain((float)renderQueueId, (float)Ogre::RENDER_QUEUE_BACKGROUND, (float)Ogre::RENDER_QUEUE_MAX);
		}

		m_renderQueueForced = true;
		m_forcedRenderQueue = renderQueueId;
		m_quad->setRenderQueueGroup( (Ogre::RenderQueueGroupID)m_forcedRenderQueue );
	}

	void TexturedQuad::enableDepthWrite( bool value )
	{
		// Check quad is valid
		if ( !isValid() )
		{
			LOG_ERROR( "enableDepthWrite. Error: set depth write value as Image/Texture has not been initialized. Please call load or init before setting the render queue" );
			return;
		}

		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(m_ogreMaterialName);
		material->getTechnique(0)->getPass(0)->setDepthWriteEnabled( value );
	}
	
	void TexturedQuad::enableDepthCheck( bool value )
	{
		// Check quad is valid
		if ( !isValid() )
		{
			LOG_ERROR( "enableDepthCheck. Error: set depth check value as Image/Texture has not been initialized. Please call load or init before setting the render queue" );
			return;
		}

		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(m_ogreMaterialName);
		material->getTechnique(0)->getPass(0)->setDepthCheckEnabled( value );
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
	void TexturedQuad::updateTexture( unsigned char* textureData, int width, int height, GraphicsType format )
	{
		// Check if the object is valid
		if ( !isValid() )
			THROW_EXCEPTION( "Trying to upload data texture to a not initializad TextureQuad object" );

		// Check resolution
		if ( ( width > m_textWidth ) || ( height > m_textHeight ) )
			THROW_EXCEPTION( "The resolution of the received data is bigger than the texture data size. Cannot update the texture" );

		//// Check format
		//if ( m_format != format )
		//	THROW_EXCEPTION( "Trying to update texture with different format" );

		// Update texture
		//m_ogreTexture->getBuffer( 0, 0 )->blitFromMemory( Ogre::PixelBox( width, height, 1, (Ogre::PixelFormat)format, textureData ) );
		Ogre::PixelBox newData( width, height, 1, (Ogre::PixelFormat)format, textureData );
		//Ogre::PixelBox dest( (size_t)m_textWidth, (size_t)m_textHeight, 1, (Ogre::PixelFormat)format, textureData );
		m_ogreTexture->getBuffer( 0, 0 )->blitFromMemory( newData );
	}

	/**
	 * @brief Returns true if the texture of this quad has alpha channel on it, and false otherwise
	 * @return true if the texture of this quad has alpha channel on it, and false otherwise
	 */
	bool TexturedQuad::hasAlpha() const
	{
		return Ogre::PixelUtil::hasAlpha( (Ogre::PixelFormat)m_format );
	}

	/**
	* @brief Flips the texture coordinates vertically
	*/
	void TexturedQuad::flipVertical( bool flip /*= true*/ )
	{
		// Update geometry to flip texture coordinates
		// We recreate the object instead of updating it because this is an extrange use, and for the normal
		// use is better in terms of performance to have static objects (in terms of vertex data)
		m_quad->clear();
		m_quad->begin( m_ogreMaterialName );

		if ( flip ) 
		{
			// m_quad positions and texture coordinates
			m_quad->position( -0.5, -0.5, 0.0);		m_quad->textureCoord( 0, 0 );
			m_quad->position( 0.5, -0.5, 0.0);		m_quad->textureCoord( m_textWidth / m_textWidthP2, 0 );
			m_quad->position( 0.5, 0.5, 0.0);		m_quad->textureCoord( m_textWidth / m_textWidthP2, m_textHeight / m_textHeightP2 );
			m_quad->position( -0.5, 0.5, 0.0);		m_quad->textureCoord( 0, m_textHeight / m_textHeightP2 );
		}
		else
		{
			m_quad->position( -0.5, -0.5, 0.0);	m_quad->textureCoord( 0, m_textHeight / m_textHeightP2 );
			m_quad->position( 0.5, -0.5, 0.0);	m_quad->textureCoord( m_textWidth / m_textWidthP2, m_textHeight / m_textHeightP2 );
			m_quad->position( 0.5, 0.5, 0.0);	m_quad->textureCoord( m_textWidth / m_textWidthP2, 0 );
			m_quad->position( -0.5, 0.5, 0.0);  m_quad->textureCoord( 0, 0 );
		}

		// quad indexes (two triangles)
		m_quad->triangle( 0, 1, 2 );
		m_quad->triangle( 0, 2, 3 );

		// Finish updating geometry
		m_quad->end();
	}

		/**
	* @brief Flips the texture coordinates horizontally
	*/
	void TexturedQuad::flipHorizontal( bool flip /*= true*/ )
	{
		// Update geometry to flip texture coordinates
		// We recreate the object instead of updating it because this is an extrange use, and for the normal
		// use is better in terms of performance to have static objects (in terms of vertex data)
		m_quad->clear();
		m_quad->begin( m_ogreMaterialName );

		if ( flip ) 
		{
			// m_quad positions and texture coordinates
			m_quad->position( -0.5, -0.5, 0.0);	m_quad->textureCoord( m_textWidth / m_textWidthP2, m_textHeight / m_textHeightP2 );
			m_quad->position( 0.5, -0.5, 0.0);	m_quad->textureCoord( 0, m_textHeight / m_textHeightP2 );
			m_quad->position( 0.5, 0.5, 0.0);	m_quad->textureCoord( 0, 0 );
			m_quad->position( -0.5, 0.5, 0.0);  m_quad->textureCoord( m_textWidth / m_textWidthP2, 0 );
		}
		else
		{
			m_quad->position( -0.5, -0.5, 0.0);	m_quad->textureCoord( 0, m_textHeight / m_textHeightP2 );
			m_quad->position( 0.5, -0.5, 0.0);	m_quad->textureCoord( m_textWidth / m_textWidthP2, m_textHeight / m_textHeightP2 );
			m_quad->position( 0.5, 0.5, 0.0);	m_quad->textureCoord( m_textWidth / m_textWidthP2, 0 );
			m_quad->position( -0.5, 0.5, 0.0);  m_quad->textureCoord( 0, 0 );
		}

		// quad indexes (two triangles)
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
		end(); // TODO: optimize this
		init( (int)other.m_textWidth, (int)other.m_textHeight, other.m_format );

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
	void TexturedQuad::setbackgroundRendering()
	{
		// Properties to be rendered as background (render queue and no depth write)
		if ( !m_renderQueueForced )
			m_quad->setRenderQueueGroup(Ogre::RENDER_QUEUE_BACKGROUND + 1);
		
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(m_ogreMaterialName);
		material->getTechnique(0)->getPass(0)->setDepthWriteEnabled( false );

		// The rest is the same as in 2d rendering
		set2dRendering();
	}

	/**
	* @internal
	* @brief Sets the necessary properties for the object to be rendered in 2d (in screen coordinates and size in pixels)
	*/
	void TexturedQuad::set2dRendering()
	{
		// Properties to be rendered in 2d
		if ( !m_renderQueueForced )
			m_quad->setRenderQueueGroup(Ogre::RENDER_QUEUE_OVERLAY -1);
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(m_ogreMaterialName);
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
		if ( !m_renderQueueForced )
			m_quad->setRenderQueueGroup( Ogre::RENDER_QUEUE_MAIN );
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(m_ogreMaterialName);
		m_quad->setUseIdentityProjection( false );
		m_quad->setUseIdentityView( false );

		// mark the object as 3d rendering
		m_render2D = false;
	}

	/**
	 * @internal
	 * @brief Configures render settings based on alpha of the image or the quad 
	 */
	void TexturedQuad::configureSceneBlending()
	{

		// If the image has alpha channel
		if ( hasAlpha() || (m_alpha < 255) )
		{
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(m_ogreMaterialName);
			material->getTechnique(0)->getPass(0)->setSceneBlending( Ogre::SBT_TRANSPARENT_ALPHA );

			enableDepthWrite(false);
			enableDepthCheck(true);
		}
		// This image has no alpha channel
		else
		{
			Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(m_ogreMaterialName);
			material->getTechnique(0)->getPass(0)->setSceneBlending( m_sbType );	
			enableDepthWrite(true);
			enableDepthCheck(true);
		}

	}
	
	/**
	 * @internal
	 * @brief Applies the current transformations (on the matrix stack) to the pivot node of the quad)
	 */
	void TexturedQuad::applyTransformations( float x, float y, float z, float imgWidth, float imgHeight )
	{
		// Get Current transformation
		Transform&	currentTransformation = GraphicsManager::getSingleton().m_transforms.top();

		// Reset pivot transformations
		m_pivotSceneNode->setPosition( Vector::ZERO );
		m_pivotSceneNode->setOrientation( Quaternion::IDENTITY );
		m_pivotSceneNode->setScale( Vector::UNIT_SCALE );

		// Note: if we extract from the transformatin matrix the rotation with getRotQuaternion()
		// The rotation seems to be affected by the matrix scale. To avoid this we extract he axis/angle pair
		// and build a Quaternion, which seems to solve the problem
		Matrix4 tranformation = currentTransformation.getTransformMatrix();
		Matrix3 rot;
		Vector axis;
		Ogre::Radian angle;
		Quaternion q;
		tranformation.extract3x3Matrix( rot );
		rot.ToAxisAngle( axis, angle );
		q.FromAngleAxis( angle, axis );

		// Apply current transformations to the pivot Node
		m_pivotSceneNode->translate( currentTransformation.getPosition() );
		//m_pivotSceneNode->rotate( currentTransformation.getRotQuaternion(), Ogre::Node::TS_PARENT );
		m_pivotSceneNode->setOrientation( q );
		m_pivotSceneNode->scale( currentTransformation.getScale() );

		// Finally, set scale for the quad scene node in the selected system
		if ( GraphicsManager::getSingleton().isProcessingMode() )
		{
			m_quadSceneNode->setScale( (float)imgWidth, (float)-imgHeight, 1 );
		}else{
			m_quadSceneNode->setScale( (float)imgWidth, (float)imgHeight, 1 );
		}

		// And the position, considering the current Image Mode
		if ( GraphicsManager::getSingleton().getImageMode() == CORNER )
			m_quadSceneNode->setPosition( x + imgWidth/2, y + imgHeight/2, z );
		// Center mode
		else 
			m_quadSceneNode->setPosition( x, y, z );

	}

	/**
	 * @internal
	 * @brief Applies the current transformations (on the matrix stack) to the pivot node of the quad)
	 */
	void TexturedQuad::applyTransformations2D( float x, float y, float imgWidth, float imgHeight )
	{
		// Get Current transformation
		Transform&	currentTransformation = GraphicsManager::getSingleton().m_transforms.top();

		// Reset pivot transformations
		m_pivotSceneNode->setPosition( Vector::ZERO );
		m_pivotSceneNode->setOrientation( Quaternion::IDENTITY );
		m_pivotSceneNode->setScale( Vector::UNIT_SCALE );

		// Now scale the pivot (coming from current transformation's  scale)
		m_pivotSceneNode->scale( currentTransformation.getScale() );

		// Pivot orientation Orientation
		m_pivotSceneNode->setOrientation( currentTransformation.getRotQuaternion() );

		// Calculate the transformed coordinates in 3d space
		Vector pos = currentTransformation.applyTransform( Vector(x, y, 0) );

		// Set properties of the quad, and set visible -> it will be rendered in the next render
		if ( GraphicsManager::getSingleton().isProcessingMode() )
		{
			// Set properties of the quad, and set visible -> it will be rendered in the next render
			setScale2d( imgWidth, -imgHeight );
			setPosition2d( pos.x, height-pos.y);
		}else{
			// Set properties of the quad, and set visible -> it will be rendered in the next render
			setScale2d( imgWidth, imgHeight );
			setPosition2d( pos.x, pos.y );
		}

		// Finally, Translate quad position, so that the image is drawn from the top-left corner
		if ( GraphicsManager::getSingleton().getImageMode() == CORNER )
			m_quadSceneNode->setPosition( m_2dWidth/2.0f, -m_2dHeight/2.0f, 0 );
		// If it's center, we don't need to change anything
	}

	/**
	 * @internal
	 * @brief Reset texture and quad sizes if needed. NOTE: Not tested yet!
	 */
	bool TexturedQuad::reset( int textureWidth, int textureHeight, GraphicsType format )
	{
		// Modify texture and quad size
		
		// Check if the class is already initialized
		if ( !isValid() )
			return false;

		// Get power of 2 texture size
		m_textWidthP2 = (float)textureWidth;
		m_textHeightP2 = (float)textureHeight;

		// Store the texture data
		m_textWidth     = (float)textureWidth;
		m_textHeight    = (float)textureHeight;	
		m_format		= format;
		m_render2D		= false;
		m_alpha			= 255;
	
		m_ogreTexture->setWidth((size_t)m_textWidth);
		m_ogreTexture->setHeight((size_t)m_textHeight);

		// Update vertex data
		m_quad->beginUpdate(0);

		// Center mode (Julio -> TODO)
		m_quad->position( -0.5, -0.5, 0.0);	m_quad->textureCoord( 0, m_textHeight / m_textHeightP2 );
		m_quad->position( 0.5, -0.5, 0.0);	m_quad->textureCoord( m_textWidth / m_textWidthP2, m_textHeight / m_textHeightP2 );
		m_quad->position( 0.5, 0.5, 0.0);	m_quad->textureCoord( m_textWidth / m_textWidthP2, 0 );
		m_quad->position( -0.5, 0.5, 0.0);  m_quad->textureCoord( 0, 0 );

		// m_quad indexes (two triangles)
		m_quad->triangle( 0, 1, 2 );
		m_quad->triangle( 0, 2, 3 );
		m_quad->end();

		// By default: 3d render
		set3dRendering();

		// calculate the necessary scale 2d values for scale 1 (initial) - so basically initializes m_2dWidth and m_2dHeight in case the image is rendered
		// in screen space in the first frame without setting the scale
		m_2dWidth = (m_textWidth / (float)width) * 2.0f;
		m_2dHeight = (m_textHeight / (float)height) * 2.0f;

		// The class is now initialized
		m_bIsValid = true;

		return true;
	}

	/**
	 * @brief Sets the material for the object
	 *
	 * @param materialName Name of the material to assign. It must be located in the data folder.
	 */
	void TexturedQuad::setMaterial( const std::string& materialName )
	{
		// Check if the material exist
		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName( materialName );
		if ( !material.isNull() )
		{
			m_quad->setMaterialName( 0, materialName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		}
		// TODO: Log -> material does not exist
		else
			LOG_ERROR( "Trying to set a material (%s) that does not exist", materialName.c_str() );
	}

	/**
	 * @brief This is a shortcut to set or change the main (or first) texture unit for the material used in the quad
	 *
	 * @param fileName Name of the image file to use (absolute or local to data folder)
	 */
	void TexturedQuad::setTexture( const std::string& fileName )
	{
		// Check if the class is already initialized
		if ( !isValid() || m_ogreMaterial.isNull() )
		{
			LOG_ERROR( "TexturedQuad::setTexture Error: Texture quad is not valid or has not bee initialized yet" );
			return;
		}

		// Change the texture name for the first texture unit state (checking all pointers are valid)
		if ( m_ogreMaterial->getTechnique(0) && m_quad->getSection(0) && m_ogreMaterial->getTechnique(0)->getPass(0) && m_ogreMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0) )
		{
			Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().getByName( m_quad->getSection(0)->getMaterialName() );
			Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().getByName( fileName );
			if ( texture.isNull() == false )
				mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->_setTexturePtr( texture );
			else
				LOG_ERROR( "TexturedQuad::setTexture. ERROR: Texture [%s] not found", fileName.c_str() );
		}
	}

	/**
	 * @brief Enables / disables this quad casting shadows
	 *
	 * @param castShadows true makes the material cast shadows, false disables it
	 */
	void TexturedQuad::enableCastShadows( bool castShadows )
	{
		// Check if the class is already initialized
		if ( !isValid() || !m_quad )
		{
			LOG_ERROR( "TexturedQuad::enableCastShadows Error: Texture quad is not valid or has not bee initialized yet" );
			return;
		}

		m_quad->setCastShadows( castShadows );
	}

	/**
	 * @brief Enables / disables this material being affected by dynamic lighting in the scene
	 *
	 * @param lighting true makes the material be altered by dynamic lighting, false, the material is not affected by
	 * global scene lighting or the lights in the scene
	 */
	void TexturedQuad::enableLighting( bool lighting )
	{
		// Check if the class is already initialized
		if ( !isValid() || m_ogreMaterial.isNull() )
		{
			LOG_ERROR( "TexturedQuad::enableLighting Error: Texture quad is not valid or has not bee initialized yet" );
			return;
		}

		if ( m_ogreMaterial->getTechnique(0) && m_ogreMaterial->getTechnique(0)->getPass(0) )
			m_ogreMaterial->getTechnique(0)->getPass(0)->setLightingEnabled( lighting );
	}

} // namespace Cing
