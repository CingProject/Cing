#include "InputData.h"

namespace Data
{
    void InputData::Update(OgreManager::InputManager &InputManager)
    {
        //Copy key states
        static __int8 OldKeys[256];
        memcpy(OldKeys, KeyStates, 256);
        memset(KeyTransitions, 0, 256);
        InputManager.getKeyboard()->copyKeyStates(KeyStates);
        
        memset(KeyTransitions, 0, 256);
        for(int x = 0; x < 256; ++x)
        {
            //This is a key transition
            if(OldKeys[x] != KeyStates[x])
            {
                if(KeyStates[x] == 1) //Pressed
                    KeyTransitions[x] = 1;
                else                  //Released
                    KeyTransitions[x] = -1;
            }
        }
        
        //Copy mouse button states
        static OIS::MouseState OldMouse;
        memcpy(&OldMouse, &CurrentMouseState, sizeof(OldMouse));
        memset(&MouseTransitions, 0, sizeof(MouseTransitions));
        CurrentMouseState = InputManager.getMouse()->getMouseState();
        
        for(int x = 0; x < 10; ++x)
        {
            if(CurrentMouseState.buttonDown((OIS::MouseButtonID)x) != OldMouse.buttonDown((OIS::MouseButtonID)x))
            {
                if(CurrentMouseState.buttonDown((OIS::MouseButtonID)x))
                    MouseTransitions[x] = 1;
                else
                    MouseTransitions[x] = -1;
            }
        }
    }
    
    // Is the key presently down?
    bool InputData::isDown(OIS::KeyCode code)
    {
        return KeyStates[(int)code] == 1;
    }
    
    // Is the key presently up?
    bool InputData::isUp(OIS::KeyCode code)
    {
        return KeyStates[(int)code] == 0;
    }
    
    // Has key just been pressed this last cycle?
    bool InputData::justDown(OIS::KeyCode code)
    {
        return KeyTransitions[(int)code] == 1;
    }
    
    // Has key just been released this last cycle?
    bool InputData::justUp(OIS::KeyCode code)
    {
        return KeyTransitions[(int)code] == -1;
    }
    
    // Is the key presently down?
    bool InputData::isDown(OIS::MouseButtonID button)
    {
        return CurrentMouseState.buttonDown(button);
    }
    
    // Is the key presently up?
    bool InputData::isUp(OIS::MouseButtonID button)
    {
        return !CurrentMouseState.buttonDown(button);
    }
    
    // Has key just been pressed this last cycle?
    bool InputData::justDown(OIS::MouseButtonID button)
    {
        return MouseTransitions[(int)button] == 1;
    }
    
    // Has key just been released this last cycle?
    bool InputData::justUp(OIS::MouseButtonID button)
    {
        return MouseTransitions[(int)button] == -1;
    }
}