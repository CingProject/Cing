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

namespace Graphics
{

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
 * @brief Sets the color used to draw border of shapes drawn after this call.
 * 
 * It specifies color for shapes not using textures or lighting. Value range is 0..255
 * @param value1 Specifies the red value
 * @param value2 Specifies the green value
 * @param value3 Specifies the blue value
 */
void stroke( int value1 , int value2, int value3 )
{
	stroke( Color( value1, value2, value3, 1 ) );
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

/*
 * @brief Sets the width of the stroke used for lines, points, and the border around shapes. 
 * 
 * @param width Variable of type Int containing the weight (in pixels) of the stroke
 */
void strokeWeight( int weight )
{
	Graphics::GraphicsManager::getSingleton().setStrokeWeight(weight);
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

} // namespace Graphics