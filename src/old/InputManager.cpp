#include "InputManager.h"
#include <iostream>
#include <map>
#include <vector>

using namespace std;

namespace OgreManager
{
    //these macros are used to limit the amount of copy-paste code,
    //as well as make what's going on more apparent
    #define CURRENT curr

    #define FORALLJOYSTICKLISTENERS for(JoystickIterator curr = mJoystickListeners.begin(); \
                                curr != mJoystickListeners.end(); ++curr)


    #define FORALLMICELISTENERS for(MouseIterator curr = mMouseListeners.begin(); \
                           curr != mMouseListeners.end(); ++curr)


    #define FORALLKEYBOARDSLISTENERS for(KeyboardIterator curr = mKeyListeners.begin(); \
                           curr != mKeyListeners.end(); ++curr)

    InputManager::InputManager(Ogre::RenderWindow *renderWindow) : 
        mMouse(NULL),
        mKeyboard(NULL),
        mInputSystem(NULL),
        mKeyStates(0xFF, false)//256 key codes possible, I imagine
    {   
        Initialize(renderWindow);
    }

    InputManager::~InputManager()
    {
        if( mInputSystem )
        {
            if( mMouse )
            {
                mInputSystem->destroyInputObject( mMouse );
                mMouse = NULL;
            }

            if( mKeyboard )
            {
                mInputSystem->destroyInputObject( mKeyboard );
                mKeyboard = NULL;
            }

            if( mJoysticks.size() > 0 )
            {
                for(vector<OIS::JoyStick*>::iterator curr = mJoysticks.begin();
                    curr != mJoysticks.end(); ++curr)
                {
                    mInputSystem->destroyInputObject( *curr );
                }

                mJoysticks.clear();
            }
            mInputSystem->destroyInputSystem(this->mInputSystem);
            mInputSystem = NULL;

            // Clear Listeners
            mKeyListeners.clear();
            mMouseListeners.clear();
            mJoystickListeners.clear();
        }
    }

    bool InputManager::Initialize( Ogre::RenderWindow *renderWindow )
    {
        if( mInputSystem )
            return false; //already setup

        // Setup basic variables
        OIS::ParamList paramList;
        size_t windowHnd = 0;
        std::ostringstream windowHndStr;

        // Get window handle
        #if defined OIS_WIN32_PLATFORM
        {
            renderWindow->getCustomAttribute( "WINDOW", &windowHnd );
            
            paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND") ));
            paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE") ));
            
            // Uncomment these two lines to allow users to switch keyboards via the language bar
            //paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND") ));
            //paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE") ));
        }
        #elif defined OIS_LINUX_PLATFORM
        {
            paramList.insert( std::make_pair(std::string("x11_mouse_grab"), std::string("false") );
            renderWindow->getCustomAttribute( "WINDOW", &windowHnd );
        }
        #endif
        
        assert(windowHnd != 0 && "Invalid window handle");

        // Fill parameter list
        windowHndStr << (unsigned int) windowHnd;
        paramList.insert( std::make_pair( std::string( "WINDOW" ), windowHndStr.str() ) );    

        // Create inputsystem
        mInputSystem = OIS::InputManager::createInputSystem( paramList );

        // If possible create a buffered keyboard
        if( mInputSystem->numKeyboards() > 0 )
        {
            mKeyboard = static_cast<OIS::Keyboard*>( mInputSystem->createInputObject( OIS::OISKeyboard, true ) );
            mKeyboard->setEventCallback( this );
        }

        // If possible create a buffered mouse
        if( mInputSystem->numMice() > 0 )
        {
            mMouse = static_cast<OIS::Mouse*>( mInputSystem->createInputObject( OIS::OISMouse, true ) );
            mMouse->setEventCallback( this );

            // Get window size
            unsigned int width, height, depth;
            int left, top;
            renderWindow->getMetrics( width, height, depth, left, top );

            // Set mouse region
            this->setWindowExtents( width, height );
        }

        // If possible create all joysticks in buffered mode
        if( mInputSystem->numJoySticks() > 0 )
        {
            // If possible create all joysticks in buffered mode
            if( mInputSystem->numJoySticks() > 0 ) {
                mJoysticks.resize( mInputSystem->numJoySticks() );

                for(vector<OIS::JoyStick*>::iterator curr = mJoysticks.begin();
                    curr != mJoysticks.end(); ++curr)
                {
                    (*curr) = static_cast<OIS::JoyStick*>( mInputSystem->createInputObject( OIS::OISJoyStick, true ) );
                    (*curr)->setEventCallback( this );
                }
            }
        }
        
        return true;
    }

    void InputManager::Capture( void )
    {
        // Need to capture / update each device every frame
        if( mMouse ) mMouse->capture();
        if( mKeyboard) mKeyboard->capture();
        if( mJoysticks.size() > 0 ) {
            for(vector<OIS::JoyStick*>::iterator curr = mJoysticks.begin();
                curr != mJoysticks.end(); ++curr)
            {
                (*curr)->capture();
            }
        }
    }

    //add listeners
    void InputManager::addKeyListener( OIS::KeyListener *keyListener, const std::string& instanceName)
    {
        if( mKeyboard )
        {
            // Check for duplicate items
            KeyListenerType::iterator curr = mKeyListeners.find( instanceName );
            if(curr == mKeyListeners.end() )
                mKeyListeners[ instanceName ] = keyListener;
        }
    }

    void InputManager::addMouseListener( OIS::MouseListener *mouseListener, const std::string& instanceName )
    {
        if( mMouse )
        {
            // Check for duplicate items
            MouseListenerType::iterator curr = mMouseListeners.find( instanceName );
            if(curr == mMouseListeners.end())
                mMouseListeners[ instanceName ] = mouseListener;
        }
    }

    void InputManager::addJoystickListener( OIS::JoyStickListener *joystickListener, const string& instanceName )
    {
        if( mJoysticks.size() > 0 )
        {
            // Check for duplicate items
            JoystickListenerType::iterator curr = mJoystickListeners.find( instanceName );
            if(curr == mJoystickListeners.end() )
                mJoystickListeners[ instanceName ] = joystickListener;
        }
    }

    //remove listeners
    void InputManager::removeKeyListener(const string &instanceName)
    {
        // Check if item exists
        KeyboardIterator curr = mKeyListeners.find(instanceName );
        if(curr != mKeyListeners.end()) mKeyListeners.erase(curr);
    }

    void InputManager::removeMouseListener( const std::string& instanceName ) {
        // Check if item exists
        MouseIterator curr = mMouseListeners.find( instanceName );
        if(curr != mMouseListeners.end()) mMouseListeners.erase(curr);
    }

    void InputManager::removeJoystickListener( const std::string& Name )
    {
        // Check if item exists
        JoystickIterator curr = mJoystickListeners.find(Name);
        if(curr != mJoystickListeners.end()) mJoystickListeners.erase(curr);
    }
    void InputManager::removeKeyListener( OIS::KeyListener *deleteme )
    {
        FORALLKEYBOARDSLISTENERS if(CURRENT->second == deleteme)
            mKeyListeners.erase(curr);
    }

    void InputManager::removeMouseListener( OIS::MouseListener *deleteme )
    {
        FORALLMICELISTENERS if(CURRENT->second == deleteme)
            mMouseListeners.erase(curr);
    }

    void InputManager::removeJoystickListener( OIS::JoyStickListener *deleteme )
    {
        FORALLJOYSTICKLISTENERS if(CURRENT->second == deleteme)
            mJoystickListeners.erase(curr);
    }

    void InputManager::removeAllListeners( void )
    {
        removeAllKeyListeners();
        removeAllMouseListeners();
        removeAllJoystickListeners();
    }

    void InputManager::removeAllKeyListeners()
    {
        mKeyListeners.clear();
    }

    void InputManager::removeAllMouseListeners()
    {
        mMouseListeners.clear();
    }

    void InputManager::removeAllJoystickListeners()
    {
        mJoystickListeners.clear();
    }

    //change mouse bounds 
    void InputManager::setWindowExtents( int width, int height )
    {
        // Set mouse region (if window resizes, we should alter this to reflect as well)
        const OIS::MouseState &mouseState = mMouse->getMouseState();
        mouseState.width  = width;
        mouseState.height = height;
    }

    OIS::Mouse* InputManager::getMouse()
    {
        return mMouse;
    }

    OIS::Keyboard* InputManager::getKeyboard()
    {
        return mKeyboard;
    }

    OIS::JoyStick* InputManager::getJoystick( unsigned int index )
    {
        // Make sure it's a valid index
        if( index < mJoysticks.size() )
            return mJoysticks[ index ];

        return 0;
    }

    //keyboard
    bool InputManager::keyPressed( const OIS::KeyEvent &e )
    {
        mKeyStates[e.key] = true;
        FORALLKEYBOARDSLISTENERS CURRENT->second->keyPressed( e );
        
        return true;
    }

    bool InputManager::keyReleased( const OIS::KeyEvent &e ) 
    {
        mKeyStates[e.key] = false;
        FORALLKEYBOARDSLISTENERS CURRENT->second->keyReleased( e );

        return true;
    }

    //mice
    bool InputManager::mouseMoved( const OIS::MouseEvent &e )
    {
        FORALLMICELISTENERS CURRENT->second->mouseMoved(e);

        return true;
    }

    bool InputManager::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
    {
        FORALLMICELISTENERS CURRENT->second->mousePressed(e, id);

        return true;
    }

    bool InputManager::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
    {
        FORALLMICELISTENERS CURRENT->second->mouseReleased(e, id);

        return true;
    }

    //Joysticks
    bool InputManager::povMoved( const OIS::JoyStickEvent &e, int pov )
    {
        FORALLJOYSTICKLISTENERS CURRENT->second->povMoved( e, pov );
        
        return true;
    }

    bool InputManager::axisMoved( const OIS::JoyStickEvent &e, int axis )
    {    
        FORALLJOYSTICKLISTENERS CURRENT->second->axisMoved( e, axis );

        return true;
    }

    bool InputManager::sliderMoved( const OIS::JoyStickEvent &e, int sliderID )
    {
        FORALLJOYSTICKLISTENERS CURRENT->second->sliderMoved(e, sliderID);

        return true;
    }


    bool InputManager::buttonPressed( const OIS::JoyStickEvent &e, int button )
    {
        FORALLJOYSTICKLISTENERS CURRENT->second->buttonPressed(e, button);

        return true;
    }

    bool InputManager::buttonReleased( const OIS::JoyStickEvent &e, int button )
    {
        FORALLJOYSTICKLISTENERS CURRENT->second->buttonReleased(e, button);

        return true;
    }

    unsigned int InputManager::getNumOfJoysticks()
    {
        return (unsigned int) this->mJoysticks.size();
    }
}