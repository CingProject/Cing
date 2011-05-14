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
		LOG_CRITICAL	= LogManager::LOG_ERROR,	///< Critical log level. It means a failure that makes impossible the continuation of the execution (for a subsistem at least)
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