/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

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