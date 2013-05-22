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

#ifndef _Cing_Exception_H_
#define _Cing_Exception_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include <string>

// Macro to throw an an exception
#if defined( _MSC_VER )
	#define THROW_EXCEPTION(message, ...) { throw Exception(__FILE__, __LINE__, message, __VA_ARGS__ ); }
#else
	#define THROW_EXCEPTION(message, args...) { throw Exception(__FILE__, __LINE__, message, ## args ); }
#endif

namespace Cing
{

/**
 * @internal
 * The exception class. The library throws exceptions when something
 * wrong occurs. Check the exception's message for extended info.
 */
class Exception
{
public:

    // Constructor / Destructor
    Exception( const char* fileName, int line, const char* message, ... );

    // Queries

    /**
     * @internal
     * Returns the exception's message.
     * @return The exception's message.
     */
    const std::string& getErrorMessage() const { return m_message; }

private:
    std::string m_message; ///< The exception's message.
};

} // namespace Cing

#endif // _Exception_H_
