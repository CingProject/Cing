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
Inc.
*/

// Precompiled headers
#include "Cing-Precompiled.h"

#include "FontManager.h"
#include "GraphicsManager.h"
#include "common/LogManager.h"
#include "common/Release.h"


// TEST
#include "OgreTechnique.h"
#include "OgreTextureUnitState.h"
#include "OgreSceneManager.h"


namespace Cing
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
		// Reset default falues for active font properties (used in the next text call)
		m_activeFontProperties.reset();

		// Load the default font
		m_defaultFont.load( Font::DEFAULT_FONT_NAME );

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
        if ( m_activeFontsToRender.size() > 0 )
            std::for_each( m_activeFontsToRender.begin(), m_activeFontsToRender.end(), ReleaseFunctor<MovableText>() );
		m_activeFontsToRender.clear();
		m_activeFont = NULL;

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
	void FontManager::setActiveFont( Font& font )
	{
		m_activeFont = &font;
		m_activeFontProperties.fontName	= font.getFontName();
		updateActiveFontProperties();
	}


	/**
	* @brief Add a text to be render queue with the current properties set by user calls
	*/
	void FontManager::addText()
	{
		// Check the manager is ok
		if ( !isValid() )
		{
			LOG_ERROR( "text error: FontManager not properly initialized. The init method has not been called or there are no active font" );
			return;
		}

		// Add a new font (to be rendered later)
		MovableText* newText = getNewText();

		// Font properties
		newText->setFontName( m_activeFontProperties.fontName );
		newText->setCharacterHeight( m_activeFontProperties.size );
		newText->setLeading( m_activeFontProperties.leading );
		newText->setColor( GraphicsManager::getSingleton().getFillColor().normalized() );
		newText->setTextAreaWidth( m_activeFontProperties.width );
		newText->setTextAreaHeight( m_activeFontProperties.height );
		newText->setTextAlignment(m_activeFontProperties.halign, m_activeFontProperties.valign);
		newText->setCaption( m_activeFontProperties.text.toUTF() );
		newText->setWordWrap( m_activeFontProperties.wordWrap );

		// Text display properties
		newText->showOnTop();
		newText->setAlwaysFaceCamera( false );
		newText->show();

		// Get global transformation
		Transform &t = GraphicsManager::getSingleton().m_transforms.top();

		// Font Position
		Vector fontPos( m_activeFontProperties.x, m_activeFontProperties.y, m_activeFontProperties.z );
		Vector transformedPos = t.getPosition() + fontPos;
		Vector scale = t.getScale();

		// Control current coordinate system
		if ( GraphicsManager::getSingleton().isProcessingMode() )
		{
			if ( m_activeFontProperties.render2d )
			{
				newText->setPosition( transformedPos.x, height - transformedPos.y );	
				newText->setScale( scale.x, -scale.y );
			}
			else
			{
				newText->setPosition( transformedPos );	
				newText->setScale( scale.x, -scale.y, scale.z );
			}
			
			newText->getParentNode()->setOrientation( t.getRotQuaternion().Inverse() );
		}
		else
		{
			if ( m_activeFontProperties.render2d )
			{
				newText->setPosition( transformedPos.x, transformedPos.y );	
				newText->setScale( scale.x, scale.y );
			}
			else
			{
				newText->setPosition( transformedPos );	
				newText->setScale( scale );
			}

			newText->getParentNode()->setOrientation( t.getRotQuaternion() );
		}

		// Setup text geometry for render
		newText->_setupGeometry();
	}

	/* 
	 * @internal
	 * Updates the previously set font properties into the active Ogre font
	 */
	void FontManager::updateActiveFontProperties()
	{
		// Check we have a font
		if ( !isValid() )
		{
			LOG_ERROR( "FontManager::updateActiveFontProperties. FontManager is not Valid or not Initialized. Maybe there is no active font set? Call textFont to set the active font" );
			return;
		}
			
		m_activeFont->updateFontAttributes( m_activeFontProperties );
	}


	/* 
	 * Calculates and returns the width of any character or text string, considering the active font and it's properties
	 * 
	 * @param text Text to analyze for it's pixel length
	 * @return the width of the received text in pixels
	 */ 
	float FontManager::textWidth( const std::string& text )
	{	
		// Check we have a font
		if ( !isValid() )
		{
			LOG_ERROR( "FontManager::textWidth. FontManager is not Valid or not Initialized. Maybe there is no active font set? Call textFont to set the active font" );
			return 0.0f;
		}

		// Empty text case
		if ( text == "" )
			return 0.0f;
			
		// Check valid viewport
		if ( !GraphicsManager::getSingleton().getMainWindow().getMainViewport() )
		{
			LOG_ERROR( "FontManager::textWidth. General Error. There is no main viewport created?" );
			return 0.0f;
		}

		//int vpWidth		= GraphicsManager::getSingleton().getMainWindow().getMainViewport()->getActualWidth();
		//int vpHeight	= GraphicsManager::getSingleton().getMainWindow().getMainViewport()->getActualHeight();
		//float viewportAspectCoef = (float)vpHeight/(float)vpWidth;
			
		float textWidth = 0;
		float fontSize = m_activeFont->getFontSize();

		// Calculate the text width
		for(unsigned int i=0; i < text.length();i++) 
		{   
			char c = text.at(i);
			// If it's a space, use the zero character length
			if (MovableText::IsSpace( c ) )
				//textWidth += m_activeFont->getOgreFont()->getGlyphAspectRatio(UNICODE_SPACE);
				textWidth += m_activeFont->getOgreFont()->getGlyphAspectRatio(UNICODE_ZERO) / 2.0f; // the 2.0f is to compensate the * 2.0f that comes later (as it is not used to draw the spaces)
			else
				textWidth += m_activeFont->getOgreFont()->getGlyphAspectRatio(text.at(i));
		}
		
		//float ttfSize		= m_activeFont->getOgreFont()->getTrueTypeSize();
		//float ttfResolution = (float)m_activeFont->getOgreFont()->getTrueTypeResolution();
		//float textWidth2 = textWidth * ttfSize * ttfResolution / 72.0f;
		textWidth *= fontSize * 2.0f /** viewportAspectCoef*/;
		
		return textWidth;
	}

	float FontManager::textHeight( const std::string& text, float textBoxWidth )
	{
		// Check we have a font
		if ( !isValid() )
		{
			LOG_ERROR( "FontManager::textWidth. FontManager is not Valid or not Initialized. Maybe there is no active font set? Call textFont to set the active font" );
			return 0.0f;
		}

		// Check empty text case
		if ( text == "" )
			return 0.0f;

		// TODO: this is not the ideal way to calculate this (it should just be on the font class, but for now)
		MovableText* newText = getNewText();

		// Font properties
		newText->setFontName( m_activeFontProperties.fontName );
		newText->setCharacterHeight( m_activeFontProperties.size );
		newText->setLeading( m_activeFontProperties.leading );
		newText->setColor( GraphicsManager::getSingleton().getFillColor().normalized() );
		newText->setTextAreaWidth( m_activeFontProperties.width );
		newText->setTextAreaHeight( m_activeFontProperties.height );
		newText->setTextAlignment(m_activeFontProperties.halign, m_activeFontProperties.valign);
		newText->setCaption( m_activeFontProperties.text.toChar() );

		// Get the height
		float textHeigh = newText->getTextBlockHeightPixels( text, textBoxWidth );

		// Make the text invisible again just in case (This is related with the TODO, it should be calculated by the Font class)
		newText->show(false);
		
		return textHeigh;
	}


	float FontManager::characterHeight()
	{
		// Check we have a font
		if ( !isValid() )
		{
			LOG_ERROR( "FontManager::textWidth. FontManager is not Valid or not Initialized. Maybe there is no active font set? Call textFont to set the active font" );
			return 0.0f;
		}
		return 0.0f;
	}

	float FontManager::spaceWidth()
	{
		// Check we have a font
		if ( !isValid() )
		{
			LOG_ERROR( "FontManager::textWidth. FontManager is not Valid or not Initialized. Maybe there is no active font set? Call textFont to set the active font" );
			return 0.0f;
		}

		return 0.0f;
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

		// Using root scene node as parent node
		//ogreSceneManager->getRootSceneNode()->attachObject( m_activeFontsToRender.back() );

		// Using individual scene nodes
		m_activeFontsToRender.back()->init( ogreSceneManager->getRootSceneNode()->createChildSceneNode() );

		// Return it
		return m_activeFontsToRender.back();
	}

   /**
	* @brief Split a text area into lines. Returns a vector of strings.
	*/
	std::vector<std::string> FontManager::splitInLines( const std::string& str, float textBoxWidth )
	{
		std::vector<std::string>	lines;

		std::string					tempLine	= "";
		std::string					currentChar = "";
		std::string					word		= "";
		float						wordWidth	= 0;
		float						lineWidth	= 0;

		for (int i = 0; i < (int)str.size() ; i++)
		{
			currentChar = str.substr( i, 1 );

			// Split text into words			
			if ( currentChar == " " )
			{

				wordWidth = textWidth( word );
				lineWidth = textWidth( tempLine );

				// add space before calculate width
				word += currentChar;

				// Calculate if current line + new word exceeds max width
				if ( ( lineWidth + wordWidth ) - 10 > textBoxWidth )
				{
					// Store current line and add current word to next one
					lines.push_back( tempLine );
					tempLine = word;
				}else{

					// Add this word to current line
					tempLine += word;
				}
				word = "";
			}
			else
				word += currentChar;

			// If this is the last character / line, add it
			if ( i == ((int)str.size()-1) )
			{
				tempLine += word;
				lines.push_back( tempLine );
			}
		}

		return lines;
	}

} // namespace
