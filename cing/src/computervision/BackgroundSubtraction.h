/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

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
	void	setTechnique		( BgSubtractionTechnique technique ) { m_technique = technique; }

	// Query  Methods
	bool	isValid	() { return m_backgroundImage != NULL; }

	// Returns current background
	_IplImage* getBackground() { return m_backgroundImage; }

	// Constants
	static const int DEFAULT_THRESHOLD; ///< Default threshold value to perform background subtraction

private:

	// Attributes
	_IplImage*	m_backgroundImage;	///< Background.	Image that will be compared with the image received in the compute method

	// Filters used
	ImageDifferenceFilter		m_differenceFilter;
	ImageThresholdFilter		m_thresholdFilter;

	BgSubtractionTechnique		m_technique;
};

} // namespace Cing

#endif // _BackgroundSubtraction_h_