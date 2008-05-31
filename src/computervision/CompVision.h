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

#ifndef _ComputerVisionFramework_H_
#define _ComputerVisionFramework_H_

#include "ComputerVisionPrereqs.h"
#include "filters/ImageDifferenceFilter.h"
#include "filters/ImageThresholdFilter.h"
#include "BlobFinder.h"

// OpenCV
#include "externLibs/OpenCV/cxcore/include/cxtypes.h"

// Graphics
#include "graphics/TexturedQuad.h"

namespace ComputerVision
{

/**
 * @brief This class contains the computer vision methods supported by the library
 * such as background subtraction, frame differencing...etc
 */
class CompVision
{
public:

	// Constructor / Destructor
	 CompVision();
	~CompVision();

	// Init / Release / Update
	bool        init              ( int width, int height);
	void        end               ();
	void        update            ( IplImage* imgToAnalyze );

	// Query methods
	bool        isValid           ()       const { return m_bIsValid; }	
  int         getNumBlobs       ()       const { return static_cast< int >( m_blobFinder.getNumBlobs() ); }
  const Blob& getBlobN          (int n ) const { return m_blobFinder.getBlobN( n ); }

  // Computer vision methods
  void        storeBackground    ( const IplImage* backgroundImage );

  // Computer vision parameters control
  void        setBlobDetection        ( bool bDetection   ) { m_blobDetection = bDetection; }
  void        setBackgroundSubtraction( bool bSubtraction ) { m_backgroundSubtraction = bSubtraction; }

  // Debug
  void        drawComputerVisionProcess();

private:

  // Filters / Computer vision algorithms
  BlobFinder              m_blobFinder;                 ///< Finds blobs in a scene and extracts their information
  ImageDifferenceFilter   m_imgDiffFilter;              ///< Filter to calculate the difference between two images
  ImageThresholdFilter    m_imgThresholdFilter;         ///< Image to apply thresholding (posterizing) of an image

	// Computer vision process images
  IplImage*               m_cameraImage;                ///< Current camera image
  IplImage*               m_background;                 ///< Stored background (empty scene). Used for the background subtraction
  IplImage*               m_afterBackgroundSubtraction; ///< Image after background subtraction is performed. Used for the blob detection

  // Debug variables
  Graphics::TexturedQuad  m_cameraTQ;                     ///< Debug variable to allow drawing the camera image
  Graphics::TexturedQuad  m_backgrounTQ;                  ///< Debug variable to allow drawing the background image
  Graphics::TexturedQuad  m_afterBackgroundSubtractionTQ; ///< Debug variable to allow drawing the image result after background subtraction

  // Computer vision process flags
  bool                    m_backgroundSubtraction;      ///< If true background subtraction will be performed every frame
  bool                    m_blobDetection;              ///< If true blob detection will be performed every frame

	bool                    m_bIsValid;	                  ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};


} // namespace CompVision

#endif // _CompVisionFramework_H_
