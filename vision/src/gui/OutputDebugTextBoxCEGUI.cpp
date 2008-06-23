/*
  This source file is part of the Vision project
  For the latest info, see http://www.playthemagic.com/vision

Copyright (c) 2008 Julio Obelleiro and Jorge Cano

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

#include "OutputDebugTextBoxCEGUI.h"
#include "GUIManagerCEGUI.h"

// CEGUI
#include "externLibs/Ogre3d/include/cegui/CEGUI.h"
#include "externLibs/Ogre3d/include/cegui/elements/CEGUIListbox.h"
#include "externLibs/Ogre3d/include/cegui/elements/CEGUIListboxTextItem.h"

//  Common
#include "common/CommonUtils.h"

// Framework
#include "framework/UserAppGlobals.h"

namespace GUI
{

// Static members init
const int		OutputDebugTextBoxCEGUI::DEFAULT_TEXT_BOX_HIGHT	= 150;
const float OutputDebugTextBoxCEGUI::DEFAULT_TRANSPARENCY		= 0.5f;
const int		OutputDebugTextBoxCEGUI::DEFAULT_MAX_MESSAGES		= 300;

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
OutputDebugTextBoxCEGUI::OutputDebugTextBoxCEGUI():
	m_listBox( NULL ),
	m_bIsValid  ( false )
{
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
OutputDebugTextBoxCEGUI::~OutputDebugTextBoxCEGUI()
{
	// Release resources
	end();
}

/**
 * @internal
 * @brief Creates the text box layout
 */
void OutputDebugTextBoxCEGUI::init()
{
	// Get CEGUI window manager to create new windows
	CEGUI::WindowManager&	win = CEGUI::WindowManager::getSingleton();

	// Create the listbox (will work as a text box)
	m_listBox = static_cast<CEGUI::Listbox*>(win.createWindow("Vanilla/Listbox", "Vision/OutputDebugTextBoxCEGUI"));
	m_listBox->setArea( CEGUI::URect(	cegui_absdim(0), 
																		cegui_absdim(Globals::height-DEFAULT_TEXT_BOX_HIGHT), 
																		cegui_absdim(Globals::width), 
																		cegui_absdim(Globals::height)));
	m_listBox->setShowVertScrollbar( false );
	m_listBox->setShowHorzScrollbar( false );
	m_listBox->setAlpha( DEFAULT_TRANSPARENCY );
	m_listBox->setAlwaysOnTop( true );

	// Add list box (text box) to the gui manager
	GUIManagerCEGUI::getSingleton().addGUIELement( m_listBox );

	m_bIsValid = true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void OutputDebugTextBoxCEGUI::end()
{

	m_bIsValid = false;
}

/**
 * @brief Prints a message to the text box
 *
 * @param text Text to output
 */
void OutputDebugTextBoxCEGUI::print( const char* text )
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to print to the OutputDebugTextBoxCEGUI, but it has not been initialized" );
		return;
	}

	// If this is the first item -> add a new one
	if ( m_listBox->getItemCount() == 0 )
		println( text );
	else
	{
		// Get the las item and add the text
		CEGUI::ListboxTextItem *listboxitem = static_cast< CEGUI::ListboxTextItem* >( m_listBox->getListboxItemFromIndex( m_listBox->getItemCount()-1 ) );

		// Add the text
		listboxitem->setText( listboxitem->getText() + text );
	}

	// Make sure the new item is visible
	m_listBox->ensureItemIsVisible( m_listBox->getItemCount() );
}

/**
 * @brief Prints a message to the text box (it will be a complete line, this is, next text printed will go
 * to the next line)
 *
 * @param text Text to output
 */
void OutputDebugTextBoxCEGUI::println( const char* text )
{
	if ( !isValid() )
		return;

	// Check the message limit
	if ( m_listBox->getItemCount() >= DEFAULT_MAX_MESSAGES )
		m_listBox->removeItem( m_listBox->getListboxItemFromIndex( 0 ) );

	// Create a new list box item to print the text
	CEGUI::ListboxTextItem* listboxitem = new CEGUI::ListboxTextItem ( text );
	listboxitem->setAutoDeleted( true );
	m_listBox->addItem( listboxitem );

	// Make sure the new item is visible
	m_listBox->ensureItemIsVisible( m_listBox->getItemCount() );
}

/**
 * @brief Clears all the messages that the list box contains
 */
void OutputDebugTextBoxCEGUI::clear()
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to clear the OutputDebugTextBoxCEGUI, but it has not been initialized" );
		return;
	}

	m_listBox->resetList();
}

/**
 * @brief Makes the text box visible or invisible
 *
 * @param visible if true, the text box will be visible, if false, it will become invisible, although
 * it will continue storing items
 */
void OutputDebugTextBoxCEGUI::setVisible( bool visible )
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to set visible the OutputDebugTextBoxCEGUI, but it has not been initialized" );
		return;
	}

	m_listBox->setVisible( visible );
}

/**
 * @brief Sets the hight of the text box
 *
 * @param pxHight hight in pixels of the text box
 */
void OutputDebugTextBoxCEGUI::setHight( int pxHight )
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to set visible the OutputDebugTextBoxCEGUI, but it has not been initialized" );
		return;
	}

	m_listBox->setHeight( CEGUI::UDim( 0, pxHight ) );
}

/**
 * @brief Sets the alpha (transparency) of the text box. 0 means transparent, 1 opaque
 *
 * @param alpha transparency of the text box. Range: 0..1
 */
void OutputDebugTextBoxCEGUI::setAlpha( float alpha )
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to set visible the OutputDebugTextBoxCEGUI, but it has not been initialized" );
		return;
	}

	m_listBox->setAlpha( alpha );
}

} // namespace GUI