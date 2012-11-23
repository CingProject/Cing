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


#include "ResourceManager.h"
#include "Exception.h"
#include "eString.h"
#include "XMLElement.h"
#include "LogManager.h"
#include "SystemUtils.h"


// Ogre
#include "OgreRoot.h"
#include "OgreConfigFile.h"

// Framework
#include "framework/UserAppGlobals.h"

namespace Cing
{

	// Static const init
#ifdef WIN32
	std::string ResourceManager::resourcesFileName			= "resources.cfg";
	std::string ResourceManager::userResourcesDirName		= "../data/";
	std::string ResourceManager::userResourcesGroupName		= "UserData";
	std::string ResourceManager::pluginsPath				= "Plugins\\plugins.cfg";
	std::string ResourceManager::libDataPath				= "../../../cing_bin/data/";
	std::string ResourceManager::userDataPath				= "";
	std::string ResourceManager::userExecPath				= "";
	std::string resourcesPathInBundle						= "";
	
	
	// TODO: Fix this with XCode project properties/postbuild steps
	// TODO: Clean this up. resourcesPathInBundle is only mac side!
#elif __APPLE__
	std::string ResourceManager::resourcesFileName			= "resources.cfg";
	std::string ResourceManager::userResourcesDirName		= "../../data/";
	std::string ResourceManager::userResourcesGroupName		= "UserData";
	std::string ResourceManager::pluginsPath				= "plugins.cfg";
	std::string ResourceManager::libDataPath				= "Contents/Resources/cing_bin/data/";
	std::string ResourceManager::userDataPath				= "";
	std::string ResourceManager::userExecPath				= "";
	std::string resourcesPathInBundle						= "";
#endif
	
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
		LOG( "Creating Ogre Root. Plugins path: %s", (resourcesPathInBundle + pluginsPath).c_str() );
		new Ogre::Root( resourcesPathInBundle + pluginsPath );

		 // Store user data path in globals
		dataFolder = userDataPath;
		LOG("User Data Folder: %s", dataFolder.c_str() );

		// Load Cing Config file
		XMLElement xml;
		xml.load( "CingConfig.xml" );

		// Get cing data folder (the root is CingConfig)
		if ( xml.isValid() )
		{
			XMLElement cingDataFolderXMLElement = xml.getChild("Cing_Data_Folder");
			cingDataFolder = cingDataFolderXMLElement.getStringAttribute("relativePath");
		}
		else
			LOG_ERROR( "CingConfig.xml not found in data folder -> using default paths" );
		
		// Get Cing data path
		LOG( "Cing Data Folder: %s", cingDataFolder.c_str());
		if ( cingDataFolder != "" )
		{
			libDataPath = cingDataFolder;
		}
		else
			LOG_ERROR( "Cing Data Folder is empty: libDataPath will use default value: %s",  libDataPath.c_str() );
		
		// Load resource paths from config file
		Ogre::ConfigFile  cf;
		std::string resourcesFileAbsPath = libDataPath + resourcesFileName ;
		LOG( "Trying to load Ogre Resources file (resources.cfg) at: %s", resourcesFileAbsPath.c_str() ); 
		cf.load( resourcesFileAbsPath.c_str() );

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
	 * @brief Adds resource locations added by the user, so that the assets there are available
	 */
	void ResourceManager::loadUserResourceLocations()
	{
		// Add the resource location of the user data
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation( userDataPath, "FileSystem", userResourcesGroupName, true );

		// Finally add any extra resource locations the user might have added
		std::list<ResourceLocation>::iterator it = m_userAdditionalResourceLocations.begin();
		for ( ; it != m_userAdditionalResourceLocations.end(); ++it )
		{
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation( it->absPath, "FileSystem", userResourcesGroupName, it->recursive );
		}
	}

	/**
	* @brief Adds another resource location to be loaded (which may contain any asset: material scripts, shaders, textures, models...)
	* 
	* @param path Path to the folder to be added. It may be an absolute path, or relative to the executable
	* @param recursive If true, all the folders contained within the specified path will be added recursively. If false, only 
	* the specified folder will be added.
	* @note This method must be called first thing in the setup(), before creating the window with size/fullscreen
	*/
	void ResourceManager::addResourceLocation( const std::string path, bool recursive /*= true*/ )
	{	
		if ( !isValid() )
		{
			LOG_ERROR( "ResourceManager::addResourceLocation. Error: Trying to add a resource location before the ResourceManager has been initialized" );
			return;
		}

		std::string absPath = path;
		// if the path is not absolute, build it
		if ( isPathAbsolute( absPath ) == false )
			absPath = userExecPath + path;

		// Add the resource and initialise it
		//static int idCounter = 0;
		//std::string newResourceGroupName = userResourcesGroupName + toString(idCounter++);
		//Ogre::ResourceGroupManager::getSingleton().addResourceLocation( absPath, "FileSystem", userResourcesGroupName, recursive );
		//Ogre::ResourceGroupManager::getSingleton().loadResourceGroup( userResourcesGroupName );
		m_userAdditionalResourceLocations.push_back( ResourceLocation(absPath, recursive) );
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
	
#ifdef __APPLE__
	/**
	 * @internal
	 * @brief Mac version. Extracts the user application execution and data path.
	 *
	 * They are stored in userDataPath and userExecPath attributes.
	 */
	void ResourceManager::extractUserAppPathMAC()
	{
		char bundlePath[2048];
		char exePath[2048];
		char resourcesPath[2048];

		CFBundleRef mainBundle = CFBundleGetMainBundle();
		assert( mainBundle );
		
		// Get URLs
		
		CFURLRef mainBundleURL = CFBundleCopyBundleURL( mainBundle);
		assert( mainBundleURL);
		
		CFURLRef exeURL = CFBundleCopyExecutableURL( mainBundle);
		assert( exeURL);		
		
		CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL( mainBundle);
		assert( resourcesURL);	
		
		// Get Paths into strings
		
		CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
		assert( cfStringRef);
		
		CFStringRef cfExePathRef = CFURLCopyFileSystemPath( exeURL, kCFURLPOSIXPathStyle);
		assert( cfExePathRef);
		
		CFStringRef cfResourcesPathRef = CFURLCopyFileSystemPath( resourcesURL, kCFURLPOSIXPathStyle);
		assert( cfResourcesPathRef);	
		
		CFStringGetCString( cfStringRef, bundlePath, 1024, kCFStringEncodingASCII);
		CFStringGetCString( cfExePathRef, exePath, 1024, kCFStringEncodingASCII);
		CFStringGetCString( cfResourcesPathRef, resourcesPath, 1024, kCFStringEncodingASCII);

		// Release resources
		CFRelease( mainBundleURL);
		CFRelease( exeURL);
		CFRelease( cfStringRef);
		CFRelease( cfExePathRef);
		CFRelease( cfResourcesPathRef);

		
		userExecPath = bundlePath;
		//userExecPath = exePath;
		std::string::size_type lastSlashPos = userExecPath.find_last_of("/");
		userExecPath = userExecPath.substr(0, lastSlashPos);
		userExecPath = userExecPath + "/";
		//userDataPath = userExecPath + userResourcesDirName;
		
		resourcesPathInBundle	= String(bundlePath) + "/" + String(resourcesPath) + "/";
		//userDataPath	= String(bundlePath) + "/" + String(resourcesPath) + "/data/";
		userDataPath = String(bundlePath) + "/" + userResourcesDirName;
		//libDataPath		= String(bundlePath) + "/" + String(resourcesPath) + "/cing_bin/data/";
		libDataPath = String(bundlePath) + "/../../../../cing_bin/data/";
		
		// Log some info
		LOG( "Exec Path %s", exePath );
		LOG( "Bundle Path %s", bundlePath );
		LOG( "Resources Path %s", resourcesPath );
		LOG( "User Data Path %s", userDataPath.c_str() );
		
		const char* curentDir = getWorkingDir().c_str();
		LOG( "Current Working Dir: %s", getWorkingDir().c_str() );
		
	}
#endif

} // namespace Cing
