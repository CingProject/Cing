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

#ifndef _XMLElement_h_
#define _XMLElement_h_

#include "CommonPrereqs.h"
#include "eString.h"

#include <string>

// Forward declarations
class TiXmlDocument;
class TiXmlElement;

namespace Common
{

/**
 * @internal
 * @brief 
 */
class XMLElement
{
public:

	// Constructor / Destructor
	XMLElement	();
	XMLElement	( const std::string& xmlFileName );
  XMLElement	( TiXmlElement* root, TiXmlDocument* xmlDoc );
	~XMLElement	();

	// Init / Release / Update
	void	load 	( const std::string& xmlFileName );
	void	end		();	

	// Query  Methods
	int						getChildCount	();
	XMLElement		getChild			( int index );
	XMLElement		getChild			( const std::string& path );
  String        getContent    ();
  String        getName       ();
	bool					isValid				() { return m_bIsValid; }

private:

	// Attributes
	TiXmlDocument*	m_xmlDoc;			///< Xml document
	TiXmlElement*	  m_rootElem;		///< Xml's root element
	bool			      m_bIsValid;	  ///< Indicates whether the class is valid or not. If invalid none of its methods except init should be called.

};

} // namespace Common

#endif // _XMLElement_h_