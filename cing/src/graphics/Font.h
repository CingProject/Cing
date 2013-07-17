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

#ifndef _Cing_Font_H_
#define _Cing_Font_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "GraphicsPrereqs.h"

// Ogre
#include "Overlay/OgreFont.h"

// Common
#include "common/eString.h"


namespace Cing
{

	// Forward declarations
	struct FontProperties;

	/**
	* @internal
	* Class to print text to screen
	*/
	class Font
	{
	public:

		// Constructor / Destructor
		Font();
		~Font();

		// Init / Release / Update
		bool  load   ( const String& ttfName = DEFAULT_FONT_NAME, float size = DEFAULT_FONT_SIZE,  int resolution = DEFAULT_FONT_RESOLUTION );
		void  end    ();

		// Query methods
		const String&	getFontName			() const { return m_fontName; }
		float			getFontSize			() const { return m_fontSize; }
		float			getFontSizeOfTexture() const { return m_fontSizeOfTexture; }
		int				getFontResolution	() const { return m_fontRes; }
		float			getTextWitdhInPixels() const;
		int				getMaxBearingY		() const;
		Ogre::FontPtr	getOgreFont			() const { return Ogre::FontPtr( m_font ); }
		bool			isValid				() const { return m_bIsValid; }

		// Set/configure
		void			addCodePointRange	( unsigned int min, unsigned int max );
		void			updateFontAttributes( const FontProperties& fontProperties );
		void			reCreateFontTexture	();


		// Const static attributes
		static const std::string	DEFAULT_FONT_NAME;       ///< Default font name
		static const float			DEFAULT_FONT_SIZE;       ///< Default font size
		static const int			DEFAULT_FONT_RESOLUTION; ///< Default font resolution


	private:

		// Attributes
		Ogre::FontPtr					m_font;				///< Pointer to the Ogre Font used underneth
		String							m_fontName;			///< Name of the font (the .otf or .ttf file)
		float							m_fontSize;			///< Font size used to draw text (it could be different than the original size used to create the texture)
		float							m_fontSizeOfTexture;///< Size of the font used to create the texture (this is what really will define the sharpness of the font)
		int								m_fontRes;			///< Resolution of the TTF font

		// Code point range (if added by user)
		Ogre::Font::CodePointRangeList	m_codePointRangeList;///< This allows the user to add wider code point range to include special characters like accents or apostrophes

		bool							m_bIsValid;			///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

	};

} // namespace Cing

#endif // _Text_H_
