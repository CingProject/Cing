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
#include "ShapeManager.h"
#include "Image.h"
#include "Framework/UserAppGlobals.h"
#include "common/CommonConstants.h"

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
	Graphics::ShapeManager::getSingleton().line( x1,  y1,  z1, x2,  y2,  z2 );
};

//------------------------------------------------------------------------------------
void beginShape()
{
	Graphics::ShapeManager::getSingleton().beginShape(TRIANGLE_STRIP);
};

//------------------------------------------------------------------------------------
void beginShape(GraphicsType operation)
{
	Graphics::ShapeManager::getSingleton().beginShape(operation);
};

//------------------------------------------------------------------------------------
void vertex( float x, float y)
{
	Graphics::ShapeManager::getSingleton().vertex(x,y);
};

//------------------------------------------------------------------------------------
void vertex( float x, float y, float z)
{
	Graphics::ShapeManager::getSingleton().vertex(x,y,z);
};

//------------------------------------------------------------------------------------
void endShape()
{
	Graphics::ShapeManager::getSingleton().endShape();
};

//------------------------------------------------------------------------------------
void endShape(GraphicsType operation)
{
	Graphics::ShapeManager::getSingleton().endShape(operation);
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
	// Transform vertex before the drawing  call 
	Transform &t = Graphics::GraphicsManager::getSingleton().m_transforms.top();

	Vector v1 = t.applyTransform( Vector( x1, y1, 0) );
	Vector v2 = t.applyTransform( Vector( x2, y2, 0) );

	Graphics::GraphicsManager::getSingleton().m_canvas->line( v1.x, v1.y, v2.x, v2.y );
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
	// Transform vertex before the drawing  call 
	Transform &t = Graphics::GraphicsManager::getSingleton().m_transforms.top();

	Vector v1 = t.applyTransform( Vector( x1, y1, 0) );

	Graphics::GraphicsManager::getSingleton().m_canvas->point( v1.x, v1.y );
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
	// Transform vertex before the drawing  call 
	Transform &t = Graphics::GraphicsManager::getSingleton().m_transforms.top();

	Vector v1 = t.applyTransform( Vector( x1, y1, 0) );
	Vector v2 = t.applyTransform( Vector( x2, y2, 0) );
	Vector v3 = t.applyTransform( Vector( x3, y3, 0) );

	Graphics::GraphicsManager::getSingleton().m_canvas->triangle( v1.x, v1.y, v2.x, v2.y, v3.x, v3.y );
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
	// TODO:	Optimize

	// Store the top of the matrix stack
	Transform &t = Graphics::GraphicsManager::getSingleton().m_transforms.top();

	Vector v1, v2, v3, v4; 
	float rectWidth, rectHeight;
	float widthDIV2, heightDIV2;

	// Calculate transformed points
	switch( GraphicsManager::getSingleton().getRectMode() )
	{
	case CORNER: 
		v1 = t.applyTransform( Vector( x1, y1, 0) );
		v2 = t.applyTransform( Vector( x1+x2, y1, 0) );
		v3 = t.applyTransform( Vector( x1+x2, y1+y2, 0) );
		v4 = t.applyTransform( Vector( x1, y1+y2, 0) );
		break;

	case CORNERS: 
		rectWidth  = x2 - x1;
		rectHeight = y2 - y1;
		v1 = t.applyTransform( Vector( x1, y1, 0) );
		v2 = t.applyTransform( Vector( x1 + rectWidth, y1, 0) );
		v3 = t.applyTransform( Vector( x1 + rectWidth, y1 + rectHeight, 0) );
		v4 = t.applyTransform( Vector( x1, y1 + rectHeight, 0) );
		break;

	case CENTER:
		widthDIV2  = x2/2;
		heightDIV2 = y2/2;
		v1 = t.applyTransform( Vector( x1 - widthDIV2, y1 - heightDIV2, 0) );
		v2 = t.applyTransform( Vector( x1 + widthDIV2, y1 - heightDIV2, 0) );
		v3 = t.applyTransform( Vector( x1 + widthDIV2, y1 + heightDIV2, 0) );
		v4 = t.applyTransform( Vector( x1 - widthDIV2, y1 + heightDIV2, 0) );
		break;

	case RADIUS: 
		v1 = t.applyTransform( Vector( x1-x2,y1-y2, 0) );
		v2 = t.applyTransform( Vector( x1+x2,y1-y2, 0) );
		v3 = t.applyTransform( Vector( x1+x2,y1+y2, 0) );
		v4 = t.applyTransform( Vector( x1-x2,y1+y2, 0) );
		break;
	}

	Graphics::GraphicsManager::getSingleton().m_canvas->quad( v1.x , v1.y , v2.x , v2.y, v3.x , v3.y, v4.x , v4.y ); 
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
	// Transform vertex before the drawing  call 
	Transform &t = Graphics::GraphicsManager::getSingleton().m_transforms.top();

	Vector v1 = t.applyTransform( Vector( x1, y1, 0) );
	Vector v2 = t.applyTransform( Vector( x2, y2, 0) );
	Vector v3 = t.applyTransform( Vector( x3, y3, 0) );
	Vector v4 = t.applyTransform( Vector( x4, y4, 0) );

	Graphics::GraphicsManager::getSingleton().m_canvas->quad( v1.x , v1.y , v2.x , v2.y, v3.x , v3.y, v4.x , v4.y ); 
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
	//TODO: Manage transforms
	Transform &t = Graphics::GraphicsManager::getSingleton().m_transforms.top();
	Vector v1 = t.applyTransform( Vector( x, y, 0) );
	Graphics::GraphicsManager::getSingleton().m_canvas->ellipse( v1.x, v1.y, width, height, t.getRotation().z );
}

void arc( float x, float y,  float width, float height, float start, float stop )
{
	//TODO: Manage transforms
	Transform &t = Graphics::GraphicsManager::getSingleton().m_transforms.top();
	Vector v1 = t.applyTransform( Vector( x, y, 0) );
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
void rectMode( int mode )
{
	Graphics::GraphicsManager::getSingleton().setRectMode( mode );
}

/**
* @brief  The origin of the ellipse is modified by the ellipseMode() function. 
*         The default configuration is ellipseMode(CENTER), which specifies the
*					location of the ellipse as the center of the shape. 
*
* @param mode
*/
void ellipseMode( int mode )
{
	Graphics::GraphicsManager::getSingleton().setEllipseMode( mode );
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
void colorMode( GraphicsType mode, float range )
{

};

/**
 * @brief  	
 *
 * @param mode
 */
void colorMode( GraphicsType mode, float range1, float range2, float range3 )
{

};
/**
 * @brief  	
 *
 * @param mode
 */
void colorMode( GraphicsType mode, float range1, float range2, float range3, float range4 )
{
};
/**
 * @brief  	
 *
 * @param mode
 */
void pushStyle()
{
	// Add a new style, copying parameters from the last one.
	Graphics::GraphicsManager::getSingleton().m_styles.push_front( Graphics::GraphicsManager::getSingleton().m_styles.front() );
};
/**
 * @brief  	
 *
 * @param mode
 */
void popStyle()
{	
	// Pop the last style created
	if ( Graphics::GraphicsManager::getSingleton().m_styles.size() > 0 )
		Graphics::GraphicsManager::getSingleton().m_styles.pop_front();
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
	Image* tempImage = GraphicsManager::getSingleton().m_canvas;
	int    numPixels = tempImage->getWidth() * tempImage->getHeight();
	int    imageWidth    = tempImage->getWidth();
	int    yIndex    = 0;

	// Read entire canvas image
	for (int i = 0; i < numPixels ; i++)
	{
		if ( (i % imageWidth == 0) && (i != 0) )	
		  yIndex++;
		Globals::pixels[i] = tempImage->getPixel( i - imageWidth*yIndex, yIndex );
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
	Image* tempImage		= GraphicsManager::getSingleton().m_canvas;
	int    numPixels		= tempImage->getWidth() * tempImage->getHeight();
	int    imageWidth		= tempImage->getWidth();
	int    yIndex				= 0;

	// Paint pixels in the canvas image
	// TODO: Too slow! Optimize
	for (int i = 0; i < numPixels ; i++)
	{
		if ( (i % imageWidth == 0) && (i != 0) )	
			yIndex++;
		stroke( Globals::pixels[i] );
		tempImage->point( i - imageWidth*yIndex, yIndex);
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
		GraphicsManager::getSingleton().m_transforms.pop();
}

/**
 * @brief 
 *
 * @param mode
 */
void resetMatrix()
{	
	GraphicsManager::getSingleton().m_transforms.top().identity();
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
};
/**
 * @brief 
 *
 * @param mode
 */
void translate( float x, float y )
{
	GraphicsManager::getSingleton().m_transforms.top().translate( x, y, 0);
};
/**
 * @brief 
 *
 * @param mode
 */
void translate( float x, float y, float z )
{
	GraphicsManager::getSingleton().m_transforms.top().translate( x, y, z);
};
/**
 * @brief 
 *
 * @param mode
 */
void rotate( float x, float y, float z )
{
	GraphicsManager::getSingleton().m_transforms.top().rotate( x, y, z);
};
/**
 * @brief 
 *
 * @param mode
 */
void rotate( float angleX )
{
	GraphicsManager::getSingleton().m_transforms.top().rotate( 0, 0, angleX );
};
/**
 * @brief 
 *
 * @param mode
 */
void rotateX(	float angle )
{
	GraphicsManager::getSingleton().m_transforms.top().rotate( angle, 0, 0 );
};
/**
 * @brief 
 *
 * @param mode
 */
void rotateY(	float angle )
{
	GraphicsManager::getSingleton().m_transforms.top().rotate( 0, angle, 0 );
};
/**
 * @brief 
 *
 * @param mode
 */
void rotateZ(	float angle )
{
	GraphicsManager::getSingleton().m_transforms.top().rotate( 0, 0, angle );
};
/**
 * @brief 
 *
 * @param mode
 */
void scale(	float x, float y, float z )
{
	GraphicsManager::getSingleton().m_transforms.top().scale( x, y, z );
};

/**
 * @brief 
 *
 * @param mode
 */
void scale(	float x, float y)
{
	GraphicsManager::getSingleton().m_transforms.top().scale( x, y, 1 );
};
/**
 * @brief 
 *
 * @param mode
 */
void save( const String& name )
{
	GraphicsManager::getSingleton().save( name );
};
/**
 * @brief 
 *
 * @param mode
 */
bool loadCollada( const Common::String& fileName )
{
	return GraphicsManager::getSingleton().loadCollada( fileName );
};

} // namespace Graphics
