/*
  This source file is part of the Vision project
  For the latest info, see http://www.XXX.org

  Copyright (c) 2008 XXX

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

#ifndef _ResourceManager_h_
#define _ResourceManager_h_

#include "CommonPrereqs.h"
#include "Singleton.h"

#include <string>

namespace Common
{

/**
 * @internal
 * @brief Manages the resources of the library and the user
 */
	class ResourceManager: public Common::SingletonStatic< ResourceManager >
{
public:

	// Singleton requirements
	friend class Common::SingletonStatic< ResourceManager >;

	// Constructor / Destructor
	~ResourceManager();

	// Init / Release / Update
	void	init 	();
	void	end		();	

	// Query  Methods
	bool	isValid	() { return m_bIsValid; }

	// Constants
	static std::string resourcesFileName;			///< Name of the resources file (contains all the data paths of the library)
	static std::string userResourcesDirName;	///< Name of the directory where the user data should be stored (relative to the exe file)
	static std::string userResourcesGroupName;///< Name of the resources group (Ogre internal) of the user data
	static std::string pluginsPath;						///< Path where the plugins to load by ogre are located
	static std::string libDataPath;						///< Path where the library's data is stored
	static std::string userDataPath;					///< Path where the user's data is stored. Here is where the application specific data should be placed
	static std::string userExecPath;					///< Path where the user's application is being executed

private:

	// Private constructor to ensure singleton
	ResourceManager								();

	// Private methods
	void extractUserAppPath				();
	void extractUserAppPathWIN		();
	void extractUserAppPathLINUX	();
	void extractUserAppPathMAC		();
	
	// Attributes
	bool	m_bIsValid;	  ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Common

#endif // _ResourceManager_h_