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
 *
 * @return true if the initialization was ok | false otherwise
 */
bool Text::init()
{
  // Check if the class is already initialized
  if ( isValid() )
    return true;

    olm=Ogre::OverlayManager::getSingletonPtr();
		if(count==0)
		{
		panel=static_cast<Ogre::OverlayContainer*>(olm->createOverlayElement("Panel","GUI"));
			panel->setMetricsMode(Ogre::GMM_PIXELS);
			panel->setPosition(0,0);
			panel->setDimensions(1.0f,1.0f);
			overlay=olm->create("GUI_OVERLAY");
			overlay->add2D(panel);
		}
		++(this->count);
    szElement="element_"+Ogre::StringConverter::toString(count);
		overlay=olm->getByName("GUI_OVERLAY");
		panel=static_cast<Ogre::OverlayContainer*>(olm->getOverlayElement("GUI"));
		textArea=static_cast<Ogre::TextAreaOverlayElement*>(olm->createOverlayElement("TextArea",szElement));
		panel->addChild(textArea);
		overlay->show();

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

		szElement="element_"+Ogre::StringConverter::toString(count);
		olm->destroyOverlayElement(szElement);
		--(this->count);
		if(count==0)
		{
			olm->destroyOverlayElement("GUI");
			olm->destroy("GUI_OVERLAY");
		}

	// The class is not valid anymore
	m_bIsValid = false;
}

void Text::setText(char *szString)
{
	textArea->setCaption(szString);
	textArea->setDimensions(1.0f,1.0f);
	textArea->setMetricsMode(Ogre::GMM_RELATIVE);
	textArea->setFontName("BlueHighway");
	textArea->setCharHeight(0.02f);
}
void Text::setText(Ogre::String szString) // now You can use Ogre::String as text
{
	textArea->setCaption(szString);
	textArea->setDimensions(1.0f,1.0f);
	textArea->setMetricsMode(Ogre::GMM_RELATIVE);
	textArea->setFontName("BlueHighway");
	textArea->setCharHeight(0.02f);
}
void Text::setPos(float x,float y)
{
	textArea->setPosition(x,y);
}
void Text::setCol(float R,float G,float B,float I)
{
	textArea->setColour(Ogre::ColourValue(R,G,B,I));
}

/**
 * @internal 
 * @brief Shows or hides the text
 *
 * @param
 */
void Text::show(bool show ) const
{
	if ( textArea )
	{
		if ( show )
			textArea->show();
		else
			textArea->hide();
	}
}

} // namespace Graphics
