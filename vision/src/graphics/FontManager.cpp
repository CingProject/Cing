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
Inc.
*/

#include "FontManager.h"
#include "GraphicsManager.h"
#include "common/LogManager.h"
#include "common/Release.h"


// TEST
#include "Ogre3d/include/OgreTechnique.h"
#include "Ogre3d/include/OgreTextureUnitState.h"
#include "Ogre3d/include/OgreSceneManager.h"


namespace Graphics
{


	/**
	* @brief Constructor
	*/
	FontManager::FontManager(): m_bIsValid( false ), m_activeFont( NULL ), m_nextFontToUse( 0 )
	{
	}

	/**
	* @brief Destructor
	*/
	FontManager::~FontManager()
	{
		end();
	}

	/**
	* @brief Loads all resources FontManager needs
	* @return true if the initialization was ok | false otherwise
	*/
	bool FontManager::init()
	{
		// Load the default font
		m_defaultFont.load();

		// Set active font to the default (until the user sets one)
		m_activeFont = &m_defaultFont;

		// Reset count of next font to use from the buffer
		m_nextFontToUse = 0;

		// Now the manager is valid
		m_bIsValid = true;

		return true;
	}

	/**
	* @brief Releases all resources FontManager needs
	*/
	void FontManager::end()
	{
		// Release the fonts
		std::for_each( m_activeFontsToRender.begin(), m_activeFontsToRender.end(), Common::ReleaseFunctor<MovableText>() );
		m_activeFontsToRender.clear();

		m_bIsValid = false;
	}

	/**
	* @brief Draws all the fonts for the current frame
	*/
	void FontManager::postRender()
	{
		// Check the manager is ok
		if ( !isValid() )
		{
			LOG_ERROR( "FontManager not properly initialized" );
			return;
		}

		// set invisible all text areas (but store them to reuse in future frames)
		std::for_each( m_activeFontsToRender.begin(), m_activeFontsToRender.end(), std::bind2nd( std::mem_fun( &MovableText::show ), false ) );

		// We can reuse all used fonts next frame
		m_nextFontToUse = 0;
	}

	/**
	* @brief Sets the current active font
	*/
	void FontManager::setActiveFont( const Font& font )
	{
		m_activeFont = &font;
		m_activeFontProperties.fontName	= font.getFontName();
	}


	/**
	* @brief Add a text to be render queue with the current properties set by user calls
	*/
	void FontManager::addText()
	{
		// Check the manager is ok
		if ( !isValid() )
		{
			LOG_ERROR( "text error: FontManager not properly initialized. The init method has not been called or there no active font" );
			return;
		}

		// Add a new font (to be rendered later)
		//m_activeFontsToRender.push_back( TextArea() );
		MovableText* newText = getNewText();

		// Set properties for the new text
		newText->init();

		// Font properties
		newText->setFontName( m_activeFontProperties.fontName );
		newText->setCharacterHeight( m_activeFontProperties.size );
		newText->setColor( GraphicsManager::getSingleton().getFillColor().normalized() );
		newText->setTextAreaWidth( m_activeFontProperties.width );
		newText->setTextAreaHeight( m_activeFontProperties.height );
		newText->setTextAlignment(m_activeFontProperties.halign, m_activeFontProperties.valign);
		newText->setCaption( m_activeFontProperties.text.toChar() );

		// Text display properties
		newText->showOnTop();
		newText->setAlwaysFaceCamera( false );
		newText->show();

		// Text transformation properties
		Transform &t = Graphics::GraphicsManager::getSingleton().m_transforms.top();
		newText->setGlobalTranslation(  t.getPosition() );
		newText->setRotation( t.getRotQuaternion() );
		newText->setScale( t.getScale() );

		// Set text area local transformation (transformation passed to the text function)
		newText->setGlobalTranslation( Vector( m_activeFontProperties.x, m_activeFontProperties.y, m_activeFontProperties.z ) );

		// Setup text geometry for render
		newText->_setupGeometry();
	}


	/**
	* @brief Retunrs a new text (actually it tries to reuse a text from previous frames, but if all are used or
	* there is no text created, it creates a new one).
	*/
	MovableText* FontManager::getNewText()
	{
		// Can we reuse a font?
		if ( m_nextFontToUse < m_activeFontsToRender.size() )
			return m_activeFontsToRender[ m_nextFontToUse++ ];

		// No text available to reuse -> create a new one
		m_activeFontsToRender.push_back( new MovableText() );
		m_nextFontToUse++;

		// Make new text area child of the root node (so we dont create so many nodes)
		Globals::ogreSceneManager->getRootSceneNode()->attachObject( m_activeFontsToRender.back() );

		// Return it
		return m_activeFontsToRender.back();
	}


} // namespace
