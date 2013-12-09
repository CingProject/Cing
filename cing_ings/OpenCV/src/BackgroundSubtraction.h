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

#ifndef _BackgroundSubtraction_h_
#define _BackgroundSubtraction_h_

// Graphics
#include "graphics/Image.h"
#include "ImageDifferenceFilter.h"
#include "ImageThresholdFilter.h"

// openCV
#include "opencv2/core/core.hpp"

namespace Cing
{

/**
 * @brief Class to perform background subraction from the images provided (background and current image)
 */
class BackgroundSubtraction
{
public:
	
	// Bg subtraction techniques
	enum BgSubtractionTechnique
	{
		ABS_DIFF,
		BRIGHTNESS
	};


	// Constructor / Destructor
	BackgroundSubtraction();
	~BackgroundSubtraction();

	// Init / Release / Update
	void	end		();	

	// Operations
	void	update					( const Image& imgToAnalyze, Image& output );
	void	storeBackground	( const Image& backgroundImage );

	// Configuration
	void	setThreshold		( int threshold ) { m_thresholdFilter.setThreshold( threshold ); }
	int		getThreshold		() { return m_thresholdFilter.getThreshold(); }
	void	setTechnique		( BgSubtractionTechnique technique ) { m_technique = technique; }

	// Query  Methods
	bool	isValid	() { return m_backgroundImage.isValid(); }

	// Returns current background
	Image& getBackground() { return m_backgroundImage; }

	// Constants
	static const int DEFAULT_THRESHOLD; ///< Default threshold value to perform background subtraction

private:

	// Attributes
	Image						m_backgroundImage;	///< Background.	Image that will be compared with the image received in the compute method

	// Filters used
	ImageDifferenceFilter		m_differenceFilter;
	ImageThresholdFilter		m_thresholdFilter;

	BgSubtractionTechnique		m_technique;
};

} // namespace Cing

#endif // _BackgroundSubtraction_h_