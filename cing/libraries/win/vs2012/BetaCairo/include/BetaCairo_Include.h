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


#ifndef __betacairo_include_H__
#define __betacairo_include_H__


#include "BetaCairo_Config.h"


#include <cairo.h>
#include "Ogre.h"
#include <string>
#include <map>

#include "BetaCairo_Library.h"
#include "BetaCairo_MicroHelpers.h"


namespace BetaCairo {

	class CairoManager;
	class CairoSurface;
	class Drawing;
	struct TextExtents;
	struct Pattern;
	struct DrawState;


#ifdef BETACAIRO_USESVG

	namespace SVG {
		class SVGDrawing;
		class SVGCodec;
	};

#endif


}
#endif