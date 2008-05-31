#include "OgreManager.h"
#include <OgreConfigFile.h>
#include <OgreRoot.h>

namespace OgreManager
{
    bool OgreManager::Initialize()
    {
        Root = new Ogre::Root("Plugins\\plugins.cfg");
        
        if(Root->showConfigDialog())
            PrimaryWindow = new Window(Root->initialise(true));       
        else
            return false;
            
        SceneManager = Root->createSceneManager(Ogre::ST_GENERIC);
            
        return true;
    };
    
    void OgreManager::LoadAssets()
    {
        // Load resource paths from config file
        Ogre::ConfigFile cf;
        std::string ResourcePath;
        ResourcePath = "..\\..\\..\\vision\\data\\";
        
        cf.load(ResourcePath + "resources.cfg");
        
        // Go through all sections & settings in the file
        Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

        Ogre::String secName, typeName, archName;
        while (seci.hasMoreElements())
        {
            secName = seci.peekNextKey();
            Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
            Ogre::ConfigFile::SettingsMultiMap::iterator i;
            for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                    ResourcePath + archName, typeName, secName);
            }
        }
        
        // Initialise, parse scripts etc
	    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();    
    }

    void OgreManager::Shutdown()
    {
        if(PrimaryWindow)
            delete PrimaryWindow;
        if(Root)
            delete Root;
    }
}