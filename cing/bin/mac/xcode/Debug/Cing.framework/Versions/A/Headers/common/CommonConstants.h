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

#ifndef _CommonConstants_h_
#define _CommonConstants_h_

// Precompiled headers
#include "Cing-Precompiled.h"


// OIS
#include "OISMouse.h"

#include "LogManager.h"


/**
* @internal
* @file This file contains common constant definitions
*/
namespace Cing
{
// Input

	// Mouse buttons
	const int LEFT		= OIS::MB_Left;
	const int RIGHT		= OIS::MB_Right;
	const int CENTER	= OIS::MB_Middle;

	// Drawing Modes
	const int CORNER	= 0;
	const int CORNERS	= 1;
	//const int CENTER	= 2;
	const int RADIUS	= 3;

	// Logging
	enum LogMessageLevel
	{
		LOG_TRIVIAL		= LogManager::LOG_TRIVIAL,	///< Trivial log level. It means that there is no problem, it is just informational
		LOG_NORMAL		= LogManager::LOG_NORMAL,	///< Normal log level. It means that there is some problem, but the application can continue executing
		LOG_CRITICAL	= LogManager::LOG_CRITICAL,	///< Critical log level. It means a failure that makes impossible the continuation of the execution (for a subsistem at least)
		LOG_SILENT		= LogManager::LOG_SILENT	///< No message will be output
	};



// Typography

	/// Mode in which the text will be rendered 
	enum TextMode 
	{ 
		MODEL,	///< Render in 3d space (although it can have only 2d coordinates, but it will be rendered in 3d space)
		SCREEN	///< Screen space (it will be rendered over all 3d stuff)	
	};
	
	/// Allightment of text (in relation to its center coordinates)
	//const int LEFT		= 0;	// Shared with mouse
	//const int RIGHT		= 1;	// Shared with mouse
	//const int CENTER		= 2;	// Shared with mouse

	/// Vertical alignment
	const int TOP			= 1; 
	//const int CENTER		= 2;	// Shared with mouse
	const int BOTTOM		= 3; 
	
}


#endif // _CommonConstants_h_