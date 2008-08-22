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

#ifndef _Color_h_
#define _Color_h_

#include "GraphicsPrereqs.h"

// Ogre
#include "externLibs/Ogre3d/include/OgreColourValue.h"

namespace Graphics
{

/**
 * @brief Stores the information about a color. The color range can be any, but by default is 0.255.
 * The default color model is RGB (red green and blue), and a fourth value, alpha, can be used to represent transparency
 *
 * This color translates from any color range, to 0..1 range that Ogre graphics engine understands, and also
 * provides an interface for al color related operations (such as change of color mode)
 */
class Color: public Ogre::ColourValue
{
public:

	// Constructor: Transforms color to Ogre range
	Color( float gray = 255 );
	Color( float gray, float alpha );
	Color( float  red, float green, float blue );
	Color( float  red, float green, float blue, float alpha );

	// To set values or range
	void	set					( float red, float green, float blue, float alpha = 255.0f );

	// Color conversion
	Color	normalized	() const;
	void	normalize		();

private:

	// Attributes
	float m_lowRange, m_hightRange; // Color values range. Default 0..255
};

} // namespace Graphics

#endif // _Color_h_
