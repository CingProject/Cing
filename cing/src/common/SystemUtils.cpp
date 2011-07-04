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


#define _CRT_SECURE_NO_WARNINGS

#include "SystemUtils.h"
#include "LogManager.h"

#if defined( _MSC_VER )
	#include <direct.h>
	#include  <io.h>
#endif

#include  <stdio.h>
#include  <stdlib.h>


namespace Cing
{
	
	
	/**
	 * @brief Returns the current working directory
	 *
	 * @return A string containing the current working directory
	 */
	std::string getWorkingDir() 
	{ 
		char path[2048]; 
#if defined( _MSC_VER )
			_getcwd(path, sizeof(path)); 
#else
			getcwd(path, sizeof(path)); 
#endif
			return path; 
	}
	
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
		
		fp = fopen( fileName.c_str(), "rb" );
		if( fp != NULL )
		{
			fclose( fp );
			return true; // File exists
		}
		
		return false;	// File does not exist
	}
	/**
	 * @brief Checks if a folder exists
	 *
	 * @param folderPath Absolute path to the folder to be checked
	 * @return True if the folder exitst, false otherwise
	 */
	bool folderExists( const std::string& folderPath )
	{
#if defined( _MSC_VER )
		if( (_access( folderPath.c_str() , 0 )) != -1 )
			return true;
#endif
		
		return false;
	}


	/** Splits a path into the basePath (the folder) and the file name (just filename + extension.
     * @param[in]	path Path to split
     * @param[out]	outFileName The filename + extension (no folders in path)
     * @param[out]	outBasePath Full path to the folder that contains the file
	 */
	void splitFilename( const std::string& path, std::string& outFileName, std::string& outBasePath )
	{
		Ogre::StringUtil::splitFilename( path, outFileName, outBasePath  );
	}

	/**
	 * @brief Returns true if the received path is absolutel, false if it's relative
	 * @todo: This method might not be totally generic, potentially fix it using boost::filesystem or poco::filesystem
	 * @param path Path to check
	 * @return true if the received path is absolutel, false if it's relative
	 */
	bool isPathAbsolute( const std::string& path )
	{
		// Split the path
		std::string basePath, fileName;
		splitFilename( path, fileName, basePath );

		// Windows case (lookf
#if defined(WIN32)
		if ( basePath.find( ":" ) != std::string::npos )
			return true;
#else
		// Unix based
		if ( Ogre::StringUtil::startsWith( basePath, "/" ) )
			return true;
#endif

		return false;
	}

	
} // namespace Cing
