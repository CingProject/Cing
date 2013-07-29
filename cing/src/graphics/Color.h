/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community 

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

#ifndef _Cing_Color_h_
#define _Cing_Color_h_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "GraphicsPrereqs.h"

// Ogre
#include "OgreColourValue.h"

namespace Cing
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
	Color( float red,  float green, float blue );
	Color( float red,  float green, float blue, float alpha );

	Color( int gray );
	Color( int gray, float alpha );

	Color( const Ogre::ColourValue& color );

	// To set values or range
	void	set	( float red, float green, float blue, float alpha = 255.0f );

	// Getters
	float	getHighRange() const { return m_hightRange; }
	float	getLowRange	() const { return m_lowRange; }

	// Color conversion
	Color	normalized	() const;
	void	normalize	();

	// Color mode
	static void					colorMode	( GraphicsType mode, float range1, float range2, float range3, float range4 );
	static const GraphicsType	getColorMode(){ return m_colorMode; };

	// TODO
	// Operators

private:

	// Attributes
	float m_lowRange, m_hightRange; // Color values range. Default 0..255

	// Static attributes
	static GraphicsType	m_colorMode;
	static float				m_rRange;
	static float				m_gRange;
	static float				m_bRange;
};

} // namespace Cing

#endif // _Color_h_
