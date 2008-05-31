#include "AbstractGameState.h"
#include <OgrePrerequisites.h>

namespace OgreManager
{
    class DebugOverlay : public Data::AbstractGameState
    {
        Ogre::Overlay* Overlay;
        Ogre::RenderWindow *Window;
        Ogre::Camera *Camera;
        
        bool ShowOverlay;
        bool Wireframe;
        
        public:
        DebugOverlay(Ogre::Overlay *overlay, Ogre::RenderWindow *window, Ogre::Camera *camera)
        {
            Overlay = overlay;
            Window = window;
            Camera = camera;
            
            ShowOverlay = true;
            Wireframe = false;
        }
        
        void Reset();
        
        void UpdateStats();
        
        virtual void Initialize();
        virtual void Shutdown();

        virtual void HandleEvents(Data::InputData &InputData); //Handle events from input thread
        virtual void Update(); //Core game logic
        virtual void Draw(::OgreManager::OgreManager &OgreManager); //Handle Ogre's thread, tell OgreManager what to draw, what has moved, etc.
    };
}