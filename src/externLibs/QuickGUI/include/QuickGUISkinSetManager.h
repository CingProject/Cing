#ifndef QUICKGUISKINSETMANAGER_H
#define QUICKGUISKINSETMANAGER_H

#include "QuickGUIExportDLL.h"
#include "QuickGUISkinSet.h"

#include "OgrePrerequisites.h"
#include "OgreSingleton.h"

#include <map>

namespace QuickGUI
{
	class _QuickGUIExport SkinSetManager :
		public Ogre::Singleton<SkinSetManager>
	{
	public:
		friend class Root;
	public:
		static SkinSetManager& getSingleton(void); 
		static SkinSetManager* getSingletonPtr(void);

		/**
		* Checks if skinName is a loaded skin SkinSet, and if textureName is an
		* embedded texture within the skin SkinSet.
		*/
		bool embeddedInSkinSet(const std::string& skinName, const std::string& textureName);
		bool embeddedInSkinSet(const std::string& textureName);

		/**
		* Returns the SkinSet if exists, NULL otherwise.
		*/
		SkinSet* getSkinSet(const std::string& name);
		/*
		* Returns skinset which has this texture name, null otherwise
		*/
		SkinSet* getSkinSetByTextureName(const std::string& texName);
		std::map<std::string,SkinSet*>* getSkinSetList();

		void loadSkin(const std::string& skinName, SkinSet::ImageType t, const std::string &resourceGroup = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		/*
		* Returns true if a skin has been loaded with the name skinName, false otherwise.
		*/
		bool skinLoaded(const std::string& skinName);

	protected:
		SkinSetManager();
		~SkinSetManager();

		std::map<std::string,SkinSet*> mSkinSets;
		std::map<std::string,SkinSet*> mSkinSetsbyTextureName;
	};
}

#endif
