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
Exception::Exception(const std::string& message, const char* fileName, int line)
{
    std::ostringstream oss;
    oss << "\n-----------Error!----------\n" << message << "\nException thrown at " << fileName << "(" << line << ")";
    m_message = oss.str();
}


} // namespace Cing