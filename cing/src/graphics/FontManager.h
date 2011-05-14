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

#ifndef _FontManager_H_
#define _FontManager_H_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "GraphicsPrereqs.h"
#include "FontProperties.h"
#include "MovableText.h"

// Common
#include "common/Singleton.h"

namespace Cing
{
	

/**
 * @internal
 * Manages the font system
 */
class FontManager: public SingletonStatic< FontManager >
{

public:

	// Required for singleton
	friend class SingletonStatic< FontManager >;

	// Destructor
	virtual ~FontManager();

	// Init / Release / Draw
	bool                    init		();
	void                    end			();
	void                    postRender  ();

	// Set methods
	void					setActiveFont	( const Font& font );
	void					addText			();

  	// Query methods
	FontProperties&			getActiveFontProperties	() { return m_activeFontProperties; }
	bool                    isValid					() const { return m_bIsValid && m_activeFont; }

private:

	// private types
	typedef std::vector< MovableText* > TextList;

	// private constructor to ensure singleton
	FontManager      ();

	// Internal methods
	MovableText*	getNewText();

	// Attributes
	FontProperties				m_activeFontProperties;	///< Font properties used in the next call to text to render a string of text
	const Font*					m_activeFont;			///< Active font (activated calling textFont function
	Font						m_defaultFont;			///< Font to use in case the user does not want to create one...
	TextList					m_activeFontsToRender;	///< Text areas to be rendered in the next draw call
	size_t						m_nextFontToUse;		///< Index to reuse previously used fonts
	bool						m_bIsValid;				///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called
	
};

} // namespace Cing

#endif // _FontManager_H_
