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

#include "ResourceManager.h"
#include "Exception.h"
#include "eString.h"
#include "XMLElement.h"
#include "LogManager.h"


// Ogre
#include "externLibs/Ogre3d/include/OgreRoot.h"
#include "externLibs/Ogre3d/include/OgreConfigFile.h"

// Framework 
#include "framework/UserAppGlobals.h"

namespace Common
{

	// Static const init
	std::string ResourceManager::resourcesFileName			= "resources.cfg";
	std::string ResourceManager::userResourcesDirName		= "../data/";
	std::string ResourceManager::userResourcesGroupName		= "UserData";
	std::string ResourceManager::pluginsPath				= "Plugins\\plugins.cfg";
	std::string ResourceManager::libDataPath				= "../../../vision/data/";
	std::string ResourceManager::userDataPath				= "";
	std::string ResourceManager::userExecPath				= "";

	/**
	* @internal
	* @brief Constructor. Initializes class attributes.
	*/
	ResourceManager::ResourceManager():
	m_bIsValid  ( false )
	{
	}

	/**
	* @internal
	* @brief Destructor. Class release.
	*/
	ResourceManager::~ResourceManager()
	{
		// Release resources
		end();
	}

	/**
	* @internal
	* @brief Initializes the Resource Manager, which makes ogre aware of all resources directories
	* @note This method does not actually load the resources, each subsystem of the library is in charche
	* of loading the actual resources that they need.
	* @note Ogre Root object is created here (to acces the resources management features), so this manager should be
	* initialized before the rest of Ogre related managers
	*/
	void ResourceManager::init()
	{
		// Obtain the user execution directory
		extractUserAppPath();

		// Init Ogre Root
		new Ogre::Root( pluginsPath );

		// Store user data path in globals
		Globals::dataFolder = userDataPath;

		// Load Cing Config file
		XMLElement xml;
		xml.load( "CingConfig.xml" );

		// Get cing data folder (the root is CingConfig)
		if ( xml.isValid() )
		{
			XMLElement cingDataFolder = xml.getChild("Cing_Data_Folder");
			Globals::cingDataFolder = cingDataFolder.getStringAttribute("relativePath");
		}
		else
			LOG( "CingConfig.xml not found in data folder -> using default paths" );

		// Get Cing data path
		if ( Globals::cingDataFolder != "" )
			libDataPath = Globals::cingDataFolder;

		// Load resource paths from config file
		Ogre::ConfigFile  cf;
		cf.load( libDataPath + resourcesFileName );

		// Go through all sections & settings in the file to add all library resource locations
		Ogre::String secName, typeName, archName;
		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
		while (seci.hasMoreElements())
		{
			// Get section name and data
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();

			// Iterate through section elements
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;

				// Add the resource location to the manager
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation( libDataPath + archName, typeName, secName );
			}
		}

		// Add the resource location of the user data
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation( userDataPath, typeName, userResourcesGroupName, true );


		m_bIsValid = true;
	}

	/**
	* @internal
	* @brief Releases the class resources. 
	* After this call no method of this object can be called without calling init method again.
	*/
	void ResourceManager::end()
	{

		m_bIsValid = false;
	}


	/**
	* @internal 
	* @brief Extracts the user data path and stores it in the attribute m_dataPath 
	*/
	void ResourceManager::extractUserAppPath()
	{
		// This is platform specific code
#ifdef WIN32
		extractUserAppPathWIN();
#elif LINUX
		extractUserAppPathLINUX();
#elif __APPLE__
		extractUserAppPathMAC();
#endif

	}

	// Windows specific code
#ifdef WIN32
#include <Windows.h>
	/**
	* @internal 
	* @brief Windows version. Extracts the user application execution and data path.
	*
	* They are stored in userDataPath and userExecPath attributes.
	*/
	void ResourceManager::extractUserAppPathWIN()
	{
		// Get executable's name
		char buffer[MAX_PATH];
		DWORD result = GetModuleFileName(NULL, buffer, MAX_PATH);
		if (result == 0)
			THROW_EXCEPTION( "Error Calling GetModuleFileName to get user data path" );

		// Find the extension position.
		userExecPath = buffer;
		std::string::size_type lastSlashPos = userExecPath.find_last_of("\\");

		// Store the user application and data path
		userExecPath = userExecPath.substr(0, lastSlashPos);
		userExecPath += "\\";
		userDataPath = userExecPath + userResourcesDirName;
	}
#endif

} // namespace Common