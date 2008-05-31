#include "AbstractGameState.h"

namespace MainGame
{
    class MainGameState : public Data::AbstractGameState
    {
        public:
        virtual void Initialize();
        virtual void Shutdown();

        virtual void HandleEvents(Data::InputData &InputData);
        virtual void Update();
        virtual void Draw(::OgreManager::OgreManager &OgreManager);
    };
}