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

#include "ImageResourceManager.h"

// Common includes
#include "common/CommonUtilsIncludes.h"

// Open CV
#include "opencv2/core/core.hpp"

namespace Cing
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
 * @brief In this class init just creates a cache to speed up the process for the most comon kind of image sizes.
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

	// Create initial pool of images with typical video camera resolutions (160x120, 320x240...)
	int minResX = 160;
	int	minResY = 120;
	int maxResX = 640;
	int	maxResY = 480;
	for ( int xRes = minResX, yRes = minResY; (xRes <= maxResX) && (yRes <= maxResY) ;  xRes *= 2, yRes *= 2)
	{
		// For 1 and 3 channel images
		m_imagePool[0].push_back( ImageResource( new cv::Mat( yRes, xRes, CV_MAKETYPE(CV_8U, 1) ) ) );
		m_imagePool[1].push_back( ImageResource( new cv::Mat( yRes, xRes, CV_MAKETYPE(CV_8U, 3) ) ) );
	}

	// Create initial pool of images with standard sizes
	int minRes = 64;
	int	maxRes = 512;
	for ( int xRes = minRes; xRes <= maxRes ; xRes *= 2)
		for ( int yRes = minRes; yRes <= maxRes ; yRes *= 2)
	{
		// For 1 and 3 channel images
		m_imagePool[0].push_back( ImageResource( new cv::Mat( yRes, xRes, CV_MAKETYPE(CV_8U, 1) ) ) );
		m_imagePool[1].push_back( ImageResource( new cv::Mat( yRes, xRes, CV_MAKETYPE(CV_8U, 3) ) ) );
	}

	return true;
}

/**
 * @brief This method returns a cv::Mat with the requested size and number of channels 
 *				Creates a new one if required (and store it in the pool).
 *
 * @note Important: The images returned have the purpose to be used as temporal working images
 * but they should not be stored nor deleted. When the work with them is finished <b>releaseImage should be called</b>
*
 * @param  width		Width of the requested image
 * @param  height		Height of the requested image
 * @param  channels Number of channels of the requested image. Supported 1 and 3
 * @return cv::Mat pointer to the image requested, or NULL if number of channels not supported
 */
cv::Mat*	ImageResourceManager::getImage( int width, int height, int channels ) 
{
	// Check ok number of channels
	if ( ( channels != 1) && ( channels != 3 ) && ( channels != 4 ) )
	{
		LOG_ERROR( "Requesting an image with a non supported number of channels. Supported 1 (Grayscale), 3 (RGB) or 4 (RGBA)" );
		return NULL;
	}

	// Search for the desired image into the image container	
	int index = 0;
	if ( channels == 1 )		index = 0;
	else if ( channels == 3 )	index = 1;
	else if ( channels == 4 )	index = 2;

	// Search the image
	for ( size_t i = 0; i < m_imagePool[index].size(); ++i )
	{
		// If found and available -> return it
		if ( (m_imagePool[index][i].image->cols== width) &&  (m_imagePool[index][i].image->rows == height) &&
					m_imagePool[index][i].available )
		{
			// mark is as used
			m_imagePool[index][i].available = false;
			return m_imagePool[index][i].image;
		}
	}

	// If not found -> create it and insert it in the pool
	m_imagePool[index].push_back( ImageResource( new cv::Mat( height, width, CV_MAKETYPE(CV_8U, channels) ) ) );
	m_imagePool[index].back().available = false;
	return m_imagePool[index].back().image;
};

/**
 * @brief Releases an image that has been requested to the manager before. This should be called after the
 * image is not needed anymore
 *
 * @param image Image to release so another one can use it
 */
void ImageResourceManager::releaseImage( cv::Mat* image )
{
	// Search for the desired image into the image container	
	int index = 0;
	if ( image->channels() == 1 )		index = 0;
	else if ( image->channels() == 3 )	index = 1;
	else if ( image->channels() == 4 )	index = 2;
	else return;

	// Search the image
	for ( size_t i = 0; i < m_imagePool[index].size(); ++i )
	{
		// If found and available -> mark it as available
		if ( m_imagePool[index][i].image == image )
		{
			m_imagePool[index][i].available = true;
			return;
		}
	}

}
/**
 * @internal
 * @brief Releases the class resources. 
 * After this method is called the class is not valid anymore.
 */
void ImageResourceManager::end()
{
	// Release all images

	// 1 channel images
	for ( size_t i = 0; i < m_imagePool[0].size(); ++i )
		delete m_imagePool[0][i].image;

	// 3 channel images
	for ( size_t i = 0; i < m_imagePool[1].size(); ++i )
		delete m_imagePool[1][i].image;

	// 4 channel images
	for ( size_t i = 0; i < m_imagePool[2].size(); ++i )
		delete m_imagePool[2][i].image;


	m_imagePool[0].clear();
	m_imagePool[1].clear();
	m_imagePool[2].clear();
		
	// The class is not valid anymore
	m_bIsValid = false;
}


} // namespace Cing