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

#ifndef _Cing_Exception_H_
#define _Cing_Exception_H_

#include <string>

// Macro to throw an an exception
#define THROW_EXCEPTION(message) { throw Exception(message, __FILE__, __LINE__); }


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
    Exception( const std::string& message, const char* fileName, int line );

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