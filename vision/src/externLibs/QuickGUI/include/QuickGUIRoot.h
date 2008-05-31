#ifndef QUICKGUIROOT_H
#define QUICKGUIROOT_H

#include "QuickGUIExportDLL.h"
#include "QuickGUIManager.h"
#include "QuickGUISkinSetManager.h"

#include "OgreException.h"
#include "OgrePrerequisites.h"
#include "OgreSingleton.h"
#include "OgreViewport.h"

#include <map>

namespace QuickGUI
{
	class _QuickGUIExport Root :
		public Ogre::Singleton<Root>
	{
	public:
		Root();
		~Root();

		static Root& getSingleton(void); 
		static Root* getSingletonPtr(void);

		GUIManager* createGUIManager(const std::string& name, Ogre::Viewport* v, const std::string& defaultSkin);
		GUIManager* createGUIManager(Ogre::Viewport* v, const std::string& defaultSkin);

		void destroyGUIManager(GUIManager* gm);
		void destroyGUIManager(const std::string& name);

		GUIManager* getGUIManager(const std::string& name);

	protected:
		int mGUIManagerCounter;

		std::map<std::string,GUIManager*> mGUIManagers;
	};
}

#endif
