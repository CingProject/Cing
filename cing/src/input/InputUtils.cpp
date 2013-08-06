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

#include "InputUtils.h"

namespace Cing
{
    // Converts an OIS mouse event to Cing Mouse event
    MouseEvent      toCing( const OIS::MouseEvent &oisEvent )
    {
        MouseEvent mevent(oisEvent.state.X.abs, oisEvent.state.Y.abs);
        return mevent;
    }
    
    // Converts an OIS Mouse button id to Cing button id
    MouseButtonID   toCing( OIS::MouseButtonID oisButtonId )
    {
        return (MouseButtonID)oisButtonId;
    }
    
    // Converts an OIS keyboard event to Cing Keyboard event
    KeyEvent toCing( const OIS::KeyEvent &oisEvent )
    {
        KeyEvent kevent(oisEvent.key, oisEvent.text);
        return kevent;
    }
    
} // namespace Cing