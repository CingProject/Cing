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

#include "Exception.h"
#include <sstream>
#include <stdarg.h>

#if OGRE_COMPILER == OGRE_COMPILER_MSVC
	#pragma warning (disable : 4996 )
#endif


namespace Cing
{

/**
 * @internal
 * Constructor. Initializes the exception message with the given message plus
 * the file's name and line where the exception was trown.
 * @param [in] message The message describing the error.
 * @param [in] fileName The file's name where the exception was thrown.
 * @param [in] The line number where the exception was thrown.
 */
Exception::Exception(const char* fileName, int line, const char* message, ...)
{
	// Extract string parameters
	char		msgFormated[2048];
	va_list		args;
	
	
	
	va_start	(args, message);
	sprintf		(msgFormated, message, args);
	va_end		(args);

    std::ostringstream oss;
    oss << "\n-----------Error!----------\n" << msgFormated << "\nException thrown at " << fileName << "(" << line << ")";
    m_message = oss.str();
}

} // namespace Cing