#include "TryCode.h"
#include <OgreErrorDialog.h>
#include <OgreException.h>
#include <Exception>

int TryCode::Execute(int (*CodeToExecute)(void), void (*CodeToShutdown)(void), char *Name)
{
    try
    {
        return (*CodeToExecute)();
    }
    catch( Ogre::Exception &e )
    {
        static Ogre::ErrorDialog * dlg = new Ogre::ErrorDialog();
        std::cout << "Ogre error in " + Ogre::String(Name) + ".\n" + e.getFullDescription() << std::endl;
        dlg->display("Ogre error in " + Ogre::String(Name) + ".\n" + e.getFullDescription());
        
        if(CodeToShutdown)
            CodeToShutdown();
            
        return 1;
    }
    
    catch(std::exception e)
    {
        static Ogre::ErrorDialog * dlg = new Ogre::ErrorDialog();
        std::cout << "Standard error in " + Ogre::String(Name) + ".\n" + e.what() << std::endl;
        dlg->display("Standard error in " + Ogre::String(Name) + ".\n" + e.what());
        
        if(CodeToShutdown)
            CodeToShutdown();
            
        return 1;
    }
    
    catch(...)
    {
        static Ogre::ErrorDialog * dlg = new Ogre::ErrorDialog();
        std::cout << "Unkown error thrown.\n";
        dlg->display("Unkown error thrown.\n");
        
        if(CodeToShutdown)
            CodeToShutdown();
            
        return 1;
    }
    
    if(CodeToShutdown)
        CodeToShutdown();
    
    return 0;
}

void TryCode::Execute(void (*CodeToExecute)(void), void (*CodeToShutdown)(void), char *Name)
{
    try
    {
        (*CodeToExecute)();
    }
    catch( Ogre::Exception &e )
    {
        static Ogre::ErrorDialog * dlg = new Ogre::ErrorDialog();
        std::cout << "Ogre error in " + Ogre::String(Name) + ".\n" + e.getFullDescription() << std::endl;
        dlg->display("Ogre error in " + Ogre::String(Name) + ".\n" + e.getFullDescription());
    }
    
    catch(std::exception e)
    {
        static Ogre::ErrorDialog * dlg = new Ogre::ErrorDialog();
        std::cout << "Standard error in " + Ogre::String(Name) + ".\n" + e.what() << std::endl;
        dlg->display("Standard error in " + Ogre::String(Name) + ".\n" + e.what());
    }
    
    catch(...)
    {
        static Ogre::ErrorDialog * dlg = new Ogre::ErrorDialog();
        std::cout << "Unkown error thrown.\n";
        dlg->display("Unkown error thrown.\n");
    }
    
    if(CodeToShutdown)
        CodeToShutdown();
}