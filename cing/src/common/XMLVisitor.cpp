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

#include "XMLVisitor.h"
#include "LogManager.h"

namespace Cing
{

	/**
	*  constructor.
	*/
	XMLVisitor::XMLVisitor( XMLElement::XMLDocSharedPtr& xmlDoc, const TiXmlElement& root, XMLElement::XMLElementArray& children, const String& path /*= "NO_PATH"*/ ):
m_xmlDoc( xmlDoc ), m_root( root ), m_children( children ), m_path( path ), TiXmlVisitor() 
{
	// If path in use -> split it for future use
	if ( path != "NO_PATH" )
	{
		std::string token;
		std::istringstream pathToTokenize( path );

		// Split the path
		while ( getline(pathToTokenize, token, '/') )
			m_tokens.push_back( token );

		// Check more than one token
		if ( m_tokens.empty() )
			LOG_ERROR( "Error in XMLElement::getChildren(). path received is empty" );
	}
}

/// Visit an element
bool XMLVisitor::VisitEnter( const TiXmlElement& element, const TiXmlAttribute* firstAttribute )	
{
	// Check if it is the parent node (we only want its children)
	if ( &element == &m_root )
		return true;

	// Check the path (if in use)
	if ( (m_path != "NO_PATH") && !m_tokens.empty() )
	{
		int index = (int)m_tokens.size()-1;
		const TiXmlElement* currentElem = &element;
		while( index >= 0 && currentElem )
		{
			// Check if the current element name corresponds with the current path index
			if ( currentElem->Value() != m_tokens[index] )
				return true;

			// All ok for now
			currentElem = currentElem->Parent()->ToElement();

			// Go to next index
			index--;
		}
	}

	// Store the element
	m_children.push_back( XMLElement( const_cast< TiXmlElement* >( &element ), m_xmlDoc ) );
	return true; 
}

} // namespace