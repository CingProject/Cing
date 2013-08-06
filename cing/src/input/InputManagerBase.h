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

#pragma once

namespace Cing
{
    // forward declarations
    class MouseBase;
    class KeyboardBase;
    
/*
 * Base class for all input managers. Not platform or system specific, just provides interaface for manager implementations (OIS, Cocoa, etc)
 */
class InputManagerBase
{
public:
    InputManagerBase            (): m_isValid(false) {}
    virtual ~InputManagerBase   () { end(); }
    
    // Lifecycle methods
    virtual bool init           () = 0;
    virtual void update         () {}
    virtual void end            () {}
    
    // Query methods
	bool            isValid         () const    { return m_isValid; }
    MouseBase&      getMouse        ()          { return *m_mouse;    }
    KeyboardBase&   getKeyboard     ()          { return *m_keyboard; }
    
    
protected:
    MouseBase*      m_mouse;
    KeyboardBase*   m_keyboard;
    bool            m_isValid; ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
};
} // namespace Cing
