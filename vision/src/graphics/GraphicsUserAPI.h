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

#ifndef _GraphicsUserAPI_h_
#define _GraphicsUserAPI_h_

#include "GraphicsPrereqs.h"

namespace Graphics
{

/**
 * @brief Graphics related functions that user can call 
 */

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

void strokeWeight( int weight); 
void strokeWeight( float weight);

} // namespace Graphics

#endif // _GraphicsUserAPI_h_