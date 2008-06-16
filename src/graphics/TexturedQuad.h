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

#ifndef _TexturedQuad_H_
#define _TexturedQuad_H_

#include "GraphicsPrereqs.h"

// Ogre
#include "OgreTexture.h"

namespace Graphics
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
	bool  init            ( int textureWidth, int textureHeight, ImageFormat format );
	void  end             ();

	// Set methods
	void  setPosition     ( float x, float y, float z );
	void  setPosition2d   ( float x, float y );
	void	setScale				( float xScale, float yScale, float zScale );
	void	setScale2d			( float xScale, float yScale );
	void  setVisible      ( bool visible );
	
	// Drawing methods
	void	draw						( float x, float y, float z );
	void	draw						( float x, float y, float z, float width, float height );
	void	draw2d					( float x, float y );
	void	draw2d					( float x, float y, float width, float height );

	// Texture update
	void	updateTexture		( const Ogre::Image& img );
	void  updateTexture   ( unsigned char* textureData, int width, int height, ImageFormat format );
	void  updateTexture   ( char* textureData, int width, int height, ImageFormat format ) { updateTexture( reinterpret_cast< unsigned char* >( textureData ), width, height, format ); }

	// Query methods
	bool  			isValid         () const { return m_bIsValid; }
	int					getTextWidth		() const { return m_textWidth;	}
	int					getTextHeight		() const { return m_textHeight;	}
	ImageFormat	getFormat				() const { return m_format;		}

	// Operators 
	void operator=				( const TexturedQuad& other );

private:

  // Private methods
  void	generateUniqueNames	();
	void	set2dRendering			();
	void	set3dRendering			();

  // Constant / static attributes
  static const std::string  MANUAL_OBJECT_NAME;     		///< Name of the manual object (which is the quad)
  static const std::string  MATERIAL_NAME;          		///< Name of the material where the texture will be placed
  static const std::string  TEXTURE_NAME;           		///< Name of the texture
  static long               m_quadCounter;          		///< Used to generate unique names for the quad materials, textures and ogre manual objects

	// Attributes
  Ogre::TexturePtr          m_ogreTexture;          		///< Ogre texture (to render the quad with it)  
  Ogre::SceneNode*          m_quadSceneNode;        		///< Quad scene node inside the scene (used to modify the scale, orientation...etc)
	Ogre::ManualObject*				m_quad;											///< Ogre manual object that contains the geometry (vertex) of the quad
	ImageFormat								m_format;										///< Format of the image
	Ogre::uint32							m_3dQueryFlags;							///< Query flags of ogre when the object is being renderd in 3d		

	float											m_textWidth, m_textHeight;  ///< Width and height of the texture
	float											m_2dWidth, m_2dHeight;			///< Width and height of the object when it is being drawn in 2d (in screen coordinates)
	float											m_2dXPos, m_2dYPos;					///< Position (in screen coordinates) of the object when it is being drawn in 2d

	std::string               m_ogreManualObjectName; 		///< Unique object name
  std::string               m_ogreTextureName;      		///< Unique texture name
  std::string               m_ogreMaterialName;     		///< Unique material name

	bool											m_visible;									///< Tells if the object is visible or not
  bool                      m_render2D;             		///< If true the quad is rendered in 2d, over the 3d scene
	bool                      m_bIsValid;	            		///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Graphics

#endif // _TexturedQuad_H_
