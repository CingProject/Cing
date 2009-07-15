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

#include "CommonUserAPI.h"
#include "LogManager.h"
#include "eString.h"

#include <stdarg.h>
#include <sstream>
#include <iostream>

// GUI
#include "gui/GUIManagerCEGUI.h"

// To use the outputdebugstring
#ifdef WIN32
#include <Windows.h>
#endif

namespace Common
{

/**
 * @brief Prints a message on console
 *
 * @param message Message to print. Different types of data can be appended using + sign.
 * Example: print( "This is an integer value = %d", myInt );
 */
void print( const char* msg, ... )
{
	// Extract string parameters
	char			msgFormated[1024];
	va_list		args;
	va_start	(args, msg);
	vsprintf	(msgFormated, msg, args);
	va_end		(args);

	// Print it to console
	std::cout << msgFormated;

	// Print it to debug output
	// TODO Decide policy
	GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().print( msgFormated );

	// If in windows, output it to the output debug string of the visual studio
	// TODO: decide policy
#if defined(WIN32)
	OutputDebugString( msgFormated );
#endif
}

/**
 * @brief Prints a message on console and adds the end of line (next print will go to the next line)
 *
 * @param message Message to print. Different types of data can be printed using printf nomenclature
 * Example: print( "This is an integer value = %d", myInt );
 */
void println( const char* msg, ... )
{
	// Extract string parameters
	char			msgFormated[1024];
	va_list		args;
	va_start	(args, msg);
	vsprintf	(msgFormated, msg, args);
	va_end		(args);

	// Print it
	std::cout << msgFormated;

	// Print it to debug output
	// TODO: decide policy
	GUI::GUIManagerCEGUI::getSingleton().getDebugOutput().println( msgFormated );

	// If in windows, output it to the output debug string of the visual studio
	// TODO: decide policy
#if defined(WIN32)
	OutputDebugString( msgFormated );
	OutputDebugString( "\n" );
#endif
}

/**
 * @brief Prints a message on console
 *
 * @param message Message to print.
 */
void print( String &msg )
{
  print( msg.toChar() );
}

/**
 * @brief Prints a message on console
 *
 * @param message Message to print.
 */
void println( String &msg )
{
  println( msg.toChar() );
}


/**
 * @brief Indicates whether the informative system messages should be output to the debug output
 *
 * @note If you are not debugging an application, is better in terms of performance no to set this
 * value to true.
 * @param value if true, normal system log messages will be output to the debug output
 */
void logNormalMsgsToDebugOutput( bool value )
{
	LogManager::getSingleton().logNormalMsgsToDebugOutput( value );
}


/**
 * @brief Indicates whether the error system messages should be output to the debug output
 *
 * @note It can affect the performance, but ideally there should be not error messages in an applciation
 * @param value if true, error system log messages will be output to the debug output
 */
void logErrorMsgsToDebugOutput( bool value )
{
	LogManager::getSingleton().logErrorMsgsToDebugOutput( value );
}

} // namespace Common
