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
