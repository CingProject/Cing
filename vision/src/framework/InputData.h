#pragma once

#include <OISPrereqs.h>
#include <OISKeyboard.h>
#include "InputManager.h"



namespace Data
{
    class InputData
    {
        private:
        // Raw key state information
        __int8 KeyStates[256];
        
        // Raw key transition information
        __int8 KeyTransitions[256];
        
        //Raw mouse transition state information
        __int8 MouseTransitions[10];
        
        public:
        OIS::MouseState CurrentMouseState;
        
        InputData()
        {
            memset(KeyStates, 0, sizeof(KeyStates));
            memset(KeyTransitions, 0, sizeof(KeyTransitions));
        }        
        
        // Is the key presently down?
        bool isDown(OIS::KeyCode code);
        
        //Is the mouse button presently down?
        bool isDown(OIS::MouseButtonID button);
        
        // Is the key presently up?
        bool isUp(OIS::KeyCode code);
        
        //Is the mouse button presently up?
        bool isUp(OIS::MouseButtonID button);
        
        // Has key just been pressed this last cycle?
        bool justDown(OIS::KeyCode code);
        
        // Has the mouse button been pressed this last cycle?
        bool justDown(OIS::MouseButtonID button);
        
        // Has key just been released this last cycle?
        bool justUp(OIS::KeyCode code);
        
        // Has the mouse button been released this last cycle?
        bool justUp(OIS::MouseButtonID button);
        
        void Update(OgreManager::InputManager &InputManager);
    };
}
