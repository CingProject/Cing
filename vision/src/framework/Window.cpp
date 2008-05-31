#include "Window.h"
#include <OgreOverlayManager.h>
#include <OgreSceneManager.h>
#include <OgreRoot.h>

namespace OgreManager
{
    void Window::SetupCamera(Ogre::SceneManager *SceneManager)
    {
        Camera = SceneManager->createCamera(RenderWindow->getName());
        Camera->setPosition(2000, 2000, 2000);
        Camera->lookAt(0,0,0);
        
        RenderWindow->addViewport(Camera);
    }
    
    ::OgreManager::DebugOverlay *Window::InitializeDebugOverlay()
    {
        this->DebugOverlay = new ::OgreManager::DebugOverlay(Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay"), 
            RenderWindow, Camera);
        RenderWindow->resetStatistics();
        return this->DebugOverlay;
    }
}