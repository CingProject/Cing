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

#include "TextArea.h"
#include "Font.h"
#include "Color.h"

// Framework
#include "framework/UserAppGlobals.h"

// OGRE
#include "OgreOverlayManager.h"
#include "OgreOverlayContainer.h"
#include "OgreStringConverter.h"
#include "OgreTextAreaOverlayElement.h"
#include "OgreFontManager.h"

namespace Cing
{
	long		TextArea::count=0;
	String		TextArea::m_panelName = "";
	String		TextArea::m_overlayName = "";


	/**
	* @internal
	* @brief Constructor. Initializes class attributes.
	*/
	TextArea::TextArea():
	m_bIsValid( false )
	{
	}

	/**
	* @internal
	* @brief Destructor. Class release.
	*/
	TextArea::~TextArea()
	{
		// Release resources
		end();
	}

	/**
	* @internal
	* @brief Initializes the class so it becomes valid.
	* @param width		width of the text box that will be printed
	* @param height	height of the text box that will be printed
	* @return true if the initialization was ok | false otherwise
	*/
	bool TextArea::init( float width, float height )
	{
		// Check if the class is already initialized
		if ( isValid() )
			return true;

		// PArametros por tocar sobre font
		// antialias_colour
		// Parametros para tocar sobre textOverlay
		//m_pText->setParameter("char_height", "16");
		//m_pText->setParameter("horz_align", "center");
		// char_height",  "Sets the height of the characters in relation to the screen."
		// space_width,  "Sets the width of a space in relation to the screen."
		// font_name
		// colour, "Sets the colour of the font (a solid colour)."
		// colour_bottom, "Sets the colour of the font at the bottom (a gradient colour)."
		// colour_top, "Sets the colour of the font at the top (a gradient colour)."
		// alignment,  "Sets the alignment of the text: 'left', 'center' or 'right'."


		// Create the  overlay and the necessary stuff to show the text in Ogre
		// Note: the structure used is
		// Overlay -> displays panel (which contains the text area)
		m_overlayManager = Ogre::OverlayManager::getSingletonPtr();

		// Create unique id
		++(this->count);
		m_textAreaName	= "textArea_" + Ogre::StringConverter::toString(count);

		// First created
		if( count == 1 )
		{
			m_panelName		= "textArea_panel_" + Ogre::StringConverter::toString(count);
			m_overlayName	= "textArea_overlay_" + Ogre::StringConverter::toString(count);


			// Create the m_panel
			//m_panel = static_cast<Ogre::OverlayContainer*>(m_overlayManager->createOverlayElement("Panel","GUI"));
			m_panel = static_cast<Ogre::OverlayContainer*>(m_overlayManager->createOverlayElement("Panel","TEXT_PANEL"));
			m_panel->setMetricsMode(Ogre::GMM_PIXELS);
			m_panel->setPosition(0,0);
			//m_panel->setDimensions(width, height);
			m_panel->setDimensions( 0.1, 0.1 );

			// Create an overlay to display the panel
			//m_overlay = m_overlayManager->create("GUI_OVERLAY");
			m_overlay = m_overlayManager->create("TEXT_OVERLAY");
			m_overlay->add2D(m_panel);
		}
		// This is not the first -> reuse overlay and panel
		else
		{
			// Get overlay and panel references
			m_overlay = m_overlayManager->getByName("TEXT_OVERLAY");
			m_panel	= static_cast<Ogre::OverlayContainer*>(m_overlayManager->getOverlayElement("TEXT_PANEL"));
		}

		//m_overlay->rotate(Ogre::Radian( Ogre::Degree( 45 ) ));


		// Create the text area (it will cover the whole screen)
		m_textArea = static_cast<Ogre::TextAreaOverlayElement*>(m_overlayManager->createOverlayElement( "TextArea", m_textAreaName ));
		m_textArea->setMetricsMode(Ogre::GMM_PIXELS);
		m_textArea->setFontName( Font::DEFAULT_FONT_NAME );
		m_textArea->setCharHeight(Font::DEFAULT_FONT_SIZE);
		m_textArea->setPosition(0,0);
		m_textArea->setDimensions(width, height);

		// Add it to the panel, and show the overlay
		m_panel->addChild(m_textArea);
		m_overlay->show();

		// The class is now initialized
		m_bIsValid = true;

		return true;
	}

	/**
	* @internal
	* @brief Releases the class resources.
	* After this method is called the class is not valid anymore.
	*/
	void TextArea::end()
	{
		// Check if the class is already released
		if ( !isValid() )
			return;

		m_overlayManager->destroyOverlayElement(m_textAreaName);
		if(count==1)
		{
			m_overlayManager->destroyOverlayElement("TEXT_PANEL");
			m_overlayManager->destroy("TEXT_OVERLAY");
		}

		--(this->count);



		// The class is not valid anymore
		m_bIsValid = false;
	}


	const String& TextArea::getFontName() const
	{
		return m_fontName;
	}

	const String& TextArea::getText() const
	{
		return m_text;
	}


	void TextArea::setText(const String& str)
	{
		m_text = str;
		// Julio
		//m_textArea->setCaption(str.toUTF());
		m_textArea->setCaption(str);

		//m_textArea->rotate(Ogre::Radian( Ogre::Degree( 90 ) ));
	}


	void TextArea::setFontName(const String& fontName)
	{
		m_fontName = fontName;
		m_textArea->setFontName(fontName);
	}

	void TextArea::setPos(float x,float y)
	{
		m_textArea->setPosition(x,y);
	}

	void TextArea::setCol( const Color& color )
	{
		m_textArea->setColour( color.normalized() );
	}


	void TextArea::setSize(float size)
	{
		m_textArea->setCharHeight(size);
	}

	/**
	* @internal
	* @brief Shows or hides the text
	*
	* @param
	*/
	void TextArea::show(bool show ) const
	{
		if ( m_textArea )
		{
			if ( show )
				m_textArea->show();
			else
				m_textArea->hide();
		}
	}

} // namespace Cing
