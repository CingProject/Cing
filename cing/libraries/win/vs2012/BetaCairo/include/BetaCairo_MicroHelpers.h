/*

	BetaCairo a wrapper for the Cairo Graphics library
	Copyright (C) 2005, 2006 Robin Southern http://get.nxogre.org/cetacairo/

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
#ifndef __betacairo_microhelpers_H__
#define __betacairo_microhelpers_H__

#include "BetaCairo_Include.h"

namespace BetaCairo {

	enum Unit {
		PIXEL,
		PICA,
		PT,
		CM,
		MM,
		INCH
	};

	enum AntiAliasing {
		AA_Cairo_Default = CAIRO_ANTIALIAS_DEFAULT,
		AA_None = CAIRO_ANTIALIAS_NONE,
		AA_Gray = CAIRO_ANTIALIAS_GRAY,
		AA_Subpixel = CAIRO_ANTIALIAS_SUBPIXEL	
	};

	enum PixelFormat {
		PF_RGBA = CAIRO_FORMAT_ARGB32,
		PF_RGB = CAIRO_FORMAT_RGB24
	};

	enum DrawOperator {
		Operator_Clear = CAIRO_OPERATOR_CLEAR,
		Operator_Source = CAIRO_OPERATOR_SOURCE,
		Operator_Over = CAIRO_OPERATOR_OVER,
		Operator_In = CAIRO_OPERATOR_IN,
		Operator_Out = CAIRO_OPERATOR_OUT,
		Operator_ATop = CAIRO_OPERATOR_ATOP,
		Operator_Dest = CAIRO_OPERATOR_DEST,
		Operator_Dest_Over = CAIRO_OPERATOR_DEST_OVER,
		Opeartor_Dest_In = CAIRO_OPERATOR_DEST_IN,
		Operator_Dest_Out = CAIRO_OPERATOR_DEST_OUT,
		Operator_Dest_ATop = CAIRO_OPERATOR_DEST_ATOP,
		Operator_Xor = CAIRO_OPERATOR_XOR,
		Operator_Add = CAIRO_OPERATOR_ADD,
		Operator_Saturate = CAIRO_OPERATOR_SATURATE
	};

	enum FontWeight {
	  Font_Slant_Normal = CAIRO_FONT_SLANT_NORMAL,
	  Font_Slant_Italic = CAIRO_FONT_SLANT_ITALIC,
	  Font_Slant_Oblique = CAIRO_FONT_SLANT_OBLIQUE
	};

	enum FontSlant {
	  Font_Weight_Normal = CAIRO_FONT_WEIGHT_NORMAL,
	  Font_Weight_Bold = CAIRO_FONT_WEIGHT_BOLD
	};

	enum LineEnds {
		LineEnd_Butt = CAIRO_LINE_CAP_BUTT, // Heh.
		LineEnd_Round = CAIRO_LINE_CAP_ROUND,
		LineEnd_Square = CAIRO_LINE_CAP_SQUARE
	};


	enum LineJoin {
		LineJoin_Miter = CAIRO_LINE_JOIN_MITER,
		LineJoin_Round = CAIRO_LINE_JOIN_ROUND,
		LineJoin_Bevel = CAIRO_LINE_JOIN_BEVEL
	};


	struct _BetaCairoExport TextExtents {
		float Xbearing;
		float Ybearing;
		float Width;
		float Height;
		float Xadvance;
		float Yadvance;
		
		float getFullWidth() {
			return Width + Xbearing;}
				
		float getFullHeight() {
			return Height;} /// + Ybearing removed yBearing because for most fonts it returns a minus.}
		
	};

	struct _BetaCairoExport Pattern {
		cairo_pattern_t* pattern;

		void linear(float x, float y, float w, float h) {
			pattern = cairo_pattern_create_linear(x,y,w,h);}

		void radial(float x, float y, float radius1, float w, float h, float radius2) {
			pattern = cairo_pattern_create_radial(x,y,radius1,w,h,radius2);}

		void addStop(float offset, Ogre::ColourValue c) {
			cairo_pattern_add_color_stop_rgba(pattern, offset, c.r, c.g,c.b,c.a);}

		void destroy() {
			cairo_pattern_destroy(pattern);}

	};

	struct _BetaCairoExport DrawState {

		std::string font;
		float fontSize;
		float linethickness;
		Ogre::ColourValue workingColour;
		// ...

	};

	class _BetaCairoExport Colour {
		
	public:
			
		enum colours {
			aliceblue=0xf0f8ff,
			antiquewhite=0xfaebd7,
			aqua=0x00ffff,
			aquamarine=0x7fffd4,
			azure=0xf0ffff,
			beige=0xf5f5dc,
			bisque=0xffe4c4,
			black=0x000000,
			blanchedalmond=0xffebcd,
			blue=0x0000ff,
			blueviolet=0x8a2be2,
			brown=0xa52a2a,
			burlywood=0xdeb887,
			cadetblue=0x5f9ea0,
			chartreuse=0x7fff00,
			chocolate=0xd2691e,
			coral=0xff7f50,
			cornflowerblue=0x6495ed,
			cornsilk=0xfff8dc,
			crimson=0xdc143c,
			cyan=0x00ffff,
			darkblue=0x00008b,
			darkcyan=0x008b8b,
			darkgoldenrod=0xb8860b,
			darkgray=0xa9a9a9,
			darkgreen=0x006400,
			darkgrey=0xa9a9a9,
			darkkhaki=0xbdb76b,
			darkmagenta=0x8b008b,
			darkolivegreen=0x556b2f,
			darkorange=0xff8c00,
			darkorchid=0x9932cc,
			darkred=0x8b0000,
			darksalmon=0xe9967a,
			darkseagreen=0x8fbc8f,
			darkslateblue=0x483d8b,
			darkslategray=0x2f4f4f,
			darkslategrey=0x2f4f4f,
			darkturquoise=0x00ced1,
			darkviolet=0x9400d3,
			deeppink=0xff1493,
			deepskyblue=0x00bfff,
			dimgray=0x696969,
			dimgrey=0x696969,
			dodgerblue=0x1e90ff,
			firebrick=0xb22222,
			floralwhite=0xfffaf0,
			forestgreen=0x228b22,
			fuchsia=0xff00ff,
			gainsboro=0xdcdcdc,
			ghostwhite=0xf8f8ff,
			gold=0xffd700,
			goldenrod=0xdaa520,
			gray=0x808080,
			green=0x008000,
			greenyellow=0xadff2f,
			grey=0x808080,
			honeydew=0xf0fff0,
			hotpink=0xff69b4,
			indianred=0xcd5c5c,
			indigo=0x4b0082,
			ivory=0xfffff0,
			khaki=0xf0e68c,
			lavender=0xe6e6fa,
			lavenderblush=0xfff0f5,
			lawngreen=0x7cfc00,
			lemonchiffon=0xfffacd,
			lightblue=0xadd8e6,
			lightcoral=0xf08080,
			lightcyan=0xe0ffff,
			lightgoldenrodyellow=0xfafad2,
			lightgray=0xd3d3d3,
			lightgreen=0x90ee90,
			lightgrey=0xd3d3d3,
			lightpink=0xffb6c1,
			lightsalmon=0xffa07a,
			lightseagreen=0x20b2aa,
			lightskyblue=0x87cefa,
			lightslategray=0x778899,
			lightslategrey=0x778899,
			lightsteelblue=0xb0c4de,
			lightyellow=0xffffe0,
			lime=0x00ff00,
			limegreen=0x32cd32,
			linen=0xfaf0e6,
			magenta=0xff00ff,
			maroon=0x800000,
			mediumaquamarine=0x66cdaa,
			mediumblue=0x0000cd,
			mediumorchid=0xba55d3,
			mediumpurple=0x9370db,
			mediumseagreen=0x3cb371,
			mediumslateblue=0x7b68ee,
			mediumspringgreen=0x00fa9a,
			mediumturquoise=0x48d1cc,
			mediumvioletred=0xc71585,
			midnightblue=0x191970,
			mintcream=0xf5fffa,
			mistyrose=0xffe4e1,
			moccasin=0xffe4b5,
			navajowhite=0xffdead,
			navy=0x000080,
			oldlace=0xfdf5e6,
			olive=0x808000,
			olivedrab=0x6b8e23,
			orange=0xffa500,
			orangered=0xff4500,
			orchid=0xda70d6,
			palegoldenrod=0xeee8aa,
			palegreen=0x98fb98,
			paleturquoise=0xafeeee,
			palevioletred=0xdb7093,
			papayawhip=0xffefd5,
			peachpuff=0xffdab9,
			peru=0xcd853f,
			pink=0xffc0cb,
			plum=0xdda0dd,
			powderblue=0xb0e0e6,
			purple=0x800080,
			red=0xff0000,
			rosybrown=0xbc8f8f,
			royalblue=0x4169e1,
			saddlebrown=0x8b4513,
			salmon=0xfa8072,
			sandybrown=0xf4a460,
			seagreen=0x2e8b57,
			seashell=0xfff5ee,
			sienna=0xa0522d,
			silver=0xc0c0c0,
			skyblue=0x87ceeb,
			slateblue=0x6a5acd,
			slategray=0x708090,
			slategrey=0x708090,
			snow=0xfffafa,
			springgreen=0x00ff7f,
			steelblue=0x4682b4,
			tan=0xd2b48c,
			teal=0x008080,
			thistle=0xd8bfd8,
			tomato=0xff6347,
			turquoise=0x40e0d0,
			violet=0xee82ee,
			wheat=0xf5deb3,
			white=0xffffff,
			whitesmoke=0xf5f5f5,
			yellow=0xffff00,
			yellowgreen=0x9acd32
		};

		static Ogre::ColourValue fromRGB(Ogre::uint r, Ogre::uint g, Ogre::uint b);
		static Ogre::ColourValue fromRGBA(Ogre::uint r, Ogre::uint g, Ogre::uint b, Ogre::uint a);
		static Ogre::ColourValue fromHSV(float hue, float saturation, float value, float alpha = 1.0f);
		static Ogre::ColourValue fromHexString(std::string);
		static Ogre::ColourValue fromColour(colours col, float alpha = 1.0f);

	

	};


}

#endif

