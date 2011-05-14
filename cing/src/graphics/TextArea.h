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

#ifndef _TextArea_H_
#define _TextArea_H_

// Precompiled headers
#include "Cing-Precompiled.h"

#include "GraphicsPrereqs.h"

// Common
#include "common/eString.h"

namespace Cing
{

	/**
	* @internal
	* Class to print text to screen
	*/
	class TextArea
	{
	public:

		// Constructor / Destructor
		TextArea();
		~TextArea();

		// Init / Release / Update
		bool  init   ( float width, float height );
		void  end    ();

		// Query methods
		const String&	getFontName	() const;
		const String&	getText		() const;
		bool			isValid		() const { return m_bIsValid; }

		// Set Methods
		void	setText		(const String& str);
		void	setFontName	(const String& fontName);
		void	setPos  	(float x,float y);
		void	setCol  	(const Color& color);
		void	setSize		(float size);
		void	show		(bool show ) const;

	private:

		// Static Attributes
		static long		count; ///< Used to have unique names for the textAreas
		static String	m_panelName;
		static String	m_overlayName;

		// Attributes
		String							m_textAreaName;
		String							m_fontName;
		String							m_text;

		// Ogre related attributes
		Ogre::OverlayManager*			m_overlayManager;
		Ogre::OverlayContainer*			m_panel ;
		Ogre::Overlay*					m_overlay;
		Ogre::TextAreaOverlayElement*	m_textArea;

		bool  m_bIsValid;	///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

	};

} // namespace Cing

#endif // _Text_H_
