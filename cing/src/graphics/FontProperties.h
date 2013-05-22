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