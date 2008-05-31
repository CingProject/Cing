#include "GameStateManager.h"

namespace Data
{
    void GameStateManager::AddState(AbstractGameState *state)
    {
        state->Initialize();
        GameStates.push_back(state);
    }
    
    void GameStateManager::RemoveState(AbstractGameState *state)
    {
        state->Shutdown();
        GameStates.remove(state);
    }
    
    void GameStateManager::HandleEvents(InputData &InputData)
    {
        for(std::list<AbstractGameState *>::iterator Iter = GameStates.begin(); Iter != GameStates.end(); ++Iter)
        {
            (*Iter)->HandleEvents(InputData);
        }
    }
    
    void GameStateManager::Update()
    {
        for(std::list<AbstractGameState *>::iterator Iter = GameStates.begin(); Iter != GameStates.end(); ++Iter)
        {
            (*Iter)->Update();
        }
    }
    
    void GameStateManager::Draw(::OgreManager::OgreManager &OgreManager)
    {
        for(std::list<AbstractGameState *>::iterator Iter = GameStates.begin(); Iter != GameStates.end(); ++Iter)
        {
            (*Iter)->Draw(OgreManager);
        }
    }
    
    void GameStateManager::Shutdown()
    {
        for(std::list<AbstractGameState *>::iterator Iter = GameStates.begin(); Iter != GameStates.end(); ++Iter)
        {
            (*Iter)->Shutdown();
        }    
    }
}
    