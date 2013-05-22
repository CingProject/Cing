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

// Precompiled headers
#include "Cing-Precompiled.h"

#include "OutputDebugTextBoxCEGUI.h"
#include "GUIManagerCEGUI.h"

// CEGUI
#include "CEGUI.h"
#include "elements/CEGUIListbox.h"
#include "elements/CEGUIListboxTextItem.h"

//  Common
#include "common/CommonUtilsIncludes.h"

// Framework
#include "framework/UserAppGlobals.h"

namespace Cing
{

// Static members init
const int		OutputDebugTextBoxCEGUI::DEFAULT_TEXT_BOX_HEIGHT		= 150;
const float		OutputDebugTextBoxCEGUI::DEFAULT_TRANSPARENCY		= 0.5f;
const int		OutputDebugTextBoxCEGUI::DEFAULT_MAX_MESSAGES		= 100;

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
OutputDebugTextBoxCEGUI::OutputDebugTextBoxCEGUI():
	m_listBox	( NULL ),
	m_bIsValid  ( false ),
	m_enabled	( true )
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
										cegui_absdim((float)(height-DEFAULT_TEXT_BOX_HEIGHT)), 
										cegui_absdim((float)width), 
										cegui_absdim((float)height)));
	m_listBox->setShowVertScrollbar( false );
	m_listBox->setShowHorzScrollbar( false );
	m_listBox->setAlpha( DEFAULT_TRANSPARENCY );
	m_listBox->setAlwaysOnTop( true );

	// Add list box (text box) to the gui manager
	GUIManagerCEGUI::getSingleton().addGUIELement( m_listBox );

	// The text box is valid from now on
	m_bIsValid	= true;


	// But by default we want it non visible and disabled by default
	setVisible(false);
	m_enabled	= false;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void OutputDebugTextBoxCEGUI::end()
{

	m_bIsValid = false;
	m_enabled = false;
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

	// Thread safe
    pt::scopelock lock( m_mutex );

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

	if ( !m_enabled )
		return;

	// Thread safe
    pt::scopelock lock( m_mutex );


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

	if ( !m_enabled )
		return;


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
		LOG_ERROR( "Trying to set visible the OutputDebugTextBoxCEGUI, but it has not been initialized. The function showDebugOutput cannot be called in the setup()" );
		return;
	}

	if ( !m_enabled )
		return;

	m_listBox->setVisible( visible );
}

/**
 * @brief Sets the height of the text box
 *
 * @param pxHeight height in pixels of the text box
 */
void OutputDebugTextBoxCEGUI::setHeight( float pxHeight )
{
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to set the OutputDebugTextBoxCEGUI height, but it has not been initialized" );
		return;
	}

	if ( !m_enabled )
		return;

	m_listBox->setHeight( CEGUI::UDim( 0, pxHeight ) );
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
		LOG_ERROR( "Trying to set the OutputDebugTextBoxCEGUI height, but it has not been initialized" );
		return;
	}

	if ( !m_enabled )
		return;

	m_listBox->setAlpha( alpha );
}

} // namespace Cing