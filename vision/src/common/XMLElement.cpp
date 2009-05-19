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

#include "XMLElement.h"
#include "LogManager.h"
#include "Release.h"

// Tiny xml
#include "externLibs/TinyXML/include/tinyxml.h"

namespace Common
{

/**
 * @internal
 * @brief Constructor. Initializes class attributes.
 */
XMLElement::XMLElement():
	m_xmlDoc( NULL ), m_rootElem( NULL ), m_bIsValid  ( false )
{
}


/**
 * @internal
 * @brief Constructor. Loads an xml file
 * @param xmlFileName  Name of the xml file to load. It must be in the application data folder
 */
XMLElement::XMLElement( const std::string& xmlFileName ):
	m_xmlDoc( NULL ), m_rootElem( NULL ), m_bIsValid  ( false )
{
	load( xmlFileName );
}

/**
 * @internal
 * @brief Destructor. Class release.
 */
XMLElement::~XMLElement()
{
	// Release resources
	end();
}

/**
 * @internal
 * @brief Loads an xml file
 * @param xmlFileName  Name of the xml file to load. It must be in the application data folder
 */
void XMLElement::load( const std::string& xmlFileName )
{
	m_xmlDoc = new TiXmlDocument( xmlFileName.c_str() );
	m_xmlDoc->LoadFile();

	// Error loading file?
	if ( m_xmlDoc->Error() )
	{
		LOG_ERROR( "Error loading %s: %s", xmlFileName.c_str(), m_xmlDoc->ErrorDesc() );
		return;
	}

	// Get root element
	m_rootElem = m_xmlDoc->RootElement();
	if ( !m_xmlDoc )
		LOG_ERROR( "Error loading %s, There is no root element", xmlFileName.c_str() );


	m_bIsValid = true;
}

/**
 * @internal
 * @brief Releases the class resources. 
 * After this call no method of this object can be called without calling init method again.
 */
void XMLElement::end()
{
	Common::Release( m_xmlDoc );
	m_bIsValid = false;
}

/**
 * @brief Returns the number of children for this element
 * @return the number of children for this element
 */
int XMLElement::getChildCount()
{
	// Check state
	if ( !isValid() )
	{
		LOG_ERROR( "Trying to call getChildCount() in a XMLElement no correctly initialized. You should call load() before using this object)" );
		return 0;
	}
	
	// TODO: check a proper way to do this (maybe tinyxml has something inside)
	int count = 0;  
	for( TiXmlNode* node = m_rootElem->FirstChild(); node != NULL; node = node->NextSibling() )  
			++count;  

	return count;
}


} // namespace Common