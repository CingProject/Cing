/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community 

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef _InputTypes_h_
#define _InputTypes_h_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "InputPrereqs.h"
#include "OISKeyboard.h"

namespace Cing
{

    // Keycode definition... for now
    // TODO see how to make this talk with Cocoa and OIS
    typedef unsigned int KeyCode;
    
    /******
     Mouse
     *******/
 	//! Button ID for mouse devices
	enum MouseButtonID
	{
		MB_Left = 0, MB_Right, MB_Middle,
		MB_Button3, MB_Button4,	MB_Button5, MB_Button6,	MB_Button7
	};
    
    
  	/** Specialised for mouse events */
	class  MouseEvent
	{
	public:
		MouseEvent( int xAbsPos, int yAbsPos ): x(xAbsPos), y(yAbsPos) {}
		virtual ~MouseEvent() {}
                
        // Mouse absolute positions
        int x, y;
	};
    
	/**
     * Base Mouse Listener class to receive mouse related events
     */
	class  MouseListener
	{
	public:
		virtual ~MouseListener() {}
		virtual bool mouseMoved( const MouseEvent &arg ) = 0;
		virtual bool mousePressed( const MouseEvent &arg, MouseButtonID id ) = 0;
		virtual bool mouseReleased( const MouseEvent &arg, MouseButtonID id ) = 0;
	};
    
    /******
     Keyboard
     *******/
    
    /*
     * Event that stores key event related information
     */
    class KeyEvent
	{
	public:
		KeyEvent(KeyCode kc, unsigned int ch): key(kc), character(ch) {}
		virtual ~KeyEvent() {}
        
		const       KeyCode key;
		unsigned int character;
	};
    
    /*
     * Base Listener for keyboard related events
     */
    class KeyListener
	{
	public:
		virtual ~KeyListener() {}
		virtual bool keyPressed(const KeyEvent &arg) = 0;
		virtual bool keyReleased(const KeyEvent &arg) = 0;
	};
    
    
///// Mouse buttons -> Defined in CommonConstants.h File
//enum MouseButton
//{
//	LEFT,
//	RIGHT,
//	CENTER
//};

} // namespace Cing

#endif // _InputTypes_h_