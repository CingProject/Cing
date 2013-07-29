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

#include "graphics/BaseRenderer2D.h"
#include "ImageDifferenceFilter.h"
#include "ImageThresholdFilter.h"

namespace Cing
{

/**
 * @brief Base class for (interface) for 2D renderers. Defines the functionality expected from any 2D renderer
 * Generally this class is mainly relevant for anybody developing new 2D rendering backends (say using Cairo graphics or Skia).
 */
class Renderer2DOpenCV: public BaseRenderer2D
{
public:

	// Constructor / Destructor
	Renderer2DOpenCV(): BaseRenderer2D() { m_isValid = true; };
	virtual ~Renderer2DOpenCV()				{ end(); }

	// Init / Release / Update
	bool	init	()	override	{}
	void    end     ()	override	{ BaseRenderer2D::end(); }
	void	update  ()	override	{}

	// Manipulation

	/// Filters an image based on the possibilities defined in ImageProcessingFilters
	void	filter ( Image& image, ImageProcessingFilters kind, float param = FLT_MAX ) override;

	// Query methods

private:

	// Filters to be used internally
	ImageDifferenceFilter	m_imgDiffFilter;
	ImageThresholdFilter	m_imgThresholdFilter;
};

} // namespace Cing

