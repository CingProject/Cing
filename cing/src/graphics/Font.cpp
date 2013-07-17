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

#include "Font.h"
#include "FontProperties.h"

// OGRE
#include "Overlay/OgreOverlayManager.h"
#include "Overlay/OgreOverlayContainer.h"
#include "Overlay/OgreTextAreaOverlayElement.h"
#include "Overlay/OgreFontManager.h"
#include "OgreStringConverter.h"

namespace Cing
{

	// Static member definition
	const std::string	Font::DEFAULT_FONT_NAME       = "DefaultFont";
	const float			Font::DEFAULT_FONT_SIZE       = 16;
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
	bool Font::load( const String& ttfName /*= DEFAULT_FONT_NAME*/, float size /*= DEFAULT_FONT_SIZE*/, int resolution /*= DEFAULT_FONT_RESOLUTION*/ )
	{
		// Check if the class is already initialized
		if ( isValid() )
			return true;

		// Available ogre font parameters
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
		m_fontSizeOfTexture = size;
		m_fontRes	= resolution;

		// If it's the default font, don't load, just get it's pointer
		if (ttfName == "DefaultFont" )
		{
			// get the font manager
			//Ogre::FontManager &fontMgr = Ogre::FontManager::getSingleton();
			Ogre::FontManager* fontMgr = Ogre::FontManager::getSingletonPtr();


			// get font pointer and load it
			m_font = fontMgr->getByName( ttfName, "General" );
			if ( !m_font.isNull() )
				m_font->load();
		}
		// If it is not the defautl font -> create it
		else
		{
			// get the font manager
			Ogre::FontManager &fontMgr = Ogre::FontManager::getSingleton();

			// create a font resource
			m_font = fontMgr.create( ttfName, "General" );

			if ( !m_font.isNull() )
			{

				// set as truetype (TODO: support images)
				m_font->setType( Ogre::FT_TRUETYPE );

				// set the .ttf file name
				m_font->setSource( ttfName );

				// set the size
				m_font->setTrueTypeSize( size );

				// set the dpi
				m_font->setTrueTypeResolution( resolution );

				// Generate supported character range (if the user added a specific range, otherwise default 33-166 will be used)
				// NOTE: Complete character set -> until 512, Details about UTF-8 codepoints here: http://www.utf8-chartable.de/
				if ( m_codePointRangeList.empty() == false )
				{
					Ogre::Font::CodePointRangeList::iterator it;
					for ( it = m_codePointRangeList.begin(); it != m_codePointRangeList.end(); ++it )
					{
						m_font->addCodePointRange( *it );
					}
				}
				
				// load the ttf (creates the texture with the font characters)
				m_font->load();
			}
		}

		// Set the class valid if the font was loded
		m_bIsValid = !m_font.isNull() &&  m_font->isLoaded();

		// Report
		if ( m_bIsValid )
			LOG( "Font succesfully loaded: %s", ttfName.c_str() );
		else
			LOG_ERROR( "Font could NOT be loaded: %s. Is the absolute path correct? or is the font in the data folder? Is the file .otf or .ttf?",  ttfName.c_str() );
	

		return m_bIsValid;
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
		{
			LOG_ERROR( "Font::getTextWitdhInPixels. Error: Font is not valid. Please call Font::load before" );
			return 0.0f;
		}

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

	/**
	* @brief Returns the width of the text (in pixels) taking in to account the current font, size...etc
	*/
	int Font::getMaxBearingY() const
	{
		// Check if the class is ok
		if ( !isValid() )
		{
			LOG_ERROR( "Font::getMaxBearingY. Error: Font is not valid. Please call Font::load before" );
			return 0;
		}
		return m_font->getTrueTypeMaxBearingY();
	}

	/*
	 * Adds a code point range to the supported character range for this font.
	 * It allows to support wider character range like accents or apostrophes, but it will make the font use bigger 
	 * textures, so if you don't need special characters, no need to call this :)
	 * @note This should be called before the load method is called.
	 * @param fontProperties Font properties to be set into the current font.	 
	 */ 
	void Font::addCodePointRange( unsigned int min, unsigned int max )
	{
		m_codePointRangeList.push_back( Ogre::Font::CodePointRange(min, max) );
	}


	/*
	 * Sets the font properties from those received as parameters
	 * @param fontProperties Font properties to be set into the current font.	 
	 */ 
	void Font::updateFontAttributes( const FontProperties& fontProperties )
	{
		// Check if the class is ok
		if ( !isValid() )
		{
			LOG_ERROR( "Font::updateFontAttributes. Error: Font is not valid. Please call Font::load before" );
			return;
		}

		m_fontName	= fontProperties.fontName;
		m_fontSize	= fontProperties.size;
		m_fontRes	= fontProperties.resolution;

		m_font->setSource( m_fontName );
		m_font->setTrueTypeSize( m_fontSize );
		m_font->setTrueTypeResolution( m_fontRes );

		// If the new font size is bigger than the current TTF size of this font, re create the texture (it's slow, but otherwise could look blurry)
		if ( m_fontSize > getFontSizeOfTexture() )
			reCreateFontTexture();
	}

	/*
	 * @internal 
	 * Re-creates the texture associated with this texture. This could be necesssary if trying to draw a font with a size
	 * higher than it was initially created (it will be blurry). Just change it calling textFont( font, newSize ) and it will be recreated.
	 */ 
	void Font::reCreateFontTexture()
	{
		// Check if the class is ok
		if ( !isValid() )
		{
			LOG_ERROR( "Font::reCreateFontTexture. Error: Font is not valid. Please call Font::load before" );
			return;
		}

		// Reload font (will re-create the texture)
		m_font->reload();

		// Update the font size on texture
		m_fontSizeOfTexture = m_fontSize;
	}



} // namespace Cing
