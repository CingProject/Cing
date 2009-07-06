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

#include "Font.h"



// OGRE
#include "externLibs/Ogre3d/include/OgreOverlayManager.h"
#include "externLibs/Ogre3d/include/OgreOverlayContainer.h"
#include "externLibs/Ogre3d/include/OgreStringConverter.h"
#include "externLibs/Ogre3d/include/OgreTextAreaOverlayElement.h"
#include "externLibs/Ogre3d/include/OgreFontManager.h"

namespace Graphics
{

	// Static member definition
	const std::string	Font::DEFAULT_FONT_NAME       = "DefaultFont";
	const int			Font::DEFAULT_FONT_SIZE       = 16;
	const int			Font::DEFAULT_FONT_RESOLUTION = 96;

	/**
	* @internal
	* @brief Constructor. Initializes class attributes.
	*/
	Font::Font():
	m_bIsValid( false )
	{
	}

	/**
	* @internal
	* @brief Destructor. Class release.
	*/
	Font::~Font()
	{
		// Release resources
		end();
	}

	/**
	* @internal
	* @brief Initializes the class so it becomes valid.
	* @param ttfName	Name of the true type font to load (example: "LucidaSans.ttf"). It shoud be in the data folder
	* @param fize		Size of the true type font (points). Default 16
	* @param resolution Resolution of the font (dpi) when rasterized (rendered into a texture). Default 96
	* @return true if the initialization was ok | false otherwise
	*/
	bool Font::load( const String& ttfName /*= DEFAULT_FONT_NAME*/, int size /*= DEFAULT_FONT_SIZE*/, int resolution /*= DEFAULT_FONT_RESOLUTION*/ )
	{
		// Check if the class is already initialized
		if ( isValid() )
			return true;

		// PArametros por tocar sobre font
		// antialias_colour
		// Parametros para tocar sobre textOverlay
		//m_pText->setParameter("char_height", "16");
		//m_pText->setParameter("horz_align", "center");
		// char_height",  "Sets the height of the characters in relation to the screen."
		// space_width,  "Sets the width of a space in relation to the screen."
		// font_name
		// colour, "Sets the colour of the font (a solid colour)."
		// colour_bottom, "Sets the colour of the font at the bottom (a gradient colour)."
		// colour_top, "Sets the colour of the font at the top (a gradient colour)."
		// alignment,  "Sets the alignment of the text: 'left', 'center' or 'right'."



		// Store the font name data
		m_fontName	= ttfName;
		m_fontSize	= size;
		m_fontRes	= resolution;

		// If it is not the defautl font -> create it
		if (ttfName != "DefaultFont" )
		{
			// get the font manager
			Ogre::FontManager &fontMgr = Ogre::FontManager::getSingleton();

			// create a font resource
			m_font = fontMgr.create( ttfName, "General" );

			// set as truetype (TODO: support images)
			m_font->setType( Ogre::FT_TRUETYPE );

			// set the .ttf file name
			m_font->setSource( ttfName );

			// set the size
			m_font->setTrueTypeSize( size );

			// set the dpi
			m_font->setTrueTypeResolution( resolution );

			// Generate all character range
			m_font->addCodePointRange(Ogre::Font::CodePointRange(33, 255));

			// load the ttf (creates the texture with the font characters)
			m_font->load();
		}

		// The class is now initialized
		m_bIsValid = true;

		return true;
	}

	/**
	* @internal
	* @brief Releases the class resources. 
	* After this method is called the class is not valid anymore.
	*/
	void Font::end()
	{
		// Check if the class is already released
		if ( !isValid() )
			return;

		// The class is not valid anymore
		m_bIsValid = false;
	}

	/**
	* @brief Returns the width of the text (in pixels) taking in to account the current font, size...etc
	*/
	float Font::getTextWitdhInPixels() const
	{
		// Check if the class is ok
		if ( !isValid() )
			return 0.0f;

		float pixelWidth = 0;
		for(String::const_iterator i = m_fontName.begin(); i < m_fontName.end();i++)
		{   
			if (*i == 0x0020)
				pixelWidth += m_font->getGlyphAspectRatio(0x0030);
			else
			{
				pixelWidth += m_font->getGlyphAspectRatio(*i);
			}
		}

		pixelWidth *= m_fontSize;
		return pixelWidth;
	}



} // namespace Graphics
