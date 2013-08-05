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

#include "Renderer2DOpenCV.h"
#include "OpenCVUtils.h"
#include "ImageResourceManager.h"

// opencv
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

// cing
#include "graphics/Image.h"
#include "graphics/GraphicsManager.h"
#include "common/CommonConstants.h"

namespace Cing
{
	/**
	 *	Filters an image as defined by the received parameters. Availabe filters
	 *	BLUR, parameter: 
	 *
	 * @param image The image that will be filtered.
	 * @param kind	The type of filter to apply
	 * @param param	An optional parameter related to the type of filter to be applied. Some filters require parameters, others don't.
	 */ 
	void Renderer2DOpenCV::filter ( Image& image, ImageProcessingFilters kind, float param /*= FLT_MAX*/ )
	{
		// Check if the renderer is valid
		if ( !isValid() )
		{
			LOG_ERROR( "Renderer2DOpenCV has not been correctly initialized. Did you call the init() method?" );
			return;
		}

		// Check the image is valid
		if ( !image.isValid() )
		{
			LOG_ERROR( "Trying to filter in an invalid image! It should be initialized before by loading an image from disk, or just calling its init method." );
			return;
		}

		int intParam = (int)param;
		switch ( kind )
		{
			case BLUR:	
				if ( param == FLT_MAX )
					intParam = 3;
				cv::blur(toCVMat(image), toCVMat(image), cv::Size(intParam, intParam) );
			break;

			case ERODE:	
				if ( param == FLT_MAX )
					intParam = 1;
				cv::erode( toCVMat(image), toCVMat(image), cv::Mat(), cv::Point(-1, -1), intParam );
			break;

			case DILATE:	
				if ( param == FLT_MAX )
					intParam = 1;
				cv::dilate( toCVMat(image), toCVMat(image), cv::Mat(), cv::Point(-1, -1), intParam );
			break;

			case THRESHOLD:	
				if ( param != FLT_MAX )
					m_imgThresholdFilter.setThreshold(intParam);
				m_imgThresholdFilter.apply(  toCVMat(image), toCVMat(image) );
			break;

			case INVERT:	
				cv::invert( toCVMat(image), toCVMat(image) );
			break;

			case FLIP_X:	
				cv::flip( toCVMat(image), toCVMat(image), 0 );
			break;

			case FLIP_Y:	
				cv::flip( toCVMat(image), toCVMat(image), 1 );
			break;	

			case FLIP_XY:	
				cv::flip( toCVMat(image), toCVMat(image), -1 );
			break;

		};
	}

	/**
	 * @brief Draws a point ( one pixel ) inside an image
	 *
	 * @param Image image to draw in (canvas)
	 * @param x x, point
	 * @param y y, point
	 */
	void Renderer2DOpenCV::point( Image& image, int x, int y )
	{
		// Check if the renderer is valid
		if ( !isValid() )
		{
			LOG_ERROR( "Renderer2DOpenCV has not been correctly initialized. Did you call the init() method?" );
			return;
		}

		// Check the image is valid
		if ( !image.isValid() )
		{
			LOG_ERROR( "Trying to draw in an invalid image! It should be initialized before by loading an image from disk, or just calling its init method." );
			return;
		}

		// Get Stroke and Fill Color
		GraphicsManager& graphManager	= GraphicsManager::getSingleton();
		Color strokeColor				= graphManager.getStrokeColor();
		int   strokeWeight				= graphManager.getStrokeWeight();

		// Create a cv::Mat header around the image contents
		cv::Mat imageMat = toCVMat( image );

		// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
		// as opencv does not support transparent drawing
		cv::Mat* canvasImage = &imageMat;
		cv::Mat* alphaCanvasImage = NULL;

		// If there is transparency involved
		if ( strokeColor.a < 255 )
		{
			// Request a temporary image to draw the transparent shape
			alphaCanvasImage = ImageResourceManager::getSingleton().getImage( image.getWidth(), image.getHeight(), image.getNChannels() );
		
			// Fill it with the current image content
			imageMat.copyTo( *alphaCanvasImage );

			// The canvas will be the temp image to draw on it and later on blend it with the current cv image
			canvasImage = alphaCanvasImage;
		}

		// Draw a pixel
		cv::rectangle(  *canvasImage,
						cv::Point(x,y),
						cv::Point(x,y),
						cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ),
						strokeWeight);		///-> Thickness.


		// If there is transparency involved -> blend the result
		if ( strokeColor.a < 255 )
		{
			// Blend the result
			cv::addWeighted( imageMat, (strokeColor.getHighRange()-strokeColor.a)/strokeColor.getHighRange(), *canvasImage, strokeColor.a/strokeColor.getHighRange(), 0, imageMat );

			// Release temp image
			ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
		}
	}

	/**
	 * @brief Draws a line inside an image
	 *
	 * @param Image image to draw in (canvas)
	 * @param x1 x, first point
	 * @param y1 y, first point
	 * @param x2 x, end point
	 * @param y2 y, end point
	 */
	void Renderer2DOpenCV::line( Image& image, int x1, int y1, int x2, int y2 )
	{
		// Check if the renderer is valid
		if ( !isValid() )
		{
			LOG_ERROR( "Renderer2DOpenCV has not been correctly initialized. Did you call the init() method?" );
			return;
		}

		// Check the image is valid
		if ( !image.isValid() )
		{
			LOG_ERROR( "Trying to draw in an invalid image! It should be initialized before by loading an image from disk, or just calling its init method." );
			return;
		}

		// Get Stroke and Fill Color
		GraphicsManager& graphManager	= GraphicsManager::getSingleton();
		Color color						= graphManager.getStrokeColor();
		int   strokeWeight				= graphManager.getStrokeWeight();

		// Create a cv::Mat header around the image contents
		cv::Mat imageMat = toCVMat( image );

		// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
		// as opencv does not support transparent drawing
		cv::Mat* canvasImage = &imageMat;
		cv::Mat* alphaCanvasImage = NULL;

		// If there is transparency involved
		if ( color.a < 255 )
		{
			// Request a temporary image to draw the transparent shape
			alphaCanvasImage = ImageResourceManager::getSingleton().getImage( image.getWidth(), image.getHeight(), image.getNChannels() );
		
			// Fill it with the current image content
			imageMat.copyTo( *alphaCanvasImage );

			// The canvas will be the temp image to draw on it and later on blend it with the current cv image
			canvasImage = alphaCanvasImage;

		}

		// Draw 
		if (graphManager.getSmooth())
		{
			cv::line(	*canvasImage,
						cv::Point(x1,y1),
						cv::Point(x2,y2),
						cv::Scalar( color.r, color.g, color.b ),
						strokeWeight,	///< Thickness.
						CV_AA,			///< Type of the ellipse boundary, see cvLine description.
						0);				///< Number of fractional bits in the center coordinates and axes' values.
		}else{
			cv::line(	*canvasImage,
						cv::Point(x1,y1),
						cv::Point(x2,y2),
						cv::Scalar( color.r, color.g, color.b ),
						strokeWeight,	///< Thickness.
						4,				///< Type of the ellipse boundary, see cvLine description.
						0); 			///< Number of fractional bits in the center coordinates and axes' values.
		}

		// If there is transparency involved -> blend the result
		if ( color.a < 255 )
		{
			// Blend the result
			cv::addWeighted( imageMat, (color.getHighRange()-color.a)/color.getHighRange(), *canvasImage, color.a/color.getHighRange(), 0, imageMat );

			// Release temp image
			ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
		}
	}

	/**
	 * @brief Draws a triangle inside an image
	 *
	 * @param Image image to draw in (canvas)
	 * @param x1 x, first point
	 * @param y1 y, first point
	 * @param x2 x, second point
	 * @param y2 y, second point
	 * @param x3 x, third point
	 * @param y3 y, third point
	 */
	void Renderer2DOpenCV::triangle( Image& image, int x1, int y1, int x2, int y2, int x3, int y3 )
	{
		// Check if the renderer is valid
		if ( !isValid() )
		{
			LOG_ERROR( "Renderer2DOpenCV has not been correctly initialized. Did you call the init() method?" );
			return;
		}

		// Check the image is valid
		if ( !image.isValid() )
		{
			LOG_ERROR( "Trying to filter in an invalid image! It should be initialized before by loading an image from disk, or just calling its init method." );
			return;
		}

		// Get Stroke and Fill Color
		GraphicsManager& graphManager	= GraphicsManager::getSingleton();
		Color strokeColor				= graphManager.getStrokeColor();
		Color fillColor					= graphManager.getFillColor();
		int   strokeWeight				= graphManager.getStrokeWeight();

		// Create a cv::Mat header around the image contents
		cv::Mat imageMat = toCVMat( image );

		// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
		// as opencv does not support transparent drawing
		cv::Mat* canvasImage = &imageMat;
		cv::Mat* alphaCanvasImage = NULL;

		// If there is transparency involved
		if ( (graphManager.getStroke() && (strokeColor.a < 255)) || ( graphManager.getFill() && (fillColor.a < 255) ) )
		{
			// Request a temporary image to draw the transparent shape
			alphaCanvasImage = ImageResourceManager::getSingleton().getImage( image.getWidth(), image.getHeight(), image.getNChannels() );
		
			// Fill it with the current image content
			imageMat.copyTo( *alphaCanvasImage );

			// The canvas will be the temp image to draw on it and later on blend it with the current cv image
			canvasImage = alphaCanvasImage;

		}

		// Draw Fill
		if (graphManager.getFill())
		{
			// Get Fill Color
			Color color        = graphManager.getFillColor();
			cv::Point pts[3] = { cv::Point(x1,y1), cv::Point(x2,y2), cv::Point(x3,y3)};

			if (graphManager.getSmooth())
				cv::fillConvexPoly( *canvasImage, (const cv::Point*)&pts, 3, cv::Scalar( color.r, color.g, color.b ), CV_AA, 0 );
			else
				cv::fillConvexPoly( *canvasImage, (const cv::Point*)&pts, 3, cv::Scalar( color.r, color.g, color.b ), 4, 0 );
		}

		// Draw stroke
		if (graphManager.getStroke())
		{
			if (graphManager.getSmooth())
			{
				cv::line( *canvasImage, cv::Point(x1,y1), cv::Point(x2,y2), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
				cv::line( *canvasImage, cv::Point(x2,y2), cv::Point(x3,y3), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
				cv::line( *canvasImage, cv::Point(x3,y3), cv::Point(x1,y1), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
			}else{																														 
				cv::line( *canvasImage, cv::Point(x1,y1), cv::Point(x2,y2), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
				cv::line( *canvasImage, cv::Point(x2,y2), cv::Point(x3,y3), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
				cv::line( *canvasImage, cv::Point(x3,y3), cv::Point(x1,y1), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
			}																														 
		}																															  	

		// If there is transparency involved -> blend the result and release the temp image used for transparency
		if ( graphManager.getFill() && (fillColor.a < 255) )
		{
			cv::addWeighted( imageMat, (fillColor.getHighRange()-fillColor.a)/fillColor.getHighRange(), *canvasImage, fillColor.a/fillColor.getHighRange(), 0, imageMat );
			ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );

			// Note: Now theres is a limitation (opencv does not support transparency in the drawing API)
			// So we limit it to only 1 kind (fill or stroke), as we fake it blending the whole drawing with the current image, which is slow.
			// This warning is to nofity it
			if ( graphManager.getStroke() && (strokeColor.a < 255) )
				LOG_ERROR_NTIMES( 3, "Trying to draw with transparency in both fill and stroke is not supported. Only fill transparency will be applied" );

		}
		else if ( graphManager.getStroke() && (strokeColor.a < 255) )
		{
			cv::addWeighted( imageMat, (strokeColor.getHighRange()-strokeColor.a)/strokeColor.getHighRange(), *canvasImage, strokeColor.a/strokeColor.getHighRange(), 0, imageMat );
			ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
		}

	}

	/**
	 * @brief Draws a rectangle inside an image
	 *
	 * @param Image image to draw in (canvas)
	 * @param x x,		top-left cornder x coordinate
	 * @param y y,		top-left cornder y coordinate
	 * @param width		width
	 * @param height	height
	 */
	void Renderer2DOpenCV::rect( Image& image, int x, int y, int width, int height )
	{
		// Check if the renderer is valid
		if ( !isValid() )
		{
			LOG_ERROR( "Renderer2DOpenCV has not been correctly initialized. Did you call the init() method?" );
			return;
		}

		// Check the image is valid
		if ( !image.isValid() )
		{
			LOG_ERROR( "Trying to draw in an invalid image! It should be initialized before by loading an image from disk, or just calling its init method." );
			return;
		}

		// Calcuate half sizes
		float widthDIV2 = (float)width/2.0f;
		float heightDIV2 = (float)height/2.0f;

		// Get Stroke and Fill Color
		GraphicsManager& graphManager	= GraphicsManager::getSingleton();
		Color strokeColor				= graphManager.getStrokeColor();
		Color fillColor					= graphManager.getFillColor();
		int   strokeWeight				= graphManager.getStrokeWeight();

		// Create a cv::Mat header around the image contents
		cv::Mat imageMat = toCVMat( image );

		// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
		// as opencv does not support transparent drawing
		cv::Mat* canvasImage = &imageMat;
		cv::Mat* alphaCanvasImage = NULL;

		// If there is transparency involved
		if ( (graphManager.getStroke() && (strokeColor.a < 255)) || ( graphManager.getFill() && (fillColor.a < 255) ) )
		{
			// Request a temporary image to draw the transparent shape
			alphaCanvasImage = ImageResourceManager::getSingleton().getImage( image.getWidth(), image.getHeight(), image.getNChannels() );
		
			// Fill it with the current image content
			imageMat.copyTo( *alphaCanvasImage );

			// The canvas will be the temp image to draw on it and later on blend it with the current cv image
			canvasImage = alphaCanvasImage;
		}

		// Draw fill
		if (graphManager.getFill())
		{
			switch( graphManager.getRectMode() )
			{

			case CORNER:
				if (graphManager.getSmooth())
					cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(x+width,y+height), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
				else
					cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(x+width,y+height), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
				break;

			case CORNERS:
				if (graphManager.getSmooth())
					cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(width,height), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
				else
					cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(width,height), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
				break;

			case CENTER:
				if (graphManager.getSmooth())
					cv::rectangle( *canvasImage, cv::Point(x-(int)widthDIV2,y-(int)heightDIV2), cv::Point(x+(int)widthDIV2,y+(int)heightDIV2), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
				else
					cv::rectangle( *canvasImage, cv::Point(x-(int)widthDIV2,y-(int)heightDIV2), cv::Point(x+(int)widthDIV2,y+(int)heightDIV2), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
				break;

			case RADIUS:
				if (graphManager.getSmooth())
					cv::rectangle( *canvasImage, cv::Point(x-width,y-height), cv::Point(x+width,y+height), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
				else
					cv::rectangle( *canvasImage, cv::Point(x-width,y-height), cv::Point(x+width,y+height), cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
				break;

			default: 
				LOG_ERROR( "Renderer2DOpenCV::rect. Rect mode not supported by this 2D renderer" );

			}
		}

		// Draw Stroke
		if (graphManager.getStroke())
		{
			switch( graphManager.getRectMode() )
			{

			case CORNER:
				if (graphManager.getSmooth())
					cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(x+width,y+height), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
				else
					cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(x+width,y+height), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
				break;

			case CORNERS:
				if (graphManager.getSmooth())
					cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(width,height), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
				else
					cv::rectangle( *canvasImage, cv::Point(x,y), cv::Point(width,height), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
				break;

			case CENTER:
				if (graphManager.getSmooth())
					cv::rectangle( *canvasImage, cv::Point(x-(int)widthDIV2,y-(int)heightDIV2), cv::Point(x+(int)widthDIV2,y+(int)heightDIV2), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
				else
					cv::rectangle( *canvasImage, cv::Point(x-(int)widthDIV2,y-(int)heightDIV2), cv::Point(x+(int)widthDIV2,y+(int)heightDIV2), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
				break;

			case RADIUS:
				if (graphManager.getSmooth())
					cv::rectangle( *canvasImage, cv::Point(x-width,y-height), cv::Point(x+width,y+height), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
				else
					cv::rectangle( *canvasImage, cv::Point(x-width,y-height), cv::Point(x+width,y+height), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
				break;
			}
		}

		// If there is transparency involved -> blend the result and release the temp image used for transparency
		if ( graphManager.getFill() && (fillColor.a < 255) )
		{
			cv::addWeighted( imageMat, (fillColor.getHighRange()-fillColor.a)/fillColor.getHighRange(), *canvasImage, fillColor.a/fillColor.getHighRange(), 0, imageMat );
			ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );

			// Note: Now theres is a limitation (opencv does not support transparency in the drawing API)
			// So we limit it to only 1 kind (fill or stroke), as we fake it blending the whole drawing with the current image, which is slow.
			// This warning is to nofity it
			if ( graphManager.getStroke() && (strokeColor.a < 255) )
				LOG_ERROR_NTIMES( 3, "Trying to draw with transparency in both fill and stroke is not supported. Only fill transparency will be applied" );

		}
		else if ( graphManager.getStroke() && (strokeColor.a < 255) )
		{
			cv::addWeighted( imageMat, (strokeColor.getHighRange()-strokeColor.a)/strokeColor.getHighRange(), *canvasImage, strokeColor.a/strokeColor.getHighRange(), 0, imageMat );
			ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
		}
	}

	/**
	 * @brief Draws a quad, defined by four points
	 *
	 * @param x1 x, first point (top-left)
	 * @param y1 y, first point (top-left)
	 * @param x2 x, second point (top-right)
	 * @param y2 y, second point (top-right)
	 * @param x3 x, third point (bottom-right)
	 * @param y3 y, third point (bottom-right)
	 * @param x4 x, fourth point (bottom-left)
	 * @param y4 y, fourth point (bottom-left)
	 */
	void Renderer2DOpenCV::quad( Image& image, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 )
	{
		// Check if the renderer is valid
		if ( !isValid() )
		{
			LOG_ERROR( "Renderer2DOpenCV has not been correctly initialized. Did you call the init() method?" );
			return;
		}

		// Check the image is valid
		if ( !image.isValid() )
		{
			LOG_ERROR( "Trying to draw in an invalid image! It should be initialized before by loading an image from disk, or just calling its init method." );
			return;
		}

		// Get Stroke and Fill Color
		GraphicsManager& graphManager	= GraphicsManager::getSingleton();
		Color strokeColor				= graphManager.getStrokeColor();
		Color fillColor					= graphManager.getFillColor();
		int   strokeWeight				= graphManager.getStrokeWeight();

		// Create a cv::Mat header around the image contents
		cv::Mat imageMat = toCVMat( image );

		// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
		// as opencv does not support transparent drawing
		cv::Mat* canvasImage = &imageMat;
		cv::Mat* alphaCanvasImage = NULL;

		// If there is transparency involved
		if ( (graphManager.getStroke() && (strokeColor.a < 255)) || ( graphManager.getFill() && (fillColor.a < 255) ) )
		{
			// Request a temporary image to draw the transparent shape
			alphaCanvasImage = ImageResourceManager::getSingleton().getImage( image.getWidth(), image.getHeight(), image.getNChannels() );
		
			// Fill it with the current image content
			imageMat.copyTo( *alphaCanvasImage );

			// The canvas will be the temp image to draw on it and later on blend it with the current cv image
			canvasImage = alphaCanvasImage;
		}

		// Draw fill
		if (graphManager.getFill())
		{
			cv::Point pts[4]= { cv::Point(x1,y1), cv::Point(x2,y2), cv::Point(x3,y3), cv::Point(x4,y4) };

			if (graphManager.getSmooth())
				cv::fillConvexPoly( *canvasImage , (const cv::Point*)&pts, 4,	cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), CV_AA, 0 );
			else
				cv::fillConvexPoly( *canvasImage , (const cv::Point*)&pts, 4,	cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), 4, 0 );
		}

		// Draw stroke
		if (graphManager.getStroke())
		{
			if (graphManager.getSmooth())
			{
				cv::line( *canvasImage, cv::Point(x1,y1), cv::Point(x2,y2), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
				cv::line( *canvasImage, cv::Point(x2,y2), cv::Point(x3,y3), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
				cv::line( *canvasImage, cv::Point(x3,y3), cv::Point(x4,y4), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
				cv::line( *canvasImage, cv::Point(x4,y4), cv::Point(x1,y1), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA, 0);
																																		 
			}else{																														 
																																		 
				cv::line( *canvasImage, cv::Point(x1,y1), cv::Point(x2,y2), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
				cv::line( *canvasImage, cv::Point(x2,y2), cv::Point(x3,y3), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
				cv::line( *canvasImage, cv::Point(x3,y3), cv::Point(x4,y4), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
				cv::line( *canvasImage, cv::Point(x4,y4), cv::Point(x1,y1), cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4, 0);  
			}																														 
		}																															 
			
		// If there is transparency involved -> blend the result and release the temp image used for transparency
		if ( graphManager.getFill() && (fillColor.a < 255) )
		{
			cv::addWeighted( imageMat, (fillColor.getHighRange()-fillColor.a)/fillColor.getHighRange(), *canvasImage, fillColor.a/fillColor.getHighRange(), 0, imageMat );
			ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );

			// Note: Now theres is a limitation (opencv does not support transparency in the drawing API)
			// So we limit it to only 1 kind (fill or stroke), as we fake it blending the whole drawing with the current image, which is slow.
			// This warning is to nofity it
			if ( graphManager.getStroke() && (strokeColor.a < 255) )
				LOG_ERROR_NTIMES( 3, "Trying to draw with transparency in both fill and stroke is not supported. Only fill transparency will be applied" );

		}
		else if ( graphManager.getStroke() && (strokeColor.a < 255) )
		{
			cv::addWeighted( imageMat, (strokeColor.getHighRange()-strokeColor.a)/strokeColor.getHighRange(), *canvasImage, strokeColor.a/strokeColor.getHighRange(), 0, imageMat );
			ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
		}
	}

	/**
	 * @brief Draws an arc inside an image. Arcs are drawn along the outer edge of an ellipse defined by the x, y,
	 *			width and height parameters. The start and stop parameters specify the angles at which to draw the arc.
	 *
	 * @param Image image to draw in (canvas)
	 * @param x x, first point
	 * @param y y, first point
	 * @param width  width
	 * @param height height
	 * @param start start angle (in radians)
	 * @param end end angle of the arc(in radians)
	 */
	void Renderer2DOpenCV::arc( Image& image, int x, int y, int width, int height, float start, float end )
	{
		// Check if the renderer is valid
		if ( !isValid() )
		{
			LOG_ERROR( "Renderer2DOpenCV has not been correctly initialized. Did you call the init() method?" );
			return;
		}

		// Check the image is valid
		if ( !image.isValid() )
		{
			LOG_ERROR( "Trying to draw in an invalid image! It should be initialized before by loading an image from disk, or just calling its init method." );
			return;
		}

		// Create a cv::Mat header around the image contents
		cv::Mat imageMat = toCVMat( image );

		// Convert from Radians to degrees(to keep compatibility with processing)
		// TODO: should keep the 360 flip only in Processing Mode?
		start	= 360 - degrees(start);
		end		= 360 - degrees(end);

		// Get Stroke and Fill Color
		GraphicsManager& graphManager	= GraphicsManager::getSingleton();
		Color strokeColor				= graphManager.getStrokeColor();
		Color fillColor					= graphManager.getFillColor();
		int   strokeWeight				= graphManager.getStrokeWeight();

		// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
		// as opencv does not support transparent drawing
		cv::Mat* canvasImage = &imageMat;
		cv::Mat* alphaCanvasImage = NULL;

		// If there is transparency involved
		if ( (graphManager.getStroke() && (strokeColor.a < 255)) || ( graphManager.getFill() && (fillColor.a < 255) ) )
		{
			// Request a temporary image to draw the transparent shape
			alphaCanvasImage = ImageResourceManager::getSingleton().getImage( image.getWidth(), image.getHeight(), image.getNChannels() );
		
			// Fill it with the current image content
			imageMat.copyTo( *alphaCanvasImage );

			// The canvas will be the temp image to draw on it and later on blend it with the current cv image
			canvasImage = alphaCanvasImage;

		}

		if (graphManager.getFill())
		{
			if (graphManager.getSmooth())
			{
				cv::ellipse(	*canvasImage,					///-> Image.
								cv::Point(x,y),					///-> Center of the ellipse.
								cvSize(width/2,height/2),	///-> Length of the ellipse axes.
								0,								///->	Rotation angle.
								start,							///-> Starting angle of the elliptic arc.
								end,							///-> Ending angle of the elliptic arc.
								cv::Scalar(fillColor.r, fillColor.g, fillColor.b),///-> Ellipse color.
								-1,
								CV_AA);							///-> Thickness of the ellipse arc.
			}else{
				cv::ellipse(	*canvasImage,					///-> Image.
								cv::Point(x,y),					///-> Center of the ellipse.
								cvSize(width/2,height/2),	///-> Length of the ellipse axes.
								0,								///->	Rotation angle.
								start,							///-> Starting angle of the elliptic arc.
								end,							///-> Ending angle of the elliptic arc.
								cv::Scalar(fillColor.r, fillColor.g, fillColor.b),///-> Ellipse color.
								-1,
								4);								///-> Thickness of the ellipse arc.
			}
		}

		if (graphManager.getStroke())
		{			
			if (graphManager.getSmooth())
			{
				cv::ellipse(	*canvasImage,					///-> Image.
								cv::Point(x,y),					///-> Center of the ellipse.
								cvSize(width/2,height/2),///-> Length of the ellipse axes.
								0,								///->	Rotation angle.
								start,							///-> Starting angle of the elliptic arc.
								end,							///-> Ending angle of the elliptic arc.
								cv::Scalar(strokeColor.r, strokeColor.g, strokeColor.b ),///-> Ellipse color.
								strokeWeight,
								CV_AA );								///-> Thickness of the ellipse arc.
			}else{
				cv::ellipse(	*canvasImage,					///-> Image.
								cv::Point(x,y),					///-> Center of the ellipse.
								cvSize(width/2,height/2),	///-> Length of the ellipse axes.
								0,								///->	Rotation angle.
								start,							///-> Starting angle of the elliptic arc.
								end,							///-> Ending angle of the elliptic arc.
								cv::Scalar(strokeColor.r, strokeColor.g, strokeColor.b ),///-> Ellipse color.
								strokeWeight,
								4 );							///-> Thickness of the ellipse arc.

			}
		}

		// If there is transparency involved -> blend the result and release the temp image used for transparency
		if ( graphManager.getFill() && (fillColor.a < 255) )
		{
			cv::addWeighted( imageMat, (fillColor.getHighRange()-fillColor.a)/fillColor.getHighRange(), *canvasImage, fillColor.a/fillColor.getHighRange(), 0, imageMat );
			ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );

			// Note: Now theres is a limitation (opencv does not support transparency in the drawing API)
			// So we limit it to only 1 kind (fill or stroke), as we fake it blending the whole drawing with the current image, which is slow.
			// This warning is to nofity it
			if ( graphManager.getStroke() && (strokeColor.a < 255) )
				LOG_ERROR_NTIMES( 3, "Trying to draw with transparency in both fill and stroke is not supported. Only fill transparency will be applied" );

		}
		else if ( graphManager.getStroke() && (strokeColor.a < 255) )
		{
			cv::addWeighted( imageMat, (strokeColor.getHighRange()-strokeColor.a)/strokeColor.getHighRange(), *canvasImage, strokeColor.a/strokeColor.getHighRange(), 0, imageMat );
			ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
		}
	}


	/**
	 * @brief Draws a ellipse inside an image
	 *
	 * @param Image image to draw in (canvas)
	 * @param x x, first point
	 * @param y y, first point
	 * @param width  width
	 * @param height height
	 * @param angleDegrees Angle in degrees in wich the ellipse will be rotated
	 */
	void Renderer2DOpenCV::ellipse( Image& image, int x1, int y1, int width, int height, float angleDegrees /*= 0*/ )
	{
		// Check if the renderer is valid
		if ( !isValid() )
		{
			LOG_ERROR( "Renderer2DOpenCV has not been correctly initialized. Did you call the init() method?" );
			return;
		}

		// Check the image is valid
		if ( !image.isValid() )
		{
			LOG_ERROR( "Trying to draw in an invalid image! It should be initialized before by loading an image from disk, or just calling its init method." );
			return;
		}


		// Check scale
		if ( (width < 0) || (height < 0) )
		{
			LOG_ERROR( "Renderer2DOpenCV::ellipse. widht and height should be positive" );
			return;
		}
			
		float widthDIV2 = (float)width/2.0f;
		float heightDIV2 = (float)height/2.0f;

		// Get Stroke and Fill Color
		GraphicsManager& graphManager	= GraphicsManager::getSingleton();
		Color strokeColor				= graphManager.getStrokeColor();
		Color fillColor					= graphManager.getFillColor();
		int   strokeWeight				= graphManager.getStrokeWeight();

		// Create a cv::Mat header around the image contents
		cv::Mat imageMat = toCVMat( image );

		// Image where the drawing will be made (if we need transparency it will be another image and then will be blended)
		// as opencv does not support transparent drawing
		cv::Mat* canvasImage = &imageMat;
		cv::Mat* alphaCanvasImage = NULL;

		// If there is transparency involved
		if ( (graphManager.getStroke() && (strokeColor.a < 255)) || ( graphManager.getFill() && (fillColor.a < 255) ) )
		{
			// Request a temporary image to draw the transparent shape
			alphaCanvasImage = ImageResourceManager::getSingleton().getImage( image.getWidth(), image.getHeight(), image.getNChannels() );
		
			// Fill it with the current image content
			imageMat.copyTo( *alphaCanvasImage );

			// The canvas will be the temp image to draw on it and later on blend it with the current cv image
			canvasImage = alphaCanvasImage;
		}


		// Draw fill
		if (graphManager.getFill())
		{
			switch( graphManager.getEllipseMode() )
			{
			case CORNER:
				if (graphManager.getSmooth())
					cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(x1+width,y1+height), angleDegrees,	0, 360,	cv::Scalar( fillColor.r, fillColor.g, fillColor.b ),-1,CV_AA);
				else
					cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(x1+width,y1+height), angleDegrees,	0, 360,	cv::Scalar( fillColor.r, fillColor.g, fillColor.b ),-1,4 );
				break;

			case CORNERS:
				if (graphManager.getSmooth())
					cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(width,height), angleDegrees,	0, 360, cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
				else
					cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(width,height), angleDegrees,	0, 360, cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
				break;

			case CENTER:
				if (graphManager.getSmooth())
					cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize((int)widthDIV2,(int)heightDIV2), angleDegrees,	0, 360, cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
				else
					cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize((int)widthDIV2,(int)heightDIV2), angleDegrees,	0, 360, cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
				break;

			case RADIUS:
				if (graphManager.getSmooth())
					cv::ellipse( *canvasImage, cv::Point(x1-width,y1-height), cvSize(x1+width,y1+height), angleDegrees,	0, 360, cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, CV_AA);
				else
					cv::ellipse( *canvasImage, cv::Point(x1-width,y1-height), cvSize(x1+width,y1+height), angleDegrees,	0, 360, cv::Scalar( fillColor.r, fillColor.g, fillColor.b ), -1, 4);
				break;
			}
		}

		// Draw stroke
		if (graphManager.getStroke())
		{
			switch( graphManager.getEllipseMode() )
			{

			case CORNER:
				if (graphManager.getSmooth())
					cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(x1+width,y1+height), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
				else
					cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(x1+width,y1+height), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
				break;

			case CORNERS:
				if (graphManager.getSmooth())
					cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(width,height), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
				else
					cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize(width,height), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
				break;

			case CENTER:
				if (graphManager.getSmooth())
					cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize((int)widthDIV2,(int)heightDIV2), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
				else
					cv::ellipse( *canvasImage, cv::Point(x1,y1), cvSize((int)widthDIV2,(int)heightDIV2), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
				break;

			case RADIUS:
				if (graphManager.getSmooth())
					cv::ellipse( *canvasImage, cv::Point(x1-width,y1-height), cvSize(x1+width,y1+height), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, CV_AA);
				else
					cv::ellipse( *canvasImage, cv::Point(x1-width,y1-height), cvSize(x1+width,y1+height), angleDegrees,	0, 360, cv::Scalar( strokeColor.r, strokeColor.g, strokeColor.b ), strokeWeight, 4);
				break;
			}
		}

		// If there is transparency involved -> blend the result and release the temp image used for transparency
		if ( graphManager.getFill() && (fillColor.a < 255) )
		{
			cv::addWeighted( imageMat, (fillColor.getHighRange()-fillColor.a)/fillColor.getHighRange(), *canvasImage, fillColor.a/fillColor.getHighRange(), 0, imageMat );
			ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );

			// Note: Now theres is a limitation (opencv does not support transparency in the drawing API)
			// So we limit it to only 1 kind (fill or stroke), as we fake it blending the whole drawing with the current image, which is slow.
			// This warning is to nofity it
			if ( graphManager.getStroke() && (strokeColor.a < 255) )
				LOG_ERROR_NTIMES( 3, "Trying to draw with transparency in both fill and stroke is not supported. Only fill transparency will be applied" );

		}
		else if ( graphManager.getStroke() && (strokeColor.a < 255) )
		{
			cv::addWeighted( imageMat, (strokeColor.getHighRange()-strokeColor.a)/strokeColor.getHighRange(), *canvasImage, strokeColor.a/strokeColor.getHighRange(), 0, imageMat );
			ImageResourceManager::getSingleton().releaseImage( alphaCanvasImage );
		}
	}


	/**																																  
	 * @brief Draws text inside an image
	 *
	 * @param Image image to draw in (canvas)
	 * @param x1 x, first point
	 * @param y1 y, first point
	 * @param text  string
	 */
	void Renderer2DOpenCV::text( Image& image, int x1, int y1,  const std::string& text )
	{
		// Check if the renderer is valid
		if ( !isValid() )
		{
			LOG_ERROR( "Renderer2DOpenCV has not been correctly initialized. Did you call the init() method?" );
			return;
		}

		// Check the image is valid
		if ( !image.isValid() )
		{
			LOG_ERROR( "Trying to draw text in an invalid image! It should be initialized before by loading an image from disk, or just calling its init method." );
			return;
		}

		// Get Stroke and Fill Color
		GraphicsManager& graphManager = GraphicsManager::getSingleton();
		Color color			= graphManager.getStrokeColor();
		int   strokeWeight	= graphManager.getStrokeWeight();
		int	  fontScale		= 2;

		cv::putText(toCVMat(image), text, cv::Point(x1,y1), cv::FONT_HERSHEY_SIMPLEX, fontScale, cv::Scalar( color.r, color.g, color.b, color.a ));
	}

	void  Renderer2DOpenCV::fill( Image& image, const Color& color )
	{

		// Check if the renderer is valid
		if ( !isValid() )
		{
			LOG_ERROR( "Renderer2DOpenCV has not been correctly initialized. Did you call the init() method?" );
			return;
		}

		// Check the image is valid
		if ( !image.isValid() )
		{
			LOG_ERROR( "Trying to draw in an invalid image! It should be initialized before by loading an image from disk, or just calling its init method." );
			return;
		}

		// Create a cv::Mat header around the image contents
		cv::Mat imageMat = toCVMat( image );

		// Set the entire image 
		switch( imageMat.channels() )
		{
		case 1:
			imageMat = cv::Scalar(color.r);
			break;
		case 3:
			imageMat = cv::Scalar(color.r, color.g, color.b);
			break;
		case 4:
			imageMat = cv::Scalar(color.r, color.g, color.b, color.a);
			break;
		default:
			LOG_ERROR( "Renderer2DOpenCV::fill: Invalid number of channels in image" );
			break;
		}
	}

}