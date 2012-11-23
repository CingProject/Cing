/*
  This source file is part of the Cing project
  For the latest info, see http://www.cing.cc

Copyright (c) 2008-2009 Julio Obelleiro and Jorge Cano

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

#ifndef _XML_SETTINGS_H
#define _XML_SETTINGS_H

#include "XMLElement.h"
#include "CommonTypes.h"
#include "XMLElement.h"

namespace Cing
{

/**
 * Class that allow easy load of XML Setting files. For not it does not cache the read results, but there is no need to parse the variables, you
 * can just ask for values. Example: settings.get( "height", defaultValue );
 */
class XMLSettings
{
public:

	// Load xml file
	bool load( const std::string& pathToXML );

	// Getters to retrieve the content of an xml tag (what is inside the opening and closing tag: example <bookstore>Content</bookstore>
    std::string 		get		( const std::string& name, const std::string& defValue );
    float       		get		( const std::string& name, float defValue );
    double       		get		( const std::string& name, double defValue );
	unsigned long		get		( const std::string& name, unsigned long defValue );
    int         		get		( const std::string& name, int defValue );
	bool				getBool	( const std::string& name, bool defValue );

	// Getters to retrieve attributes of an xml tags (here for example category would be an attribute <book category="CHILDREN"></book>
    std::string 		getAttribute	( const std::string& tagName, const std::string& attributeName, const std::string& defValue );
    float       		getAttribute	( const std::string& tagName, const std::string& attributeName, float defValue );
    double       		getAttribute	( const std::string& tagName, const std::string& attributeName, double defValue );
	unsigned long		getAttribute	( const std::string& tagName, const std::string& attributeName, unsigned long defValue );
    int         		getAttribute	( const std::string& tagName, const std::string& attributeName, int defValue );
	bool				getBoolAttribute( const std::string& tagName, const std::string& attributeName, bool defValue );
	Cing::Vector		getAttribute	( const std::string& tagName, const Cing::Vector& defValue );

	// Getter of the XML parsing elements
	Cing::XMLElement	get( const std::string& name );
	Cing::XMLElement&	getRootNode(){ return m_xmlFile; };	
	
	// Getters
	const std::string&	getFileName() const { return m_fileName; }

	// Save
	bool				save( const std::string&  xmlFileName );
protected:
	Cing::XMLElement	m_xmlFile; ///< Root node of the xml file
	std::string			m_fileName;///< Filename loaded in this xml (relative to app data folder)

};

} // namespace

#endif