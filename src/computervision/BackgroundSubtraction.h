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

#ifndef _BackgroundSubtraction_h_
#define _BackgroundSubtraction_h_

#include "ComputerVisionPrereqs.h"

// Graphics
#include "graphics/Image.h"
#include "graphics/ImageDifferenceFilter.h"
#include "graphics/ImageThresholdFilter.h"

namespace ComputerVision
{

/**
 * @brief Class to perform background subraction from the images provided (background and current image)
 */
class BackgroundSubtraction
{
public:

	// Constructor / Destructor
	BackgroundSubtraction();
	~BackgroundSubtraction();

	// Init / Release / Update
	void	end		();	

	// Operations
	void	update					( const Graphics::Image& imgToAnalyze, Graphics::Image& output );
	void	storeBackground	( const Graphics::Image& backgroundImage );

	// Configuration
	void  setThreshold		( int threshold ) { m_thresholdFilter.setThreshold( threshold ); }

	// Query  Methods
	bool	isValid	() { return m_backgroundImage != NULL; }

	// Constants
	static const int DEFAULT_THRESHOLD; ///< Default threshold value to perform background subtraction

private:

	// Attributes
	_IplImage*	m_backgroundImage;	///< Background.	Image that will be compared with the image received in the compute method

	// Filters used
	Graphics::ImageDifferenceFilter		m_differenceFilter;
	Graphics::ImageThresholdFilter		m_thresholdFilter;
};

} // namespace ComputerVision

#endif // _BackgroundSubtraction_h_