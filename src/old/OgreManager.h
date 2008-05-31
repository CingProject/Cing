#pragma once

#include <OgrePrerequisites.h>
#include "Window.h"

namespace OgreManager
{
    class OgreManager
    {
        public:
        Ogre::Root *Root;
        Ogre::SceneManager *SceneManager;
        Window *PrimaryWindow;
        
        bool Initialize();
        void Shutdown();
        void LoadAssets();
        
        OgreManager()
        {
            Root = 0;
            SceneManager = 0;
            PrimaryWindow = 0;
        }
    };
}