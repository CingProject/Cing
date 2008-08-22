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

#ifndef _GraphicsUserAPI_h_
#define _GraphicsUserAPI_h_

#include "GraphicsPrereqs.h"

namespace Graphics
{

/**
 * @brief Graphics related functions that user can call
 */

// Render related functions
void setRenderMode( RenderMode mode );

// 2d
void strokeWeight( int weight );

// Color related functions
void fill( int gray );
void fill( int gray, int alpha );
void fill( int value1 , int value2, int value3 );
void fill( int value1 , int value2, int value3, int alpha );
void fill( const Color& color );
void fill( const Color& color, int alpha );

void stroke( int gray );
void stroke( int gray, int alpha );
void stroke( int value1 , int value2, int value3 );
void stroke( int value1 , int value2, int value3, int alpha );
void stroke( const Color& color );
void stroke( const Color& color, int alpha );

void setBackgroundColor( const Color& color );

// Camera 3D related functions
void	useDefault3DCameraControl	( bool useDefault );
void	useMouseCameraControl			( bool value );
void	useKeyboardCameraControl	( bool value );

// Debug methods
void	showFps ( bool show );


} // namespace Graphics

#endif // _GraphicsUserAPI_h_
