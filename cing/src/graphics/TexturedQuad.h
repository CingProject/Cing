/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

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

#ifndef _TexturedQuad_H_
#define _TexturedQuad_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "GraphicsPrereqs.h"

#include "common/CommonTypes.h"
#include "common/eString.h"

// Ogre
#include "OgreTexture.h"

namespace Cing
{

	/**
	* @internal
	* Represents a quad (two triangles) with a texture (dynamically update possible)
	*/
	class TexturedQuad
	{
	public:

		// Constructor / Destructor
		TexturedQuad();
		~TexturedQuad();

		// Init / Release 
		bool  init				( int textureWidth, int textureHeight, GraphicsType format, GraphicsType usage = DYNAMIC, Ogre::SceneManager* sm = NULL );
		void  end				();
		bool  reset				( int textureWidth, int textureHeight, GraphicsType format );

		// Set methods
		void  	setPosition     ( float x, float y, float z );
		void  	setPosition2d   ( float x, float y );
		void	setScale		( float xScale, float yScale, float zScale );
		void	setScale2d		( float xScale, float yScale );
		void	setOrientation	( const Vector& axis, float angle );
		void	setOrientation	( const Quaternion& orientation );
		void	rotate			( const Vector& axis, float angle );
		void  	setVisible      ( bool visible );
		void  	setAdditiveMode ( bool value );

		// Drawing methods
		void	draw			( float x, float y, float z );
		void	draw			( float x, float y, float z, float width, float height );
		void	draw			(float x1, float y1, float z1,	float x2, float y2, float z2,
								 float x3, float y3, float z3,	float x4, float y4, float z4);

		void	draw2d			( float x, float y );
		void	draw2d			( float x, float y, float width, float height );
		void	draw2d			( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 );
		void	drawBackground	( float x, float y );
		void	drawBackground	( float x, float y , float imgWidth, float imgHeight );

		// Texture update
		void	updateTexture	( const Ogre::Image& img );
		void	updateTexture	( unsigned char* textureData, int width, int height, GraphicsType format );
		void	updateTexture	( char* textureData, int width, int height, GraphicsType format ) { updateTexture( reinterpret_cast< unsigned char* >( textureData ), width, height, format ); }

		// Query methods
		bool  					isValid         () const 	{ return m_bIsValid; }
		int						getTextWidth	() const 	{ return (int)m_textWidth;	}
		int						getTextHeight	() const 	{ return (int)m_textHeight;	}
		GraphicsType			getFormat		() const 	{ return m_format;		}
		Ogre::ManualObject*		getManualObject	() 			{ return m_quad;		}
		Ogre::MaterialPtr		getMaterial		() 			{ return m_ogreMaterial; }
		Ogre::TexturePtr		getOgreTexture	() const 	{ return m_ogreTexture; }
		Ogre::SceneNode*		getSceneNode	()			{ return m_quadSceneNode;   }
		const Ogre::SceneNode*	getSceneNode	() const	{ return m_quadSceneNode;   }
		Ogre::SceneNode*		getPivotSceneNode()			{ return m_pivotSceneNode;   }
		const String&			getMaterialName	() const	{ return m_ogreMaterialName; }
		const String&			getTextureName	() const	{ return m_ogreTextureName; }
		bool					hasAlpha		() const;

		// Texture coordinate control
		void					flipVertical	(bool flip = true);
		void					flipHorizontal	(bool flip = true);

		// Operators 
		void					operator = ( const TexturedQuad& other );

		// Texture / material control
		void					setTransparency		( float alpha );
		void					forceRenderQueue	( unsigned int renderQueueId );
		void					restoreRenderQueue	() { m_renderQueueForced = false; }
		void					enableDepthWrite	( bool value );
		void					enableDepthCheck	( bool value );

		void					setMaterial			( const std::string& materialName );
		void					setTexture			( const std::string& fileName );

		void					setSceneNode		( Ogre::SceneNode* node)   { m_quadSceneNode = node; };
		void					setPivotSceneNode	( Ogre::SceneNode* node)   { m_pivotSceneNode = node; };

		Ogre::SceneManager*		getSceneManager	() { return m_sm; }

		// Lighting, Shadow and other material settings
		void					enableLighting		( bool lighting );
		void					enableCastShadows	( bool castShadows );

		void					setSceneBlendType	( Ogre::SceneBlendType type );

	protected:

		// Private methods
		void	generateUniqueNames		();
		void	set2dRendering			();
		void	set3dRendering			();
		void	configureSceneBlending	();
		void	setbackgroundRendering	();
		void	applyTransformations	( float x, float y, float z, float imgWidth, float imgHeight );
		void	applyTransformations2D	( float x, float y, float imgWidth, float imgHeight );

		// Constant / static attributes
		static const std::string  MANUAL_OBJECT_NAME;     			///< Name of the manual object (which is the quad)
		static const std::string  MATERIAL_NAME;          			///< Name of the material where the texture will be placed
		static const std::string  TEXTURE_NAME;           			///< Name of the texture
		static long               m_quadCounter;          			///< Used to generate unique names for the quad materials, textures and ogre manual objects

		// Attributes
		Ogre::SceneManager*			m_sm;							///< SceneManager
		Ogre::MaterialPtr			m_ogreMaterial;					///< Material used to render the quad
		Ogre::TexturePtr			m_ogreTexture;          		///< Ogre texture (to render the quad with it)  
		Ogre::SceneNode*			m_quadSceneNode;        		///< Quad scene node inside the scene (used to modify the scale, orientation...etc)
		Ogre::SceneNode*			m_pivotSceneNode;        		///< Pivot scene node (parent of the quadSceneNode. Used to concatenate transformations
		Ogre::ManualObject*			m_quad;							///< Ogre manual object that contains the geometry (vertex) of the quad
		GraphicsType				m_format;						///< Format of the image
		float						m_alpha;						///< Transparency of the object. Range 0...255.
		
		Ogre::uint8					m_forcedRenderQueue;			///< Render queue to be used by this quad. Forced by the user
		bool						m_renderQueueForced;			///< True if the render queue has been forced by the user

		float						m_xScale, m_yScale, m_zScale;	///< Scale of the node

		float						m_textWidth, m_textHeight;		///< Width and height of the texture
		float						m_textWidthP2, m_textHeightP2;  ///< Width and height of the texture
		float						m_2dWidth, m_2dHeight;			///< Width and height of the object when it is being drawn in 2d (in screen coordinates)
		float						m_2dXPos, m_2dYPos;					///< Position (in screen coordinates) of the object when it is being drawn in 2d

		String               		m_ogreManualObjectName; 		///< Unique object name
		String               		m_ogreTextureName;      		///< Unique texture name
		String               		m_ogreMaterialName;     		///< Unique material name

		bool						m_visible;						///< Tells if the object is visible or not
		bool						m_render2D;             		///< If true the quad is rendered in 2d, over the 3d scene
		bool						m_bIsValid;	            		///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

		Ogre::SceneBlendType		m_sbType;							///< Scene blend operation
	};

} // namespace Cing

#endif // _TexturedQuad_H_
