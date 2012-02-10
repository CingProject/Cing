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

#ifndef _FontProperties_H_
#define _FontProperties_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "GraphicsPrereqs.h"
#include "Color.h"
#include "Font.h"
#include "MovableText.h"

#include "common/eString.h"
#include "common/MathUtils.h"
#include "common/CommonConstants.h"

namespace Cing
{

	/// Store the properties associated to a text block
	struct FontProperties
	{
		String		fontName;
		String		text;
		float		size;
		float		x, y, z, width, height;
		float		leading;			/// Size in pixels for spacing between lines
		int			resolution;			/// Resolution (for True Type fonts)
		int			halign;				/// RIGHT, CENTER, LEFT
		int			valign;				/// TOP, BOTTOM, CENTER, or BASELINE			
		Color		color;
		bool		render2d;			/// If true, the text will be rendered in screen coordinates (2d)
		bool		wordWrap;			/// If true, words will not be split when rendering text within a box, if possible. If false, words will be split if necessary

		// Constructor
		FontProperties() 
		{
			// Default values
			fontName	= "DefaultFont";				// Should be Font::DEFAULT_FONT_NAME but for some reason it appears blank when assigning so
			size		= Font::DEFAULT_FONT_SIZE;		// Use default size (the one specified in the ttf when loaed)
			resolution	= Font::DEFAULT_FONT_RESOLUTION;
			x			= 0;
			y			= 0;
			z			= 0;
			width		= -1;		// Any width	(all the screen)
			height		= -1;		// Any height (all the screen)
			leading		= -1;		// Default leading
			halign		= LEFT;
			valign		= TOP;
			color		= Color::White;
			render2d	= true;
			wordWrap	= true;
		}

		// Comparison operator
		bool operator==( const FontProperties& other ) 
		{
			if ( text != other.text )						return false;
			if ( !equal( size, other.size) )				return false;
			if ( !equal( x, other.x) ) 						return false;
			if ( !equal( y, other.y) ) 						return false;
			if ( !equal( width, other.width) )				return false;
			if ( !equal( height, other.height) )			return false;
			if ( !resolution != other.resolution )			return false;
			if ( !equal( leading, other.leading) )			return false;
			if ( halign != other.halign )					return false;
			if ( valign != other.valign )					return false;
			if ( color	!= other.color )					return false;
			if ( wordWrap != other.wordWrap )				return false;

			// all is the same...
			return true;
		}
	};

} // namespace Cing

#endif