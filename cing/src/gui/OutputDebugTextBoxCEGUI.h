/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

  Copyright (c) 2006-2009 Julio Obelleiro and Jorge Cano

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef _OutputDebugTextBoxCEGUI_h_
#define _OutputDebugTextBoxCEGUI_h_

#include "GUIPrereqs.h"

#include "OgrePlatform.h"


#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#error "mac"
//#import <CoreFoundation/CoreFoundation.h>
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

};

} // namespace Cing

#endif // _OutputDebugTextBoxCEGUI_h_
