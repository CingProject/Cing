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

#pragma once

// Precompiled headers
#include "Cing-Precompiled.h"

#include "GraphicsPrereqs.h"
#include "common/LogManager.h"

namespace Cing
{


#define LOG_UNDEFINED_METHOD LOG_ERROR( (std::string(__FUNCTION__) + std::string( " - ERROR: Implementation not provided by 2D Rendering Backend" )).c_str() )

/**
 * @brief Base class for (interface) for 2D renderers. Defines the functionality expected from any 2D renderer
 * Generally this class is mainly relevant for anybody developing new 2D rendering backends (say using Cairo graphics or Skia).
 */
class BaseRenderer2D
{
public:

	// Constructor / Destructor
	BaseRenderer2D(): m_isValid(false) {};
	virtual ~BaseRenderer2D() { end(); }

	// Init / Release / Update
	virtual bool	init	()			{ return true; }
	virtual void    end     ()			{ m_isValid = false; }
	virtual void	update  ()			{}

	// Manipulation

	/// Filters an image based on the possibilities defined in ImageProcessingFilters
	virtual void	filter	( Image& image, ImageProcessingFilters kind, float param = FLT_MAX )			{ LOG_UNDEFINED_METHOD; }
	virtual void	triangle( Image& image, int x1, int y1, int x2, int y2, int x3, int y3 )				{ LOG_UNDEFINED_METHOD; }
	virtual void  	line	( Image& image, int x1, int y1, int x2, int y2 )								{ LOG_UNDEFINED_METHOD; }
	virtual void  	arc		( Image& image, int x, int y,  int width, int height, float start, float stop )	{ LOG_UNDEFINED_METHOD; }
	virtual void  	point	( Image& image, int x, int y)													{ LOG_UNDEFINED_METHOD; }
	virtual void  	quad	( Image& image, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 ){ LOG_UNDEFINED_METHOD; }
	virtual void  	ellipse	( Image& image, int x, int y, int width, int height, float angleDegrees = 0 )	{ LOG_UNDEFINED_METHOD; }
	virtual void  	rect	( Image& image, int x, int y, int width, int height )							{ LOG_UNDEFINED_METHOD; }
	virtual void  	text	( Image& image, int x1, int y1, const std::string& text )						{ LOG_UNDEFINED_METHOD; }
	virtual void  	fill    ( Image& image, const Color& color )											{ LOG_UNDEFINED_METHOD; }

	// Query methods
	bool			isValid	() const	{ return m_isValid; } 

protected:

	bool	m_isValid;	///< Indicates whether the class is valid or not. If not valid (for example because it has not been initialized), this class's functionality won't likely work
};

} // namespace Cing

