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

// Precompiled headers
#include "Cing-Precompiled.h"


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