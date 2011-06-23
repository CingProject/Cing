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

#ifndef _XMLElement_h_
#define _XMLElement_h_

// Precompiled headers
#include "Cing-Precompiled.h"


#include "CommonPrereqs.h"
#include "eString.h"

#include "OgreSharedPtr.h"
#include "TinyXML/include/tinyxml.h"

#include <string>

namespace Cing
{


	/**
	* @internal
	* @brief
	*/
	class XMLElement
	{
	public:

		// XMLElement Array type
		typedef std::vector< XMLElement > XMLElementArray;

		// Shared pointer for the xml doc (it can be shared among many XMLElement)
		typedef Ogre::SharedPtr<TiXmlDocument> XMLDocSharedPtr;

		// Constructor / Destructor
		XMLElement	();
		XMLElement	( const std::string& xmlFileName );
		XMLElement	( TiXmlElement* root, XMLDocSharedPtr& xmlDoc );
		~XMLElement	();

		// Init / Release / Update
		bool	load 	( const std::string& xmlFileName );
		void	end		();

		// Query  Methods
		int						getChildCount	    ();
		XMLElement		getChild			    ( int index );
		XMLElement		getChild			    ( const std::string& path );
		void          getChildren       ( XMLElementArray& children, const String& path = "NO_PATH" );
		String        getContent        ();
		int           getIntAttribute   ( const String& name, int defaultValue = 0 );
		float         getFloatAttribute ( const String& name, float defaultValue = 0.0f );
		String        getStringAttribute( const String& name, String defaultValue = "" );
		String        getName           ();
		bool					isValid				    () { return m_bIsValid; }

	private:

		// Attributes
		XMLDocSharedPtr   m_xmlDoc;
		TiXmlElement*	    m_rootElem;		///< Xml's root element
		bool			        m_bIsValid;	  ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

	};




} // namespace Cing

#endif // _XMLElement_h_
