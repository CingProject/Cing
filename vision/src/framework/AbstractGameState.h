#pragma once

#include <iostream>
#include <string>
#include "OgreManagerPrereqs.h"

namespace Data
{
class InputData;

    //The various game states inherit from the abstract game state
    //The AGS class needs to be able to push other states onto the
    //game state stack, pop itself off the stack, and transfer 
    //control to other states
    class AbstractGameState
    {
        public:
        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

        virtual void HandleEvents(Data::InputData &InputData) = 0; //Handle events from input thread
        virtual void Update() = 0; //Core game logic
        virtual void Draw(OgreManager::OgreManager &OgreManager) = 0; //Handle Ogre's thread, tell OgreManager what to draw, what has moved, etc.
    };
}