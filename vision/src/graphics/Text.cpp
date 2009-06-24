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

#include "Text.h"

// OGRE
#include "externLibs/Ogre3d/include/OgreOverlayManager.h"
#include "externLibs/Ogre3d/include/OgreOverlayContainer.h"
#include "externLibs/Ogre3d/include/OgreStringConverter.h"
#include "externLibs/Ogre3d/include/OgreTextAreaOverlayElement.h"
#include "externLibs/Ogre3d/include/OgreFontManager.h"

namespace Graphics
{

	int Text::count=0;

	/**
	* @internal
	* @brief Constructor. Initializes class attributes.
	*/
	Text::Text():
	m_bIsValid( false )
	{
	}

	/**
	* @internal
	* @brief Destructor. Class release.
	*/
	Text::~Text()
	{
		// Release resources
		end();
	}

	/**
	* @internal
	* @brief Initializes the class so it becomes valid.
	* @param ttfName Name of the true type font to load. It shoud be in the data folder
	* @return true if the initialization was ok | false otherwise
	*/
	bool Text::load( const char* ttfName )
	{
		// Check if the class is already initialized
		if ( isValid() )
			return true;

		// Store the font name
		m_fontName = ttfName;

		// get the font manager
		Ogre::FontManager &fontMgr = Ogre::FontManager::getSingleton();

		// create a font resource
		Ogre::ResourcePtr font = fontMgr.create( ttfName, "Fonts" );

		// set as truetype (TODO: support images)
		font->setParameter( "type", "truetype" );

		// set the .ttf file name
		font->setParameter( "source", ttfName );

		// set the size
		font->setParameter("size","26");

		// set the dpi
		font->setParameter("resolution","96");

		// load the ttf
		font->load();

		// Create the  overlay and the necessary stuff to show the text in Ogre
		// Note: the structure used is
		// Overlay -> displays panel (which contains the text area)
		m_overlayManager = Ogre::OverlayManager::getSingletonPtr();

		// First created
		if( count == 0 )
		{
			// Create the m_panel
			m_panel = static_cast<Ogre::OverlayContainer*>(m_overlayManager->createOverlayElement("Panel","GUI"));
			m_panel->setMetricsMode(Ogre::GMM_PIXELS);
			m_panel->setPosition(0,0);
			m_panel->setDimensions(1.0f,1.0f);

			// Create an overlay to display the panel
			m_overlay = m_overlayManager->create("GUI_OVERLAY");
			m_overlay->add2D(m_panel);
		}
		// This is not the first -> reuse overlay and panel
		else
		{
			// Get overlay and panel references 
			m_overlay = m_overlayManager->getByName("GUI_OVERLAY");
			m_panel	= static_cast<Ogre::OverlayContainer*>(m_overlayManager->getOverlayElement("GUI"));
		}

		// Create unique id
		++(this->count);
		m_uniqueName = "element_" + Ogre::StringConverter::toString(count);
	
		// Create the text area
		m_textArea = static_cast<Ogre::TextAreaOverlayElement*>(m_overlayManager->createOverlayElement( "TextArea", m_uniqueName ));

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
	void Text::end()
	{
		// Check if the class is already released
		if ( !isValid() )
			return;

		m_uniqueName = "element_" + Ogre::StringConverter::toString(count);
		m_overlayManager->destroyOverlayElement(m_uniqueName);
		--(this->count);
		if(count==0)
		{
			m_overlayManager->destroyOverlayElement("GUI");
			m_overlayManager->destroy("GUI_OVERLAY");
		}

		// The class is not valid anymore
		m_bIsValid = false;
	}

	void Text::setText(char *szString)
	{
		m_textArea->setCaption(szString);
		m_textArea->setDimensions(1.0f,1.0f);
		m_textArea->setMetricsMode(Ogre::GMM_RELATIVE);
		m_textArea->setFontName( m_fontName );
		m_textArea->setCharHeight(0.02f);
	}
	void Text::setText(Ogre::String szString) // now You can use Ogre::String as text
	{
		m_textArea->setCaption(szString);
		m_textArea->setDimensions(1.0f,1.0f);
		m_textArea->setMetricsMode(Ogre::GMM_RELATIVE);
		m_textArea->setFontName(m_fontName);
		m_textArea->setCharHeight(0.02f);
	}
	void Text::setPos(float x,float y)
	{
		m_textArea->setPosition(x,y);
	}
	void Text::setCol(float R,float G,float B,float I)
	{
		m_textArea->setColour(Ogre::ColourValue(R,G,B,I));
	}

	/**
	* @internal 
	* @brief Shows or hides the text
	*
	* @param
	*/
	void Text::show(bool show ) const
	{
		if ( m_textArea )
		{
			if ( show )
				m_textArea->show();
			else
				m_textArea->hide();
		}
	}

} // namespace Graphics
