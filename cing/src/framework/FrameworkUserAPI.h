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

#ifndef _FrameworkUserAPI_h_
#define _FrameworkUserAPI_h_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "FrameworkPrereqs.h"
#include "Application.h"
#include "graphics/GraphicsTypes.h"

namespace Cing
{
// Application setup
void fullscreen 		( int width = -1, int height = -1, GraphicMode mode = OPENGL );
void vSync				( bool value = true ); // Note: must be called before size or fullscreen
void FSAA				( int fsaa );
void windowBorder		( bool border );
void windowMonitorIndex	( unsigned int index  );
void size				( int width, int height, GraphicMode mode = OPENGL );

// Application control
void exit();

// Execution control
void delay( unsigned int milliseconds );
void loop();
void noLoop();
void redraw();
void setFrameRate( int frameRate );

// Plugin related
void			registerPlugin	( Plugin& plugin );
Cing::Plugin*	getPlugin		( const std::string& pluginName );

} // namespace Cing

#endif // _FrameworkUserAPI_h_