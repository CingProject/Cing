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

#define _CRT_SECURE_NO_WARNINGS

#include "SystemUtils.h"


namespace Common
{

/**
 * @brief Checks if a file exists
 *
 * @param fileName Path to the file to that will be checked
 * @return True if the file exitst, false otherwise
 */
bool fileExists( const std::string& fileName )
{
  FILE* fp = NULL;

  //will not work if you do not have read permissions
  //to the file, but if you don't have read, it
  //may as well not exist to begin with.

  fopen_s( &fp, fileName.c_str(), "rb" );
  if( fp != NULL )
  {
      fclose( fp );
      return true; // File exists
  }

  return false;	// File does not exist
}

} // namespace Common
