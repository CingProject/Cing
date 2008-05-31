#include <list>
#include "AbstractGameState.h"

namespace Data
{
    class GameStateManager
    {
        std::list<AbstractGameState *> GameStates;
        
        public:
        void AddState(AbstractGameState *state);        
        void RemoveState(AbstractGameState *state);        
        void HandleEvents(InputData &InputData);        
        void Update();        
        void Draw(::OgreManager::OgreManager &OgreManager);
        void Shutdown();
    };
}
    