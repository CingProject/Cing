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

#ifndef _Text_H_
#define _Text_H_

#include "GraphicsPrereqs.h"

namespace Graphics
{

/**
 * @internal
 * Class to print text to screen
 */
class Text
{
public:

	// Constructor / Destructor
	 Text();
	~Text();

	// Init / Release / Update
	bool  init   ();
	void  end    ();

	// Query methods
	bool  isValid() const { return m_bIsValid; }

	// Set Methods
	void  setText (char *szString);
	void  setText (Ogre::String szString);
	void  setPos  (float x,float y);
	void  setCol  (float R,float G,float B,float I);
	void	show		(bool show ) const;


private:

	// Attributes
	bool  m_bIsValid;	///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.
	Ogre::OverlayManager *olm;
	Ogre::OverlayContainer *panel ;
	Ogre::Overlay *overlay;
	Ogre::TextAreaOverlayElement *textArea;
	static int count;
	Ogre::String szElement;

};

} // namespace Graphics

#endif // _Text_H_
