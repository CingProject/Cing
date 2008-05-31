#include "TryCode.h"
#include "OgreManager.h"
#include <Exception>
#include <OgreWindowEventUtilities.h>
#include "GameStateManager.h"
#include <process.h>
#include <OgreRoot.h>
#include "MainGameState.h"
#include "InputData.h"

OgreManager::OgreManager Manager;
Data::InputData InputData;
Data::GameStateManager GameStateManager;
MainGame::MainGameState MainGameState;

bool ProgramStillRunning = true;
bool InputDone = false;
bool GameEngineDone = false;


void ThreadShutdown()
{
    ProgramStillRunning = false;
}

int ProgramInitialize()
{
    if(!Manager.Initialize())
        return 1;
    
    Manager.LoadAssets();
    Manager.PrimaryWindow->SetupCamera(Manager.SceneManager);
    
    //Debug Overlay
    GameStateManager.AddState(Manager.PrimaryWindow->InitializeDebugOverlay());
    GameStateManager.AddState(&MainGameState);
    
    return 0;
}

// loop general
int OgreThread()
{
    while(ProgramStillRunning)
    {
        GameStateManager.Draw(Manager);
        
        Manager.Root->renderOneFrame();
        
        Ogre::WindowEventUtilities::messagePump();
        
        if(Manager.PrimaryWindow->RenderWindow->isClosed())
            ProgramStillRunning = false;
        
        //Yield our timeslice
        Sleep(0);
    }
    
    return 0;
}

// hilo engine
void GameEngineLoop()
{
    while(ProgramStillRunning)
    {
        GameStateManager.Update();
        Sleep(1000 / 100);
    }
}

void GameEngineThread(void *)
{
    TryCode::Execute(GameEngineLoop, ThreadShutdown, "Game Engine");
    GameEngineDone = true;
}

// hilo input
void InputLoop()
{
    while(ProgramStillRunning)
    {
        GameStateManager.HandleEvents(InputData);
        
        Manager.PrimaryWindow->InputManager->Capture();
        InputData.Update(*Manager.PrimaryWindow->InputManager);
        
        if(InputData.justDown(OIS::KC_ESCAPE))
            ProgramStillRunning = false;
        
        Sleep(1000 / 100);
    }
}

void InputThread(void *)
{
    TryCode::Execute(InputLoop, ThreadShutdown, "Input Thread");
    InputDone = true;
}

int ProgramShutdown()
{
    GameStateManager.Shutdown();
    
    Manager.Shutdown();
    return 0;
}

int main(int argc, char **argv)
{
    if(TryCode::Execute(ProgramInitialize, 0, "Program Initialize") != 0)
        return 1;
    
    uintptr_t input = _beginthread(InputThread, 0, 0);
    uintptr_t engine = _beginthread(GameEngineThread, 0, 0);
    
    TryCode::Execute(OgreThread, 0, "OgreThread");
    
    //Signal the end of the program
    ProgramStillRunning = false;
    
    //Hack, we really want to "join" the child threads.  I'm just not so sure how to do that.
    while(!GameEngineDone || !InputDone)
        Sleep(1000 / 100);
    
    TryCode::Execute(ProgramShutdown, ThreadShutdown, "Program Shutdown");
    return 0;
}

/*
 * For the windows platform, just redirect program flow into int main(int, char **)
 */
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 && !_CONSOLE
    #define WIN32_LEAN_AND_MEAN
    #include "windows.h"

    INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
    {
        //pass strCmdLine into main
        //return main(0, strCmdLine)
        
        return main(0, NULL);
    }
#endif