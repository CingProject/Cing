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

// Precompiled headers
#include "Cing-Precompiled.h"


#include "CommonUserAPI.h"
#include "eString.h"
#include "ResourceManager.h"

#include <stdarg.h>
#include <sstream>
#include <iostream>

// GUI
#include "gui/GUIManagerCEGUI.h"

// To use the outputdebugstring
#ifdef WIN32
#include <Windows.h>
#endif

namespace Cing
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
	GUIManagerCEGUI::getSingleton().getDebugOutput().print( msgFormated );

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
	GUIManagerCEGUI::getSingleton().getDebugOutput().println( msgFormated );

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
 * @brief Set the output log level. The lower the level, the more verbose Cing will be.
 *
 * @note If you are not debugging an application, is better in terms of performance no to set the LOG_ERROR as log level
 * to avoid having to much log messages
 * @param level Log Level (LOG_ERROR, LOG_NORMAL, or LOG_TRIVIAL);
 */
void setLogLevel ( LogMessageLevel level )
{
	LogManager::getSingleton().setLogLevel( (LogManager::LogMessageLevel)level );
}

/**
 * @brief Adds another resource location to be loaded (which may contain any asset: material scripts, shaders, textures, models...)
 * 
 * @param path Path to the folder to be added. It may be an absolute path, or relative to the executable
 * @param recursive If true, all the folders contained within the specified path will be added recursively. If false, only 
 * the specified folder will be added.
 */
void addResourceLocation( const std::string path, bool recursive /*= true*/ )
{
	ResourceManager::getSingleton().addResourceLocation( path, recursive );
}



} // namespace Cing
