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
		int			halign;				/// RIGHT, CENTER, LEFT
		int			valign;				/// TOP, BOTTOM, CENTER, or BASELINE			
		TextMode	mode;				/// MODEL, SCREEN, (SHAPE? -> not for now)
		Color		color;
		bool		render2d;			/// If true, the text will be rendered in screen coordinates (2d)

		// Constructor
		FontProperties() 
		{
			// Default values
			fontName	= "DefaultFont";
			size		= 25;		// Use default size (the one specified in the ttf when loaed)
			x			= 0;
			y			= 0;
			z			= 0;
			width		= -1;		// Any width	(all the screen)
			height		= -1;		// Any height (all the screen)
			halign		= LEFT;
			valign		= TOP;
			color		= Color::White;
			render2d	= true;
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
			if ( halign != other.halign )					return false;
			if ( valign != other.valign )					return false;
			if ( mode	!= other.mode )						return false;
			if ( color	!= other.color )					return false;

			// all is the same...
			return true;
		}
	};

} // namespace Cing

#endif