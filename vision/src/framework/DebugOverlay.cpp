#include "DebugOverlay.h"
#include <OgreOverlayManager.h>
#include <OgreRenderTarget.h>
#include <OgreRenderWindow.h>
#include <OgreOverlayElement.h>
#include <OgreStringConverter.h>
#include <OgreCamera.h>
#include "InputData.h"

using namespace Ogre;

namespace OgreManager
{   
    void DebugOverlay::Initialize()
    {
    
    }
    
    void DebugOverlay::Shutdown()
    {
        
    }
    
    void DebugOverlay::Reset()
    {
        Window->resetStatistics();
    }

    void DebugOverlay::HandleEvents(Data::InputData &InputData)
    {
        if(InputData.justDown(OIS::KC_F12))
            ShowOverlay = !ShowOverlay;        
        
        if(InputData.justDown(OIS::KC_F11))
            Wireframe = !Wireframe;
        
    }
    
    void DebugOverlay::Update()
    {   
        
    }
    
    void DebugOverlay::Draw(::OgreManager::OgreManager &OgreManager)
    {
        if(ShowOverlay)
            Overlay->show();
        else
            Overlay->hide();
        
        if(Wireframe)
            Camera->setPolygonMode(Ogre::PM_WIREFRAME);
        else
            Camera->setPolygonMode(Ogre::PM_SOLID);
            
        UpdateStats();
    }
    
    void DebugOverlay::UpdateStats()
    {
        static String currFps = "Current FPS: ";
        static String avgFps = "Average FPS: ";
        static String bestFps = "Best FPS: ";
        static String worstFps = "Worst FPS: ";
        static String tris = "Triangle Count: ";

        // update stats when necessary
        try
        {
            OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
            OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
            OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
            OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

            const RenderTarget::FrameStats& stats = Window->getStatistics();

            guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
            guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
            guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
                +" "+StringConverter::toString(stats.bestFrameTime)+" ms");
            guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
                +" "+StringConverter::toString(stats.worstFrameTime)+" ms");

            OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
            guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));
        }
        catch(...)
        {
            // ignore
        }
    }
}