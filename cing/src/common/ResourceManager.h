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