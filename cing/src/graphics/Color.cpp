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

// Precompiled headers
#include "Cing-Precompiled.h"

#include "Color.h"

// Common
#include "common/MathUtils.h"

namespace Cing
{

/// Static member initialization
	GraphicsType Color::m_colorMode = RGB;
	float  Color::m_rRange = 255.0f ;
	float  Color::m_gRange = 255.0f ;
	float  Color::m_bRange = 255.0f ;

/**
 * @brief Initializes the color with a specific color values (depending on the color range
 * which by default is 0..255, and RGB color model (red, green, blue).
 *
 * @param gray Gray color. Red, green and blue with the same value
 */
Color::Color( float gray /*= 255*/ ): 
	m_lowRange( 0.0f ), 
	m_hightRange( 255.0f )
{
	set( gray, gray, gray, m_hightRange );
}


/**
 * @brief Initializes the color with a specific color values (depending on the color range
 * which by default is 0..255, and RGB color model (red, green, blue).
 *
 * @param gray	Gray color. Red, green and blue with the same value
 * @param alpha Alpha value. Represents transparency. Max value means opaque, min value means transparent.
 */
Color::Color( float gray, float alpha ): 
	m_lowRange( 0.0f ), 
	m_hightRange( 255.0f )
{
	set( gray, gray, gray, alpha );
}

/**
 * @brief Initializes the color with a specific color values (depending on the color range
 * which by default is 0..255, and RGB color model (red, green, blue).
 *
 * @param red		Red color value
 * @param green	Green color value
 * @param blue	Blue color value
 */
Color::Color( float  red, float green, float blue ): 
	m_lowRange( 0.0f ), 
	m_hightRange( 255.0f )
{
	set( red, green, blue, m_hightRange );
}

/**
 * @brief Initializes the color with a specific color values (depending on the color range
 * which by default is 0..255, and RGB color model (red, green, blue).
 *
 * @param red		Red color value
 * @param green	Green color value
 * @param blue	Blue color value
 * @param alpha Alpha value. Represents transparency. Max value means opaque, min value means transparent.
 */
Color::Color( float  red, float green, float blue, float alpha ): 
	m_lowRange( 0.0f ), 
	m_hightRange( 255.0f )
{
	set( red, green, blue, alpha );
}

/**
 * @brief Initializes the color with a specific color values (depending on the color range
 * which by default is 0..255, and RGB color model (red, green, blue).
 *
 * @param gray Gray color. Red, green and blue with the same value
 */
Color::Color( int gray ): 
	m_lowRange( 0.0f ), 
	m_hightRange( 255.0f )
{
	set( (float)gray, (float)gray, (float)gray, m_hightRange );
}

/**
 * @brief Initializes the color with a specific color values (depending on the color range
 * which by default is 0..255, and RGB color model (red, green, blue).
 *
 * @param gray Gray color.	Red, green and blue with the same value
 * @param alpha				Transparency value (0 = transparent, 255 = opaque)
 */
Color::Color( int gray, float alpha ): 
	m_lowRange( 0.0f ), 
	m_hightRange( 255.0f )
{
	set( (float)gray, (float)gray, (float)gray, alpha );
}

/**
 * @brief Initializes the color with a specific color values (depending on the color range
 * which by default is 0..255, and RGB color model (red, green, blue).
 *
 * @param red		Red color value
 * @param green	Green color value
 * @param blue	Blue color value
 * @param alpha Alpha value. Represents transparency. Max value means opaque, min value means transparent.
 */
Color::Color(  Ogre::ColourValue color ): 
	m_lowRange( 0.0f ), 
	m_hightRange( 255.0f )
{
	color.r = map( color.r, 0.0, 1.0, 0, 255);
	color.g = map( color.g, 0.0, 1.0, 0, 255);
	color.b = map( color.b, 0.0, 1.0, 0, 255);
	
	set( color.r, color.g, color.b, color.a );
}
/**
 * @brief Sets the color values of this Color object.
 *
 * @param red		Red color value
 * @param green	Green color value
 * @param blue	Blue color value
 * @param alpha Alpha value. Represents transparency. Max value means opaque, min value means transparent.
 */
void Color::set( float red, float green, float blue, float alpha /*= 255.0f*/ )
{
	// store values
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

/**
 * @internal 
 * @brief Returns the color value normalized. This means, value will range from 0..1
 *
 * @return Normalized (0..1) color value
 */
Color Color::normalized() const
{
	// Create an equal color
	Color normalizedColor = *this;

	// Normalize it
	normalizedColor.normalize();

	return normalizedColor;
}

/**
 * @internal 
 * @brief Normalizes the values of this color to range 0..1
 */
void Color::normalize()
{
	// Convert values to current color range
	r = map( r, m_lowRange, m_hightRange, 0.0f, 1.0f );
	g = map( g, m_lowRange, m_hightRange, 0.0f, 1.0f );
	b = map( b, m_lowRange, m_hightRange, 0.0f, 1.0f );
	a = map( a, m_lowRange, m_hightRange, 0.0f, 1.0f );
}

/**
 * @internal 
 * @brief Change the color mode and range
 */
void Color::colorMode( GraphicsType mode, float range1, float range2, float range3, float range4 )
{
	m_colorMode  = mode;
	//m_lowRange   = 0;
	//m_hightRange = 255;
}

} // namespace Cing