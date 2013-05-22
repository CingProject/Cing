/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
  Copyright (c) 2006- Julio Obelleiro and Jorge Cano

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

#ifndef _OutputDebugTextBoxCEGUI_h_
#define _OutputDebugTextBoxCEGUI_h_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "GUIPrereqs.h"

#include "OgrePlatform.h"

#include "PTypes/include/pasync.h"


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#import <CoreFoundation/CoreFoundation.h>
#endif

namespace Cing
{

/**
 * @internal
 * @brief Kind of text box that allows to output messages within the application window
 */
class OutputDebugTextBoxCEGUI
{
public:

	// Constructor / Destructor
	OutputDebugTextBoxCEGUI();
	~OutputDebugTextBoxCEGUI();

	// Init / Release / Update
	void	init 		();
	void	end			();	

	// Text control
	void	print		( const char* text );
	void	println		( const char* text );
	void	clear		();

	// Visual aspect
	void	setVisible	( bool visible );
	void	setHeight	( float pxHeight );
	void	setAlpha	( float alpha );

	void	enable		( bool value ) { m_enabled = value; }

	// Query  Methods
	bool	isValid		() { return m_bIsValid; }

	// Static const public members
	static const int		DEFAULT_TEXT_BOX_HEIGHT;	///< Height in pixels of the text box (actually internally it is a listbox)
	static const float		DEFAULT_TRANSPARENCY;		///< Default transparency of the text box: 0 means transparent, 1 means opaque
	static const int		DEFAULT_MAX_MESSAGES;		///< Max number of messages that the text box will print. If the limit is reached older messages will be deleted as needed

private:

	// Attributes
	CEGUI::Listbox* m_listBox;		///< Displays all the output messages
	bool			m_bIsValid;		///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
	bool			m_enabled;
    pt::mutex		m_mutex;


};

} // namespace Cing

#endif // _OutputDebugTextBoxCEGUI_h_
