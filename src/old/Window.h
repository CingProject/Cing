#pragma once

#include <OgreRenderWindow.h>
#include "DebugOverlay.h"
#include "InputManager.h"

namespace OgreManager
{
    class Window
    {
        public:
        Ogre::RenderWindow *RenderWindow;
        Ogre::Camera *Camera;
        DebugOverlay *DebugOverlay;
        InputManager *InputManager;
        
        Window(Ogre::RenderWindow *window)
        {
            this->RenderWindow = window;
            Camera = 0;
            DebugOverlay = 0;
            this->InputManager = new ::OgreManager::InputManager(window);
        }
        
        ~Window()
        {
            delete InputManager;
        }
        
        void SetupCamera(Ogre::SceneManager *SceneManager);
        ::OgreManager::DebugOverlay *InitializeDebugOverlay();
    };
}