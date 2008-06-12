/*
  This source file is part of the Vision project
  For the latest info, see http://www.XXX.org

  Copyright (c) 2008 XXX

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

#include "ImageResourceManager.h"

// Common includes
#include "common/Exception.h"
#include "common/Release.h"

#include "externLibs/OpenCV/highgui/include/highgui.h"

namespace Graphics
{
  
/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
ImageResourceManager::ImageResourceManager():
  m_bIsValid    ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
ImageResourceManager::~ImageResourceManager()
{
  // Release resources
  end();
}

/**
 * @internal
 * @brief Initializes the class so it becomes valid.
 *
 * @return true if the initialization was ok | false otherwise
 */
bool ImageResourceManager::init()
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

	// The class is now initialized
	m_bIsValid = true;

	// for debug
	for (int i = 0; i < 4 ; i++)
	{
		m_TempImages.push_back( cvCreateImage( cvSize( 640, 480 ), IPL_DEPTH_8U, 3 ) );
	}
	
	return true;
}

/**
 * @brief This method returns an IplImage with the requested size and number of channels 
 *				Creates a new one if requires.
 * @param  int width, int height, int channels
 * @return IplImage
 */
IplImage&	ImageResourceManager::getImage (int width, int height, int channels) const {
	
	// Search for the desired image into the image container	

	// Return the found image.
	return *m_TempImages[0];
};

/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void ImageResourceManager::end()
{
  // Check if the class is already released
  if ( !isValid() )
    return;

	// The class is not valid anymore
	m_bIsValid = false;
}


} // namespace Graphics