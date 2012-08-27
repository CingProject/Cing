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

// Precompiled headers
#include "Cing-Precompiled.h"

#include "GraphicsUserAPI.h"
#include "GraphicsTypes.h"
#include "GraphicsManager.h"
#include "ShapeManager.h"
#include "Image.h"
#include "Framework/UserAppGlobals.h"
#include "common/CommonConstants.h"
#include "common/LogManager.h"
#include "graphics/FontProperties.h"
#include "graphics/FontManager.h"

namespace Cing
{


/**
 * @brief Set the background to a grayscale value, based on the current colorMode.
 *
 * @param[in] gray Float to set to the background
 */
void background(  float gray  )
{
	setBackgroundColor( Color( gray ) );
}

/**
 * @brief Set the background to a grayscale value, based on the current colorMode.
 *
 * @param[in] gray  Float to set the background
 * @param[in] alpha Float to set the background
 */
void background(  float gray, float alpha )
{
	setBackgroundColor( Color( gray, alpha ) );
}

/**
 * @brief Set the background to a grayscale value, based on the current colorMode.
 *
 * @param[in] x Float to set the background
 * @param[in] y Float to set the background
 * @param[in] z Float to set the background
 */
void background( float x, float y, float z )
{
	setBackgroundColor( Color(x, y, z ) );
}

/**
 * @brief Set the background to a grayscale value, based on the current colorMode.
 *
 * @param[in] x Float to set the background
 * @param[in] y Float to set the background
 * @param[in] z Float to set the background
 * @param[in] a Float to set the background
 */
void background( float x, float y, float z, float a )
{
	setBackgroundColor( Color(x, y, z, a ) );
}

/**
 * @brief Set the background to a grayscale value, based on the current colorMode.
 *
 * @param[in] rgb Int to set the background
 */
void background( int rgb )
{
	setBackgroundColor( Color(rgb) );
}

/**
 * @brief Set the background to a grayscale value, based on the current colorMode.
 *
 * @param[in] rgb Int to set the background
 * @param[in] a   Float to set the background
 */
void background( int rgb, float a )
{
	setBackgroundColor( Color(rgb, a ) );
}

/**
 * @brief Set the background to a color, based on the current colorMode. 
 *
 * @param[in] theColor Color to set to the background
 */
void background(  const Color& theColor  )
{
	setBackgroundColor( theColor );
}

/**
 * @brief Modifies the background of the window (really the main viewport in the window). For compatibility.
 *
 * @param[in] color Color to set to the background
 */
void setBackgroundColor( const Color& color )
{
	GraphicsManager::getSingleton().setBackgroundColor( color );
}


/**
 * @brief Sets the color used to draw lines and border around shapes.
 *
 * @param[in] gray Float specifies a value between white and black
 */
void stroke(  float gray  )
{
	GraphicsManager::getSingleton().setStrokeColor( Color( gray ) );
}

/**
 * @brief Sets the color used to draw lines and border around shapes.
 *
 * @param[in] gray  Float specifies a value between white and black
 * @param[in] alpha Float opacity of the stroke
 */
void stroke(  float gray, float alpha )
{
	GraphicsManager::getSingleton().setStrokeColor( Color( gray, alpha ) );
}

/**
 * @brief Sets the color used to draw lines and border around shapes.
 *
 * @param[in] r Float
 * @param[in] g Float
 * @param[in] b Float
 */
void stroke( float r, float g, float b )
{
	GraphicsManager::getSingleton().setStrokeColor( Color( r, g, b ) );
}

/**
 * @brief Sets the color used to draw lines and border around shapes.
 *
 * @param[in] r Float
 * @param[in] g Float
 * @param[in] b Float
 * @param[in] a Float
 */
void stroke( float r, float g, float b, float a )
{
	GraphicsManager::getSingleton().setStrokeColor( Color( r, g, b, a ) );
}

/**
 * @brief Sets the color used to draw lines and border around shapes.
 *
 * @param[in] rgb Int
 */
void stroke( int rgb )
{
	GraphicsManager::getSingleton().setStrokeColor( Color( rgb ) );
}

/**
 * @brief Sets the color used to draw lines and border around shapes.
 *
 * @param[in] rgb Int
 * @param[in] a   Float
 */
void stroke( int rgb, float a )
{
	GraphicsManager::getSingleton().setStrokeColor( Color( rgb, a ) );
}

/*
 * @brief Sets the color used to draw border of shapes drawn after this call.
 *
 * It specifies color for shapes not using textures or lighting. Value range is 0..255
 * @param color Variable of type Color containing the red, green, blue and alpha values for the new color
 */
void stroke( const Color& color )
{
	GraphicsManager::getSingleton().setStrokeColor( color );
}

/**
 * @brief Sets the render mode used to draw the objects in the scene
 *
 * @param mode Active render mode. Possible values: POINTS, WIREFRAME, SOLID
 */
void setRenderMode( RenderMode mode )
{
	GraphicsManager::getSingleton().setRenderMode( mode );
}


/*
 * @brief Sets the color used to fill the shapes drawn after this call.
 *
 * It specifies color for shapes not using textures or lighting. Value range is 0..255
 * @param gray Gray level. A value of 0 means black, and 255 means white.
 */
void fill( float gray )
{
	fill( Color( gray ) );
}

/*
 * @brief Sets the color used to fill the shapes drawn after this call.
 *
 * It specifies color for shapes not using textures or lighting. Value range is 0..255
 * @param gray Gray level. A value of 0 means black, and 255 means white.
 * @param alpha Alpha level. A value of 0 means transparent, and 255 means opaque.
 */
void fill( float gray, float alpha )
{
	fill( Color( gray, alpha ) );

}

/*
 * @brief Sets the color used to fill the shapes drawn after this call.
 *
 * It specifies color for shapes not using textures or lighting. Value range is 0..255
 * @param value1 Specifies the red value
 * @param value2 Specifies the green value
 * @param value3 Specifies the blue value
 */
void fill( float value1 , float value2, float value3 )
{
	fill( Color( value1, value2, value3, 255 ) );
}

/*
 * @brief Sets the color used to fill the shapes drawn after this call.
 *
 * It specifies color for shapes not using textures or lighting. Value range is 0..255
 * @param value1 Specifies the red value
 * @param value2 Specifies the green value
 * @param value3 Specifies the blue value
 * @param alpha  Specifies the alpha value. A value of 0 means transparent, and 255 means opaque.
 */
void fill( float value1 , float value2, float value3, float alpha )
{
	fill( Color( value1, value2, value3, alpha ) );
}

/*
 * @brief Sets the color used to fill the shapes drawn after this call.
 *
 * It specifies color for shapes not using textures or lighting. Value range is 0..255
 * @param color Variable of type Color containing the red, green, blue and alpha values for the new color
 */
void fill( const Color& color )
{

	GraphicsManager::getSingleton().setFillColor( color );
}

/*
 * @brief Sets the width of the stroke used for lines, points, and the border around shapes.
 *
 * @param width Variable of type Int containing the weight (in pixels) of the stroke
 */
void strokeWeight( int weight )
{
	GraphicsManager::getSingleton().setStrokeWeight(weight);
}

/*
 * @brief Disables filling geometry. If both noStroke() and noFill() are called, nothing
 * will be drawn to the screen.
 *
 * @param none
 */
void noFill()
{
	GraphicsManager::getSingleton().noFill();
}

/*
 * @brief Disables drawing the stroke (outline).
 *
 * @param none
 */
void noStroke()
{
	GraphicsManager::getSingleton().noStroke();
}

/*
 * @brief Draws all geometry with smooth (anti-aliased) edges. This will slow down the
 * frame rate of the application, but will enhance the visual refinement.
 *
 * @param none
 */
void smooth()
{
	GraphicsManager::getSingleton().smooth();
}

/*
 * @brief Draws all geometry with jagged (aliased) edges.
 *
 * @param none
 */
void noSmooth()
{
	GraphicsManager::getSingleton().noSmooth();
}

/**
 * @brief Allows to enable or disable the default 3d camera control
 *
 * @param useDefault If true, the default camera control will be enabled.
 * If false, it will be disable, so the user will be responsible to control the 3d camera
 * if it is required.
 *
 * @note The default camera control is composed by:
 * - mouse: controls rotation -> by default deactivated
 * - keyboard:
 *		- w,a,s,d: moves forward/backward/left/right
 *		- q,e: rotation
 *		- cursors: up, down movement
 *		- r: restore camera rotation to initial settings
 */
void enableDefault3DCameraControl( bool useDefault /*= true */ )
{
	GraphicsManager::getSingleton().enableDefault3DCameraControl( useDefault );
}

/**
 * @brief Activates or deactivates the default camera control through the mouse
 *
 * @param value if true, it activates mouse control, if false it deactivates it
 */
void enableMouseCameraControl( bool value  /*= true */)
{
	GraphicsManager::getSingleton().getDefaultCameraController().useMouse( value );
}

/**
 * @brief Activates or deactivates the default camera control through the keyboard
 *
 * @param value if true, it activates keyboard control, if false it deactivates it
 */
void enableKeyboardCameraControl( bool value  /*= true */)
{
	GraphicsManager::getSingleton().getDefaultCameraController().useKeyboard( value );
}


/**
 * @brief Makes the frames per second to be printed on the screen or not
 *
 * @param show if true the current fps wil be printed on screen, if false, it won't be printed
 */
void showFps( bool show /*= true */ )
{
	GraphicsManager::getSingleton().showFps( show );
}

/**
 * @brief Draws a line in 3d space
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param z1 z, first point
 * @param y2 y, second point
 * @param x2 x, second point
 * @param z2 z, second point
 */
void line( float x1, float y1, float z1, float x2, float y2, float z2)
{
    // Transform vertices before the drawing  call 
	Transform &t = GraphicsManager::getSingleton().m_shapesTransforms.top();

	Vector v1 = t.applyTransform( Vector( x1, y1, z1) );
	Vector v2 = t.applyTransform( Vector( x2, y2, z2) );

	ShapeManager::getSingleton().line( v1.x,  v1.y,  v1.z, v2.x,  v2.y,  v2.z );
};

//------------------------------------------------------------------------------------
void beginShape()
{
	ShapeManager::getSingleton().beginShape(TRIANGLE_STRIP);
};

//------------------------------------------------------------------------------------
void beginShape(GraphicsType operation)
{
	ShapeManager::getSingleton().beginShape(operation);
};

//------------------------------------------------------------------------------------
void vertex( float x, float y )
{
	// Transform vertices before the drawing  call 
	Transform &t = GraphicsManager::getSingleton().m_shapesTransforms.top();
	Vector v1 = t.applyTransform( Vector( x, y, 0 ) );

	ShapeManager::getSingleton().vertex(v1.x,v1.y);
};

//------------------------------------------------------------------------------------
void vertex( float x, float y, float z)
{
	// Transform vertices before the drawing  call 
	Transform &t = GraphicsManager::getSingleton().m_shapesTransforms.top();
	Vector v1 = t.applyTransform( Vector( x, y, z ) );

	ShapeManager::getSingleton().vertex(v1.x,v1.y,v1.z);
};

//------------------------------------------------------------------------------------
void endShape()
{
	ShapeManager::getSingleton().endShape();
};

//------------------------------------------------------------------------------------
void endShape(GraphicsType operation)
{
	ShapeManager::getSingleton().endShape(operation);
};



/*
* @Draws a line (a direct path between two points) to the screen
*
* It specifies color for shapes not using textures or lighting. Value range is 0..255
* @param x1 int or float: x-coordinate of the first point
* @param y1 int or float: y-coordinate of the first point
* @param x2 int or float: x-coordinate of the second point
* @param y2 int or float: y-coordinate of the second point
*/

void line( int x1, int y1, int x2, int y2 )
{
	// Transform vertex before the drawing  call
	Transform &t = GraphicsManager::getSingleton().m_shapesTransforms.top();

	Vector v1 = t.applyTransform( Vector( (float)x1, (float)y1, 0) );
	Vector v2 = t.applyTransform( Vector( (float)x2, (float)y2, 0) );

	GraphicsManager::getSingleton().m_canvas.line( (int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y );
};

/*
 * @Draws a point, a coordinate in space at the dimension of one pixel
 *
 * It specifies color for shapes not using textures or lighting. Value range is 0..255
 * @param x int or float: x-coordinate of the point
 * @param y int or float: y-coordinate of the point
 */

void point( int x, int y )
{
	// Transform vertex before the drawing  call
	Transform &t = GraphicsManager::getSingleton().m_shapesTransforms.top();

	Vector v1 = t.applyTransform( Vector( (float)x, (float)y, 0) );

	GraphicsManager::getSingleton().m_canvas.point( (int)v1.x, (int)v1.y );
};

/*
 * @A triangle is a plane created by connecting three points. The first two arguments
 * specify the first point, the middle two arguments specify the second point, and the
 * last two arguments specify the third point.
 *
 * @param x1 int or float: x-coordinate of the first point
 * @param y1 int or float: y-coordinate of the first point
 * @param x2 int or float: x-coordinate of the second point
 * @param y2 int or float: y-coordinate of the second point
 * @param x3 int or float: x-coordinate of the third point
 * @param y3 int or float: y-coordinate of the third point
 */

void triangle( int x1, int y1, int x2, int y2, int x3, int y3 )
{
	// Transform vertex before the drawing  call
	Transform &t = GraphicsManager::getSingleton().m_shapesTransforms.top();

	Vector v1 = t.applyTransform( Vector( (float)x1, (float)y1, 0) );
	Vector v2 = t.applyTransform( Vector( (float)x2, (float)y2, 0) );
	Vector v3 = t.applyTransform( Vector( (float)x3, (float)y3, 0) );

	GraphicsManager::getSingleton().m_canvas.triangle( (int)v1.x, (int)v1.y, (int)v2.x, (int)v2.y, (int)v3.x, (int)v3.y );
};

/**
 * @brief Draws a rectangle inside the 2dCanvas
 *
 * @param x x, first point
 * @param y y, first point
 * @param width x, end point
 * @param height y, end point
 */
void rect( int x, int y, int width, int height )
{
	// TODO:	Optimize

	// Store the top of the matrix stack
	Transform &t = GraphicsManager::getSingleton().m_shapesTransforms.top();

	Vector v1, v2, v3, v4;
	float rectWidth, rectHeight;
	float widthDIV2, heightDIV2;

	// Calculate transformed points
	switch( GraphicsManager::getSingleton().getRectMode() )
	{
	case CORNER:
		v1 = t.applyTransform( Vector( (float)x,		(float)y, 0) );
		v2 = t.applyTransform( Vector( (float)x+width,	(float)y, 0) );
		v3 = t.applyTransform( Vector( (float)x+width,	(float)y+height, 0) );
		v4 = t.applyTransform( Vector( (float)x,		(float)y+height, 0) );
		break;

	case CORNERS:
		rectWidth  = (float)(width - x);
		rectHeight = (float)(height - y);
		v1 = t.applyTransform( Vector( (float)x,				(float)y, 0) );
		v2 = t.applyTransform( Vector( (float)x + rectWidth,	(float)y, 0) );
		v3 = t.applyTransform( Vector( (float)x + rectWidth,	(float)y + rectHeight, 0) );
		v4 = t.applyTransform( Vector( (float)x,				(float)y + rectHeight, 0) );
		break;

	case CENTER:
		widthDIV2  = (float)width/2.0f;
		heightDIV2 = (float)height/2.0f;
		v1 = t.applyTransform( Vector( (float)x - widthDIV2, (float)y - heightDIV2, 0) );
		v2 = t.applyTransform( Vector( (float)x + widthDIV2, (float)y - heightDIV2, 0) );
		v3 = t.applyTransform( Vector( (float)x + widthDIV2, (float)y + heightDIV2, 0) );
		v4 = t.applyTransform( Vector( (float)x - widthDIV2, (float)y + heightDIV2, 0) );
		break;

	case RADIUS:
		v1 = t.applyTransform( Vector( (float)x-width,(float)y-height, 0) );
		v2 = t.applyTransform( Vector( (float)x+width,(float)y-height, 0) );
		v3 = t.applyTransform( Vector( (float)x+width,(float)y+height, 0) );
		v4 = t.applyTransform( Vector( (float)x-width,(float)y+height, 0) );
		break;
	}

	GraphicsManager::getSingleton().m_canvas.quad( (int)v1.x , (int)v1.y , 
													(int)v2.x , (int)v2.y, 
													(int)v3.x , (int)v3.y, 
													(int)v4.x , (int)v4.y );
}

/**
 * @brief Draws a quad, defined by four points
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param x2 x, second point
 * @param y2 y, second point
 * @param x3 x, third point
 * @param y3 y, third point
 * @param x4 x, fourth point
 * @param y4 y, fourth point
 */
void quad( int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4 )
{
	// Transform vertex before the drawing  call
	Transform &t = GraphicsManager::getSingleton().m_shapesTransforms.top();

	Vector v1 = t.applyTransform( Vector( (float)x1, (float)y1, 0) );
	Vector v2 = t.applyTransform( Vector( (float)x2, (float)y2, 0) );
	Vector v3 = t.applyTransform( Vector( (float)x3, (float)y3, 0) );
	Vector v4 = t.applyTransform( Vector( (float)x4, (float)y4, 0) );

	GraphicsManager::getSingleton().m_canvas.quad( (int)v1.x , (int)v1.y , 
													(int)v2.x , (int)v2.y, 
													(int)v3.x , (int)v3.y, 
													(int)v4.x , (int)v4.y );
}

/**
 * @brief Draws an ellipse
 *
 * @param x x, first point
 * @param y y, first point
 * @param width
 * @param height
 */
void ellipse( int x, int y, int width, int height)
{
	//TODO: Manage transforms
	//Transform &t = GraphicsManager::getSingleton().m_shapesTransforms.top();
	//Vector v1 = t.applyTransform( Vector( (float)x, (float)y, 0) );
	//GraphicsManager::getSingleton().m_canvas.ellipse( (int)v1.x, (int)v1.y, width, height, t.getRotation().z );

	// Get current transformation
	Transform &t = GraphicsManager::getSingleton().m_shapesTransforms.top();
	
	// Transform position
	Vector v1 = t.applyTransform( Vector( (float)x, (float)y, 0) );

	// Transform scale
	Matrix4 scaleRotMatrix = t.getTransformMatrix() * Matrix4::getScale( (float)width, (float)height, 1.0f );

	// Extract scale/rotation
	Vector pos(0, 0, 0);
	Vector scale(0, 0, 0);
	Quaternion orient;
	scaleRotMatrix.decomposition( pos, scale, orient );	
	float yaw	= orient.getYaw().valueRadians();
	float pitch = orient.getPitch().valueRadians();
	float roll	= orient.getRoll().valueRadians();

	GraphicsManager::getSingleton().m_canvas.ellipse( (int)v1.x, (int)v1.y, (int)scale.x, (int)scale.y );


}

void arc( int x, int y,  int width, int height, float start, float stop )
{
	//TODO: Manage transforms
	Transform &t = GraphicsManager::getSingleton().m_shapesTransforms.top();
	
	// Transform position
	Vector v1 = t.applyTransform( Vector( (float)x, (float)y, 0) );

	// Transform scale
	Matrix4 scaleRotMatrix = t.getTransformMatrix() * Matrix4::getScale( (float)width, (float)height, 1.0f );

	// Extract scale/rotation
	Vector pos(0, 0, 0);
	Vector scale(0, 0, 0);
	Quaternion orient;
	scaleRotMatrix.decomposition( pos, scale, orient );	
	float yaw	= orient.getYaw().valueRadians();
	float pitch = orient.getPitch().valueRadians();
	float roll	= orient.getRoll().valueRadians();

	// Note: widtha and height are swaped becuase the way opencv expects them (TODO: check)
	GraphicsManager::getSingleton().m_canvas.arc( (int)v1.x, (int)v1.y, (int)scale.y, (int)scale.x, start - roll, stop - roll );
}

/**
 * @brief  	Modifies the location from which images draw. The default mode is
 * rectMode(CORNER), which specifies the location to be the upper left corner of
 * the shape and uses the third and fourth parameters of rect() to specify the width
 * and height. The syntax rectMode(CORNERS) uses the first and second parameters
 * of rect() to set the location of one corner and uses the third and fourth parameters
 * to set the opposite corner. The syntax rectMode(CENTER) draws the image from its
 * center point and uses the third and forth parameters of rect() to specify the image's
 * width and height. The syntax rectMode(RADIUS) draws the image from its center point
 * and uses the third and forth parameters of rect() to specify half of the image's
 * width and height
 *
 * @param mode
 */
void imageMode( int mode )
{
	GraphicsManager::getSingleton().setImageMode( mode );
}

/**
 * @brief  	Modifies the location from which rectangles draw. The default mode is
 * rectMode(CORNER), which specifies the location to be the upper left corner of
 * the shape and uses the third and fourth parameters of rect() to specify the width
 * and height. The syntax rectMode(CORNERS) uses the first and second parameters
 * of rect() to set the location of one corner and uses the third and fourth parameters
 * to set the opposite corner. The syntax rectMode(CENTER) draws the image from its
 * center point and uses the third and forth parameters of rect() to specify the image's
 * width and height. The syntax rectMode(RADIUS) draws the image from its center point
 * and uses the third and forth parameters of rect() to specify half of the image's
 * width and height
 *
 * @param mode
 */
void rectMode( int mode )
{
	GraphicsManager::getSingleton().setRectMode( mode );
}

/**
* @brief  The origin of the ellipse is modified by the ellipseMode() function.
*         The default configuration is ellipseMode(CENTER), which specifies the
*		  location of the ellipse as the center of the shape.
*
* @param mode
*/
void ellipseMode( int mode )
{
	GraphicsManager::getSingleton().setEllipseMode( mode );
}

/**
 * @brief  	Changes the colorMode and/or range of colors. Modes RGB and HSB
 *			Default range is 0-255
 *
 * @param mode
 */

void colorMode( GraphicsType mode )
{
	Color::colorMode( mode, 255, 255, 255, 255 );
};


/**
 * @brief
 *
 * @param mode
 */
void pushStyle()
{
	// Add a new style, copying parameters from the last one.
	GraphicsManager::getSingleton().m_styles.push_front( GraphicsManager::getSingleton().m_styles.front() );
};
/**
 * @brief
 *
 * @param mode
 */
void popStyle()
{
	// Pop the last style created
	if ( GraphicsManager::getSingleton().m_styles.size() > 0 )
		GraphicsManager::getSingleton().m_styles.pop_front();
};

//Create functions
/**
 * @brief
 *
 * @param mode
 */
Image createImage(int width, int height, GraphicsType format)
{
	return Image( width,  height,  format);
};
/**
 * @brief
 *
 * @param mode
 */
Image loadImage( const std::string& name)
{
	Image newImage;
	newImage.load( name );
	return newImage;
}
/**
 * @brief Loads the pixel data for the display window into the pixels[] vector.
 *        This function must always be called before reading from or writing to pixels[].
 *
 * @param mode
 */
void loadPixels()
{
	// Create temporal image to allow an easy access to the data
	const Image& tempImage = GraphicsManager::getSingleton().m_canvas;
	int    numPixels = tempImage.getWidth() * tempImage.getHeight();
	int    imageWidth= tempImage.getWidth();
	int    yIndex    = 0;

	// Read entire canvas image
	for (int i = 0; i < numPixels ; i++)
	{
		if ( (i % imageWidth == 0) && (i != 0) )
		  yIndex++;
		pixels[i] = tempImage.getPixel( i - imageWidth*yIndex, yIndex );
	}

}
/**
 * @brief Updates the display window with the data in the pixels[] array.
 *        Use in conjunction with loadPixels()
 *
 * @param mode
 */
void updatePixels()
{
	// Create temporal image to allow an easy access to the data
	Image& tempImage	= GraphicsManager::getSingleton().m_canvas;
	int    numPixels	= tempImage.getWidth() * tempImage.getHeight();
	int    imageWidth	= tempImage.getWidth();
	int    yIndex		= 0;

	// Paint pixels in the canvas image
	// TODO: Too slow! Optimize
	for (int i = 0; i < numPixels ; i++)
	{
		if ( (i % imageWidth == 0) && (i != 0) )
			yIndex++;
		stroke( pixels[i] );
		tempImage.point( i - (imageWidth*yIndex), yIndex);
	}
}
/**
 * @brief
 *
 * @param mode
 */
void pushMatrix()
{
	// Add new IDENTITY transform object to the stack
	GraphicsManager::getSingleton().m_transforms.push( GraphicsManager::getSingleton().m_transforms.top() );
	GraphicsManager::getSingleton().m_shapesTransforms.push( GraphicsManager::getSingleton().m_shapesTransforms.top() );
}
/**
 * @brief
 *
 * @param mode
 */
void popMatrix()
{
	// Pop the last transform
	if ( GraphicsManager::getSingleton().m_transforms.size() > 0 )
	{
		GraphicsManager::getSingleton().m_transforms.pop();
		GraphicsManager::getSingleton().m_shapesTransforms.pop();
	}
}

/**
 * @brief
 *
 * @param mode
 */
void resetMatrix()
{
	GraphicsManager::getSingleton().m_transforms.top().identity();
	GraphicsManager::getSingleton().m_shapesTransforms.top().identity();
}

/**
 * @brief
 *
 * @param mode
 */
void printMatrix()
{
	GraphicsManager::getSingleton().m_transforms.top().printMatrix();
}
/**
 * @brief
 *
 * @param mode
 */
void applyMatrix( float m00, float m01, float m02, float m03,
				  float m10, float m11, float m12, float m13,
				  float m20, float m21, float m22, float m23,
				  float m30, float m31, float m32, float m33 )
{
	GraphicsManager::getSingleton().m_transforms.top().applyMatrix( m00, m01, m02, m03,
																	m10, m11, m12, m13,
																	m20, m21, m22, m23,
																	m30, m31, m32, m33);

	GraphicsManager::getSingleton().m_shapesTransforms.top().applyMatrix(	m00, m01, m02, m03,
																			m10, m11, m12, m13,
																			m20, m21, m22, m23,
																			m30, m31, m32, m33);

};
/**
 * @brief
 *
 * @param mode
 */
void translate( float x, float y )
{
	translate(x, y, 0);
};
/**
 * @brief
 *
 * @param mode
 */
void translate( float x, float y, float z )
{
	GraphicsManager::getSingleton().m_transforms.top().translate( x, y, z );
	GraphicsManager::getSingleton().m_shapesTransforms.top().translate( x, y, z );
};

/**
 * @brief
 *
 * @param mode
 */
void rotate( float angleZRad )
{
	rotateZ(angleZRad);
};
/**
 * @brief
 *
 * @param mode
 */
void rotateX( float angleRad )
{
	GraphicsManager::getSingleton().m_transforms.top().rotateX( angleRad );
	GraphicsManager::getSingleton().m_shapesTransforms.top().rotateX( angleRad );

};
/**
 * @brief
 *
 * @param mode
 */
void rotateY( float angleRad )
{
	GraphicsManager::getSingleton().m_transforms.top().rotateY( angleRad );
	GraphicsManager::getSingleton().m_shapesTransforms.top().rotateY( angleRad );
};
/**
 * @brief
 *
 * @param mode
 */
void rotateZ( float angleRad )
{
	GraphicsManager::getSingleton().m_transforms.top().rotateZ( angleRad );

	// Note: The angle is inverted in the shape transformations (in Processing coordinate system Mode)
	// This is because 3d objects and Images are centered in relation to its parent node, but shapes are not
	// to be able to have 1 shared parent node for all shapes (performance reasons)
	// Therefore the coordinate system PROCESSING with the scale (1, -1, 1) of the root node, affects shapes and the rest
	// of the objects differently.
	if ( GraphicsManager::getSingleton().isProcessingMode() )
		GraphicsManager::getSingleton().m_shapesTransforms.top().rotateZ( -angleRad );
	else
		GraphicsManager::getSingleton().m_shapesTransforms.top().rotateZ( angleRad );
};
/**
 * @brief
 *
 * @param mode
 */
void scale(	float x, float y, float z )
{
	GraphicsManager::getSingleton().m_transforms.top().scale( x, y, z );
	GraphicsManager::getSingleton().m_shapesTransforms.top().scale( x, y, z );
};

/**
 * @brief
 *
 * @param mode
 */
void scale(	float x, float y)
{
	GraphicsManager::getSingleton().m_transforms.top().scale( x, y, 1 );
	GraphicsManager::getSingleton().m_shapesTransforms.top().scale( x, y, 1 );
};

/**
 * @brief Saves the current frame been rendered into an image file
 * @param name Path (relative to data folder or absolute) for the file to be created
 */
void saveCurrentFrame( const String& name )
{
	GraphicsManager::getSingleton().saveFrame( name );
};

/**
 * @brief Saves an area/rectangle of the current frame been rendered into an image file
 * @param name Path (relative to data folder or absolute) for the file to be created
 * @param rect rectangle to be saved
 */
void saveCurrentFrame( const String& name, const Rect& rect )
{
	GraphicsManager::getSingleton().saveFrame( name );
};

//----------------------------------------------------------------------------------- 
// Typography
//-----------------------------------------------------------------------------------

/**
 * Draws text to the screen in 2d coordinates (screen coordinates)
 * @param text	text to print on screen
 * @param x		x coordinate where the text will be printed
 * @param y		y coordinate where the text will be printed
 */
void text( const String& text, float x, float y )
{
	// Check empty text
	if ( text == "" )
	{
		LOG_ERROR( "Trying to pring empty text" );
		return;
	}

	// Set the font properties
	FontProperties& currentFontProperties	= FontManager::getSingleton().getActiveFontProperties();
	currentFontProperties.render2d			= true;
	currentFontProperties.text				= text;
	currentFontProperties.x					= x;
	currentFontProperties.y					= y;
	currentFontProperties.width				= -1;
	currentFontProperties.height			= -1;
	
	// Add the text to the manager so that it gets rendered in the next draw
	FontManager::getSingleton().addText();
}


/**
 * Draws text to the screen in 3d (3d world coordinates)
 * @param text	text to print on screen
 * @param x		x coordinate where the text will be printed
 * @param y		y coordinate where the text will be printed
 */
void text( const String& text, float x, float y, float z )
{
	// Check empty text
	if ( text == "" )
	{
		LOG_ERROR( "Trying to pring empty text" );
		return;
	}

	// Set the font properties
	FontProperties& currentFontProperties	= FontManager::getSingleton().getActiveFontProperties();
	currentFontProperties.render2d			= false;
	currentFontProperties.text				= text;
	currentFontProperties.x					= x;
	currentFontProperties.y					= y;
	currentFontProperties.z					= z;
	currentFontProperties.width				= -1;
	currentFontProperties.height			= -1;

	// Add the text to the manager so that it gets rendered in the next draw
	FontManager::getSingleton().addText();
}

/**
 * Draws text to the screen in 2d coordinates (screen coordinates)
 * @param text	text to print on screen
 * @param x			x coordinate where the text will be printed
 * @param y			y coordinate where the text will be printed
 * @param width		width of the text box that will be printed
 * @param height	height of the text box that will be printed. If -1, the height will not be limited
 */
void text( const String& text, float x, float y, float width, float height )
{
	// Set the font properties
	FontProperties& currentFontProperties	= FontManager::getSingleton().getActiveFontProperties();
	currentFontProperties.render2d			= true;
	currentFontProperties.text				= text;
	currentFontProperties.x					= x;
	currentFontProperties.y					= y;
	currentFontProperties.width				= width;
	currentFontProperties.height			= height;

	// Add the text to the manager so that it gets rendered in the next draw
	FontManager::getSingleton().addText();
}

/**
 * Draws text to the screen in 3d (3d world coordinates)
 * @param text	text to print on screen
 * @param x			x coordinate where the text will be printed
 * @param y			y coordinate where the text will be printed
 * @param width		width of the text box that will be printed
 * @param height	height of the text box that will be printed
 */
void text( const String& text, float x, float y, float z, float width, float height )
{
	// Set the font properties
	FontProperties& currentFontProperties	= FontManager::getSingleton().getActiveFontProperties();
	currentFontProperties.render2d			= false;
	currentFontProperties.text				= text;
	currentFontProperties.x					= x;
	currentFontProperties.y					= y;
	currentFontProperties.z					= z;
	currentFontProperties.width				= width;
	currentFontProperties.height			= height;

	// Add the text to the manager so that it gets rendered in the next draw
	FontManager::getSingleton().addText();
}


/**
 * Sets the font used to draw from now on
 * @param font	font used to draw in future calls to text
 */
void textFont(Font& font)
{
	textFont( font, font.getFontSize() );
}

/**
 * Sets the font used to draw from now on
 * @param font	font used to draw in future calls to text
 * @param size	size that will be used to draw the text. It is best to specify the size in the font.load call.
 */
void textFont(Font& font, float size)
{
	// Now update the properties (to set it as active font)
	FontProperties& currentFontProperties	= FontManager::getSingleton().getActiveFontProperties();
	currentFontProperties.fontName			= font.getFontName();
	currentFontProperties.size				= size;

	FontManager::getSingleton().setActiveFont( font );

}

/**
 * Sets the font resolution (default is 96)
 * @param resolution resolution that will be used to draw the text.
 */
void textResolution(int resolution)
{
	FontProperties& currentFontProperties	= FontManager::getSingleton().getActiveFontProperties();
	currentFontProperties.resolution = resolution;

	FontManager::getSingleton().updateActiveFontProperties();
}

/**
 * Sets the spacing between lines of text in units of pixels. 
 * This setting will be used in all subsequent calls to the text() function.
 * @param leading distance in pixels between different lines of text
 */
void textLeading(float leading)
{
	FontProperties& currentFontProperties	= FontManager::getSingleton().getActiveFontProperties();
	currentFontProperties.leading			= leading;

	FontManager::getSingleton().updateActiveFontProperties();
}	

/**
 * Allows (if set to true), to have line breaks only between and not within words, except when a single word is longer than a line.
 * @param wordWrap if true, words will not be split to break lines in a text block. If false, words will be split 
 */
void textWordWrap(bool wordWrap)
{
	FontProperties& currentFontProperties	= FontManager::getSingleton().getActiveFontProperties();
	currentFontProperties.wordWrap			= wordWrap;

	FontManager::getSingleton().updateActiveFontProperties();
}


/**
 * Sets the horizontal and vertical alignment for the text drawn from now on
 * @param halign	Horizontal alignment
 * @param valign	Vertical alignment
 */
void textAlign(int halign, int valign)
{
	FontProperties& currentFontProperties	= FontManager::getSingleton().getActiveFontProperties();
	currentFontProperties.halign			= halign;
	currentFontProperties.valign			= valign;
}

/**
 * Sets font size in pixels of the subsequent text calls
 * @param size	size in pixels of the subsequent text calls. Always is better to specify the size in the Font.load
 * to ensuze best rendering quality
 */
void textSize(float size)
{
	FontProperties& currentFontProperties	= FontManager::getSingleton().getActiveFontProperties();
	currentFontProperties.size				= size;
	FontManager::getSingleton().updateActiveFontProperties();
}

/* 
 * Calculates and returns the width of any character or text string, considering the active font and it's properties
 * 
 * @param text Text to analyze for it's pixel length
 * @return the width of the received text in pixels
 */ 
float textWidth( const std::string& text )
{
	return FontManager::getSingleton().textWidth( text );
}

/* 
 * Calculates and returns the height of any character or text string, considering the active font and it's properties, plus
 * the received text box width (so that if the text will be limited in width, we can know how much vertical space it will take, 
 * which will depend on the total number of lines we need).
 * 
 * @param text			Text to analyze for it's pixel height
 * @param textBoxWidth	If left blank, the text height will be calculated based on it's new line '\n' characters. Otherwise this
 * specifies the horizontal limit (width of the text box that will be used to render the text, and will end up determining the number o
 * lines that are required to draw it).
 * @return the height of the received text in pixels
 */
float textHeight( const std::string& text, float textBoxWidth /*= -1*/ )
{
	return FontManager::getSingleton().textHeight( text, textBoxWidth );
}

   /**
	* @brief Split a text area into lines. Returns a vector of strings.
	* @param text			Input text
	* @param textBoxWidth	This specifies the horizontal limit (width of the text box that will be used to render the text,
	* and will end up determining the number o lines that are required to draw it).
	*/
std::vector< std::string > splitInLines( const std::string& text, float textBoxWidth )
{
	return FontManager::getSingleton().splitInLines( text, textBoxWidth );
}

/**
 * Sets the 3d coordinate system. Thre are two systems: OPENGL3D and PROCESSING (default)
 * @param[in] coordSystem the input coordSystem 	 
 */
void applyCoordinateSystemTransform( const GraphicsType& coordSystem )
{
	GraphicsManager::getSingleton().applyCoordinateSystemTransform( coordSystem );
}

//-----------------------------------------------------------------------------------
// Shadows
//-----------------------------------------------------------------------------------

/**
 * @brief Enables use of shadows. Should be called in the setup before initializing any 3d object.
 * @param[in] technique Shadow technique to be used
 */
void enableShadows( ShadowTechnique technique )
{
	GraphicsManager::getSingleton().enableShadows( technique );
}

/**
 * @brief Sets the shadow color used to modulate areas in shaodw (when shadows that modulate are enabled)
 * @param[in] color color of the shadow. Range 0..255
 */
void setShadowColor( const Color& color )
{
	GraphicsManager::getSingleton().setShadowColor( color );
}

//-----------------------------------------------------------------------------------
// Coordinate System related
//-----------------------------------------------------------------------------------


/**
 * @brief Returns the screen coordinate (range 0..1) that relates to the received world coordinate
 * @param[in] worldCoordinate The world coordinate to transform to screen coordinate. The output coordinate will be in the range [0,1)
 */
Vector2 worldToScreenNormalized( const Vector& worldCoordinate )
{
	Vector posToTransform = worldCoordinate;
	const Matrix4& viewMatrix = GraphicsManager::getSingleton().getActiveCamera().getOgreCamera()->getViewMatrix();
	const Matrix4& projMatrix = GraphicsManager::getSingleton().getActiveCamera().getOgreCamera()->getProjectionMatrix();

	// Transform this into (non homogenous) clip space 
	Vector hcsPosition = projMatrix * (viewMatrix * worldCoordinate); 

	// Normalize coordinates
	Vector2 screenPos;
	screenPos.x = ((hcsPosition.x * 0.5f) + 0.5f);// 0 <= x <= 1 // left := 0,right := 1
	screenPos.y = 1.0f - ((hcsPosition.y * 0.5f) + 0.5f);// 0 <= y <= 1 // bottom := 0,top := 1

	return screenPos;
}

/**
 * @brief Returns the screen coordinate that relates to the received world coordinate
 * @param[in] worldCoordinate The world coordinate to transform to screen coordinate (in pixel units)
 */
Vector2 worldToScreen( const Vector& worldCoordinate )
{
	Vector2 screenCoords = worldToScreenNormalized( worldCoordinate );
	
	// Transform to pixel units (right now they are normalized, that is, in the range 0..1)
	screenCoords.x *= width;
	screenCoords.y *= height;

	return screenCoords;
}


/**
 * @brief Returns the world coordinate that relates to the received screen coordinate
 * @param[in] screenCoordinate The screen coordinate to transform to world coordinate
 * @param[in] distanceToCamera Distance from the camera position, where the 3d world should be calculated
 */
Vector screenToWorld( const Vector2& screenCoordinate, float distanceToCamera )
{
	// Get current camera (TODO make this parameter)
	Ogre::Camera* camera = GraphicsManager::getSingleton().getActiveCamera().getOgreCamera();
	Ogre::Viewport* viewport = camera->getViewport();
	if ( !camera || !viewport)
	{
		LOG_ERROR( "getCameraToViewportRay. Cannot get current Ogre Camera or Viewport, one fo them is NULL" );
		return Vector(-1, -1, -1);
	}

	// Check boundaries
	if ( (screenCoordinate.x  < 0) || (screenCoordinate.x  > width) ||
		 (screenCoordinate.y  < 0) || (screenCoordinate.y  > height) )
	{
		LOG_ERROR( "getCameraToViewportRay. Screen coordinates are out of the screen. Cannot project them to world coordinates." );
		return Vector(-1, -1, -1);

	}

	// Calculate normalized screen coordinate (0..1)
	float scrx = screenCoordinate.x / viewport->getActualWidth(); 
	float scry = screenCoordinate.y / viewport->getActualHeight();

	// Cast the ray to world coordinates
	Ogre::Ray	ray = camera->getCameraToViewportRay(scrx, scry);
	
	return ray.getPoint(distanceToCamera);

}

/**
 * @brief Returns the world coordinate that relates to the received screen coordinate
 * @param[in] screenCoordinate The screen coordinate to transform to world coordinate
 * @param[in] distanceToCamera Distance from the camera position, where the 3d world should be calculated
 */
Vector screenToWorld( const Vector& screenCoordinate, float distanceToCamera )
{
	return screenToWorld( Vector2(screenCoordinate.x, screenCoordinate.y), distanceToCamera );
}

//----------------------------------------------------------------------------------- 
// Helpers
//----------------------------------------------------------------------------------- 
unsigned int numberOfChannels ( GraphicsType format )
{
	return Ogre::PixelUtil::getNumElemBytes( (Ogre::PixelFormat)format );
}


} // namespace Cing
