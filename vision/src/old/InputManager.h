#pragma once
#include <OIS.h>
#include <OgreRenderWindow.h>

namespace OgreManager
{

    typedef std::map<std::string, OIS::KeyListener*> KeyListenerType;
    typedef std::map<std::string, OIS::MouseListener*> MouseListenerType;
    typedef std::map<std::string, OIS::JoyStickListener*> JoystickListenerType;

    typedef JoystickListenerType::iterator JoystickIterator;
    typedef MouseListenerType::iterator MouseIterator;
    typedef KeyListenerType::iterator KeyboardIterator;

    class InputManager : public OIS::KeyListener, OIS::MouseListener, OIS::JoyStickListener
    {
        private:
        OIS::InputManager *mInputSystem;
        
        OIS::Mouse        *mMouse;
        OIS::Keyboard     *mKeyboard;
        std::vector<OIS::JoyStick*> mJoysticks;
        
        //the various listeners
        KeyListenerType mKeyListeners;
        MouseListenerType mMouseListeners;
        JoystickListenerType mJoystickListeners;
        
        //called by the inputsystem of OIS automatically
        bool keyPressed( const OIS::KeyEvent &e );
        bool keyReleased( const OIS::KeyEvent &e );

        bool mouseMoved( const OIS::MouseEvent &e );
        bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
        bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

        bool povMoved( const OIS::JoyStickEvent &e, int pov );
        bool axisMoved( const OIS::JoyStickEvent &e, int axis );
        bool sliderMoved( const OIS::JoyStickEvent &e, int sliderID );
        bool buttonPressed( const OIS::JoyStickEvent &e, int button );
        bool buttonReleased( const OIS::JoyStickEvent &e, int button );
        
        public:
        std::vector<bool> mKeyStates;
        
        //adding methods
        void addKeyListener( OIS::KeyListener *keyListener, const std::string& instanceName );
        void addMouseListener( OIS::MouseListener *mouseListener, const std::string& instanceName );
        void addJoystickListener( OIS::JoyStickListener *joystickListener, const std::string& instanceName );

        //remove methods
        void removeKeyListener( const std::string& instanceName );
        void removeMouseListener( const std::string& instanceName );
        void removeJoystickListener( const std::string& instanceName );

        void removeKeyListener( OIS::KeyListener *keyListener );
        void removeMouseListener( OIS::MouseListener *mouseListener );
        void removeJoystickListener( OIS::JoyStickListener *joystickListener );

        void removeAllListeners();
        void removeAllKeyListeners();
        void removeAllMouseListeners();
        void removeAllJoystickListeners();
        
        public:
        virtual ~InputManager();
        InputManager(Ogre::RenderWindow *renderWindow);

        bool Initialize( Ogre::RenderWindow *renderWindow );
        void Capture( void );
        
        void setWindowExtents( int width, int height );

        OIS::Mouse*    getMouse( void );
        OIS::Keyboard* getKeyboard( void );
        OIS::JoyStick* getJoystick( unsigned int index );

        unsigned int getNumOfJoysticks();
    };
}