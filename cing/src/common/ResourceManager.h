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

#ifndef _CingResourceManager_h_
#define _CingResourceManager_h_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "CommonPrereqs.h"
#include "Singleton.h"

#include <string>

namespace Cing
{

/**
 * @internal
 * @brief Manages the resources of the library and the user
 */
	class ResourceManager: public SingletonStatic< ResourceManager >
{
public:

	// Singleton requirements
	friend class SingletonStatic< ResourceManager >;

	// Constructor / Destructor
	~ResourceManager();

	// Init / Release / Update
	void	init 						();
	void	end							();
	void	loadUserResourceLocations	();

	// Query  Methods
	bool	isValid	() { return m_bIsValid; }

	// Add Resource location
	void	addResourceLocation( const std::string path, bool recursive = true );

	// Constants
	static std::string resourcesFileName;		///< Name of the resources file (contains all the data paths of the library)
	static std::string userResourcesDirName;	///< Name of the directory where the user data should be stored (relative to the exe file)
	static std::string userResourcesGroupName;	///< Name of the resources group (Ogre internal) of the user data
	static std::string pluginsPath;				///< Path where the plugins to load by ogre are located
	static std::string libDataPath;				///< Path where the library's data is stored
	static std::string userDataPath;			///< Path where the user's data is stored. Here is where the application specific data should be placed
	static std::string userExecPath;			///< Path where the user's application is being executed

private:

	// Private constructor to ensure singleton
	ResourceManager								();

	// Private methods
	void extractUserAppPath				();
	void extractUserAppPathWIN		();
	void extractUserAppPathLINUX	();
	void extractUserAppPathMAC		();
	
	// Private types
	struct ResourceLocation
	{
		ResourceLocation( const std::string& _absPath, bool _recursive): absPath(_absPath), recursive(_recursive) {}
		std::string absPath;
		bool		recursive;
	};

	// Attributes
	std::list<ResourceLocation>	m_userAdditionalResourceLocations;	///< New resource locations that will be added
	bool						m_bIsValid;							///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Cing

#endif // _ResourceManager_h_