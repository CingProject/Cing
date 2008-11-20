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

#include "GraphicsUserAPI.h"
#include "GraphicsManager.h"
#include "Image.h"

namespace Graphics
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
	setBackgroundColor( Color(rgb, rgb, rgb, rgb ) );
}

/**
 * @brief Set the background to a grayscale value, based on the current colorMode. 
 *
 * @param[in] rgb Int to set the background
 * @param[in] a   Float to set the background
 */
void background( int rgb, float a )
{
	setBackgroundColor( Color(rgb, rgb, rgb, a ) );
}

void background( const Image& image );

/**
 * @brief Modifies the background of the window (really the main viewport in the window). For compatibility.
 * 
 * @param[in] color Color to set to the background
 */
void setBackgroundColor( const Color& color )
{
	Graphics::GraphicsManager::getSingleton().setBackgroundColor( color );
}


/**
 * @brief Sets the color used to draw lines and border around shapes.
 *
 * @param[in] gray Float specifies a value between white and black
 */
void stroke(  float gray  )
{
	Graphics::GraphicsManager::getSingleton().setStrokeColor( Color( gray ) );
}

/**
 * @brief Sets the color used to draw lines and border around shapes.
 *
 * @param[in] gray  Float specifies a value between white and black
 * @param[in] alpha Float opacity of the stroke
 */
void stroke(  float gray, float alpha )
{
	Graphics::GraphicsManager::getSingleton().setStrokeColor( Color( gray, alpha ) );
}

/**
 * @brief Sets the color used to draw lines and border around shapes.
 *
 * @param[in] x Float
 * @param[in] y Float
 * @param[in] z Float
 */
void stroke( float x, float y, float z )
{
	Graphics::GraphicsManager::getSingleton().setStrokeColor( Color( x, y, z ) );
}

/**
 * @brief Sets the color used to draw lines and border around shapes.
 *
 * @param[in] x Float
 * @param[in] y Float
 * @param[in] z Float
 * @param[in] a Float
 */
void stroke( float x, float y, float z, float a )
{
	Graphics::GraphicsManager::getSingleton().setStrokeColor( Color( x, y, z, a ) );
}

/**
 * @brief Sets the color used to draw lines and border around shapes.
 *
 * @param[in] rgb Int 
 */
void stroke( int rgb )
{
	Graphics::GraphicsManager::getSingleton().setStrokeColor( Color( rgb, rgb, rgb, rgb ) );
}

/**
 * @brief Sets the color used to draw lines and border around shapes.
 *
 * @param[in] rgb Int 
 * @param[in] a   Float
 */
void stroke( int rgb, float a )
{
	Graphics::GraphicsManager::getSingleton().setStrokeColor( Color( rgb, rgb, rgb, a ) );
}

/*
 * @brief Sets the color used to draw border of shapes drawn after this call.
 * 
 * It specifies color for shapes not using textures or lighting. Value range is 0..255
 * @param color Variable of type Color containing the red, green, blue and alpha values for the new color
 */
void stroke( const Color& color )
{
	Graphics::GraphicsManager::getSingleton().setStrokeColor( color );
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
void fill( int gray )
{
	fill( Color( gray, gray, gray, 1 ) );
}

/*
 * @brief Sets the color used to fill the shapes drawn after this call.
 * 
 * It specifies color for shapes not using textures or lighting. Value range is 0..255
 * @param gray Gray level. A value of 0 means black, and 255 means white.
 * @param alpha Alpha level. A value of 0 means transparent, and 255 means opaque.
 */
void fill( int gray, int alpha )
{
	fill( Color( gray, gray, gray, alpha ) );

}

/*
 * @brief Sets the color used to fill the shapes drawn after this call.
 * 
 * It specifies color for shapes not using textures or lighting. Value range is 0..255
 * @param value1 Specifies the red value
 * @param value2 Specifies the green value
 * @param value3 Specifies the blue value
 */
void fill( int value1 , int value2, int value3 )
{
	fill( Color( value1, value2, value3, 1 ) );
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
void fill( int value1 , int value2, int value3, int alpha )
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
 * @brief Sets the color used to fill the shapes drawn after this call.
 * 
 * It specifies color for shapes not using textures or lighting. Value range is 0..255
 * @param color Variable of type Color containing the red, green, blue and alpha values for the new color
 * @param alpha Specifies the alpha value. A value of 0 means transparent, and 255 means opaque.
 */
void fill( const Color& color, int alpha )
{
	// Create a new color to set the alpha without modifying the received parameter
	Color newColor = Color( color.r, color.g, color.b, alpha );

	// Set the fill color
	GraphicsManager::getSingleton().setFillColor( newColor );
}


/*
 * @brief Sets the width of the stroke used for lines, points, and the border around shapes. 
 * 
 * @param width Variable of type Int containing the weight (in pixels) of the stroke
 */
void strokeWeight( int weight )
{
	Graphics::GraphicsManager::getSingleton().setStrokeWeight(weight);
}

/*
 * @brief Disables filling geometry. If both noStroke() and noFill() are called, nothing
 * will be drawn to the screen.
 * 
 * @param none
 */
void noFill()
{
	Graphics::GraphicsManager::getSingleton().noFill();
}

/*
 * @brief Disables drawing the stroke (outline).
 * 
 * @param none
 */
void noStroke()
{
	Graphics::GraphicsManager::getSingleton().noStroke();
}

/*
 * @brief Draws all geometry with smooth (anti-aliased) edges. This will slow down the
 * frame rate of the application, but will enhance the visual refinement.
 * 
 * @param none
 */
void smooth()
{
	Graphics::GraphicsManager::getSingleton().smooth();
}

/*
 * @brief Draws all geometry with jagged (aliased) edges.
 * 
 * @param none
 */
void noSmooth()
{
	Graphics::GraphicsManager::getSingleton().noSmooth();
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
void useDefault3DCameraControl( bool useDefault )
{
	Graphics::GraphicsManager::getSingleton().useDefault3DCameraControl( useDefault );
}

/**
 * @brief Activates or deactivates the default camera control through the mouse
 *
 * @param value if true, it activates mouse control, if false it deactivates it
 */
void useMouseCameraControl( bool value )
{
	Graphics::GraphicsManager::getSingleton().getDefaultCameraController().userMouse( value );
}

/**
 * @brief Activates or deactivates the default camera control through the keyboard
 *
 * @param value if true, it activates keyboard control, if false it deactivates it
 */
void useKeyboardCameraControl( bool value )
{
	Graphics::GraphicsManager::getSingleton().getDefaultCameraController().userKeyboard( value );
}


/**
 * @brief Makes the frames per second to be printed on the screen or not
 *
 * @param show if true the current fps wil be printed on screen, if false, it won't be printed
 */
void showFps( bool show )
{
	Graphics::GraphicsManager::getSingleton().showFps( show );
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
	Graphics::GraphicsManager::getSingleton().addVertex( Vector(x1, y1, z1));
	Graphics::GraphicsManager::getSingleton().addVertex( Vector(x2, y2, z2));
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

void line( float x1, float y1, float x2, float y2 )
{
	Graphics::GraphicsManager::getSingleton().line( x1, y1, x2, y2 );
};

/*
 * @Draws a point, a coordinate in space at the dimension of one pixel
 * 
 * It specifies color for shapes not using textures or lighting. Value range is 0..255
 * @param x1 int or float: x-coordinate of the point
 * @param y1 int or float: y-coordinate of the point
 */

void point( float x1, float y1 )
{
	Graphics::GraphicsManager::getSingleton().point( x1, y1 );
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

void triangle( float x1, float y1, float x2, float y2, float x3, float y3 )
{
	Graphics::GraphicsManager::getSingleton().triangle( x1, y1, x2, y2, x3, y3 );
};

/**
 * @brief Draws a rectangle inside the 2dCanvas
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param x2 x, end point
 * @param y2 y, end point
 */
void rect( float x1, float y1, float x2, float y2 )
{
	Graphics::GraphicsManager::getSingleton().rect( x1, y1, x2, y2 );
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
void quad( float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4 )
{
	Graphics::GraphicsManager::getSingleton().quad( x1, y1, x2, y2, x3, y3, x4, y4 );
}

/**
 * @brief Draws an ellipse
 *
 * @param x1 x, first point
 * @param y1 y, first point
 * @param width
 * @param height
 */
void ellipse( float x, float y, float width, float height)
{
	Graphics::GraphicsManager::getSingleton().ellipse( x, y, width, height );
}

void arc( float x, float y,  float width, float height, float start, float stop )
{
	Graphics::GraphicsManager::getSingleton().m_canvas->arc( x, y, width, height, start, stop );
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
void rectMode( RectMode mode )
{
	Graphics::GraphicsManager::getSingleton().setRectMode( mode );
}


} // namespace Graphics