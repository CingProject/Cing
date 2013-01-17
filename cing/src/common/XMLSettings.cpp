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

// Precompiled headers
#include "Cing-Precompiled.h"

#include "XMLSettings.h"
#include "Cing.h"
#include <string>

namespace Cing
{

// Loads the xml file 
bool XMLSettings::load( const std::string& pathToXML )
{
	m_fileName = pathToXML;
	return m_xmlFile.load( pathToXML );
}


/**
 * @brief Returns an xml element associated with a received tag name.
 * @note It can handle nested tags using the character '/'. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * This would be in the xml:
 *		<Parent>
 *			<Child>Child contents<Child/>
 *		<Parent/>
 * @param name Name or path (in the xml) of the xml element to find
 */
Cing::XMLElement XMLSettings::get( const std::string& name )
{
	// split input name in tokens
    std::vector<std::string> tokens;
	split( name, '/', tokens );

	Cing::XMLElement elem = m_xmlFile;
    for ( std::vector<std::string>::const_iterator it = tokens.begin(); it != tokens.end(); ++it )
    {
        elem = elem.getChild( *it );
        if ( !elem.isValid() )
            break;
    }
    return elem;
}

/**
 * @brief Returns the string contents of an an xml tag given its name or path within the xml
 * @note It can handle nested tags using the character '/'. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * This would be in the xml:
 *		<Parent>
 *			<Child>Child contents<Child/>
 *		<Parent/>
 * @param name		Name or path (in the xml) of the xml tag to find
 * @param defValue	Value returned in case the tag is not found. En error log will be printed
 */
std::string XMLSettings::get( const std::string& name, const std::string& defValue )
{
    Cing::XMLElement elem = get( name );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [string], default value %s", name.c_str(), defValue.c_str() );
        return defValue;
    }
    return elem.getContent();
}

/**
 * @brief Returns the float contents of an an xml tag given its name or path within the xml
 * @note It can handle nested tags using the character '/'. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * This would be in the xml:
 *		<Parent>
 *			<Child>5.0<Child/>
 *		<Parent/>
 * @param name		Name or path (in the xml) of the xml tag to find
 * @param defValue	Value returned in case the tag is not found. En error log will be output
 */
float XMLSettings::get( const std::string& name, float defValue )
{
    Cing::XMLElement elem = get( name );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [float], default value %f", name.c_str(), defValue );
        return defValue;
    }
    return stringToFloat( elem.getContent() );
}

/**
 * @brief Returns the double contents of an an xml tag given its name or path within the xml
 * @note It can handle nested tags using the character '/'. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * This would be in the xml:
 *		<Parent>
 *			<Child>5.0<Child/>
 *		<Parent/>
 * @param name		Name or path (in the xml) of the xml tag to find
 * @param defValue	Value returned in case the tag is not found. En error log will be output
 */
double XMLSettings::get( const std::string& name, double defValue )
{
    Cing::XMLElement elem = get( name );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [double], default value %f", name.c_str(), defValue );
        return defValue;
    }
    return stringToDouble( elem.getContent() );
}


/**
 * @brief Returns the unsigned long contents of an an xml tag given its name or path within the xml
 * @note It can handle nested tags using the character '/'. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * This would be in the xml:
 *		<Parent>
 *			<Child>3500<Child/>
 *		<Parent/>
 * @param name		Name or path (in the xml) of the xml tag to find
 * @param defValue	Value returned in case the tag is not found. En error log will be output
 */
unsigned long XMLSettings::get( const std::string& name, unsigned long defValue) {
	Cing::XMLElement elem = get( name );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [unsigned long], default value %ld", name.c_str(), defValue );
        return defValue;
    }
    return stringToUint32( elem.getContent() );
}

/**
 * @brief Returns the int contents of an an xml tag given its name or path within the xml
 * @note It can handle nested tags using the character '/'. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * This would be in the xml:
 *		<Parent>
 *			<Child>-5<Child/>
 *		<Parent/>
 * @param name		Name or path (in the xml) of the xml tag to find
 * @param defValue	Value returned in case the tag is not found. En error log will be output
 */
int XMLSettings::get( const std::string& name, int defValue )
{
    Cing::XMLElement elem = get( name );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [int], default value %i", name.c_str(), defValue );
        return defValue;
    }
    return stringToInt( elem.getContent() );
}

/**
 * @brief Returns the bool contents of an an xml tag given its name or path within the xml. Values can be "true" or "false"
 * @note It can handle nested tags using the character '/'. For example, "Parent/Child" will find a tag called Child 
 * that is nested in a tag called Parent.
 * This would be in the xml:
 *		<Parent>
 *			<Child>true<Child/>
 *		<Parent/>
 * @param name		Name or path (in the xml) of the xml tag to find
 * @param defValue	Value returned in case the tag is not found. En error log will be output
 */
bool XMLSettings::getBool( const std::string& name, bool defValue )
{
	// Get it as string value
	Cing::XMLElement elem = get( name );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [bool], default value [%s]",  name.c_str(), defValue? "true": "false" );
        return defValue;
    }

	// Convert to lower case
	String boolString = elem.getContent();
	boolString.toLowerCases();

	// Check value
	bool returnValue = true;

	// Value is true
	if ( boolString == "true" )
		returnValue = true;

	// Value is false
	else if ( boolString == "false" )
		returnValue = false;

	// Value is not recognized, assuming false
	else
	{
		LOG_WARNING( "XMLSettings: tag %s has an unrecognized value [%s], assuming [false]",  name.c_str(), elem.getContent().c_str() );
		returnValue = false;
	}
		
	return returnValue;
}
//----

/**
 * @brief Returns a string attribute of an xml tag, given the tag's name or path within the xml, and the attribute name
 * @note It can handle nested tags with /. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * For example, the instruction: settings.getAttribute( "Parent/Child", "name", "Peter" ); would return "John" with the 
 * following xml content:
 *		<Parent>
 *			<Child name="John"><Child/>
 *		<Parent/>
 * @param name		Name or path (in the xml) of the xml tag to find
 * @param defValue	Value returned in case the tag or attribute are not found. En error log will be printed
 */
std::string XMLSettings::getAttribute( const std::string& tagName, const std::string& attributeName, const std::string& defValue )
{
    Cing::XMLElement elem = get( tagName );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: tag %s not found [string], default value %s", tagName.c_str(), defValue.c_str() );
        return defValue;
    }
    return elem.getStringAttribute(attributeName, defValue);
}

/**
 * @brief Returns a float attribute of an xml tag, given the tag's name or path within the xml, and the attribute name
 * @note It can handle nested tags with /. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * For example, the instruction: settings.getAttribute( "Parent/Child", "height", 5.4 ); would return 5.6 with the 
 * following xml content:
 *		<Parent>
 *			<Child height="5.6"><Child/>
 *		<Parent/>
 * @param name		Name or path (in the xml) of the xml tag to find
 * @param defValue	Value returned in case the tag or attribute are not found. En error log will be printed
 */
float XMLSettings::getAttribute( const std::string& tagName, const std::string& attributeName, float defValue )
{
    Cing::XMLElement elem = get( tagName );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [float], default value %f", tagName.c_str(), defValue );
        return defValue;
    }
	return elem.getFloatAttribute( attributeName, defValue );
}

/**
 * @brief Returns a double attribute of an xml tag, given the tag's name or path within the xml, and the attribute name
 * @note It can handle nested tags with /. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * For example, the instruction: settings.getAttribute( "Parent/Child", "height", 5.4 ); would return 5.6 with the 
 * following xml content:
 *		<Parent>
 *			<Child height="5.6"><Child/>
 *		<Parent/>
 * @param name		Name or path (in the xml) of the xml tag to find
 * @param defValue	Value returned in case the tag or attribute are not found. En error log will be printed
 */
double XMLSettings::getAttribute( const std::string& tagName, const std::string& attributeName, double defValue )
{
    Cing::XMLElement elem = get( tagName );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [double], default value %f", tagName.c_str(), defValue );
        return defValue;
    }
    return elem.getDoubleAttribute( attributeName, defValue );
}


/**
 * @brief Returns a unsigned long attribute of an xml tag, given the tag's name or path within the xml, and the attribute name
 * @note It can handle nested tags with /. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * For example, the instruction: settings.getAttribute( "Parent/Child", "height", 100 ); would return 18000 with the 
 * following xml content:
 *		<Parent>
 *			<Child height="18000"><Child/>
 *		<Parent/>
 * @param name		Name or path (in the xml) of the xml tag to find
 * @param defValue	Value returned in case the tag or attribute are not found. En error log will be printed
 */
unsigned long XMLSettings::getAttribute( const std::string& tagName, const std::string& attributeName, unsigned long defValue) {
	Cing::XMLElement elem = get( tagName );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [unsigned long], default value %ld", tagName.c_str(), defValue );
        return defValue;
    }
    return elem.getAttribute( attributeName, defValue );
}

/**
 * @brief Returns an int attribute of an xml tag, given the tag's name or path within the xml, and the attribute name
 * @note It can handle nested tags with /. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * For example, the instruction: settings.getAttribute( "Parent/Child", "height", 6 ); would return 5 with the 
 * following xml content:
 *		<Parent>
 *			<Child height="5"><Child/>
 *		<Parent/>
 * @param name		Name or path (in the xml) of the xml tag to find
 * @param defValue	Value returned in case the tag or attribute are not found. En error log will be printed
 */
int XMLSettings::getAttribute( const std::string& tagName, const std::string& attributeName, int defValue )
{
    Cing::XMLElement elem = get( tagName );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [int], default value %i", tagName.c_str(), defValue );
        return defValue;
    }
    return elem.getIntAttribute( attributeName, defValue );
}

/**
 * @brief Returns a bool attribute of an xml tag, given the tag's name or path within the xml, and the attribute name
 * @note It can handle nested tags with /. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * For example, the instruction: settings.getAttribute( "Parent/Child", "enabled" ); would return true with the 
 * following xml content:
 *		<Parent>
 *			<Child enabled="true"><Child/>
 *		<Parent/>
 * @param name		Name or path (in the xml) of the xml tag to find
 * @param defValue	Value returned in case the tag or attribute are not found. En error log will be printed
 */
bool XMLSettings::getBoolAttribute( const std::string& tagName, const std::string& attributeName, bool defValue )
{
	// Get it as string value
	Cing::XMLElement elem = get( tagName );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [bool], default value [%s]",  tagName.c_str(), defValue? "true": "false" );
        return defValue;
    }

	// Convert to lower case
	String boolString = elem.getStringAttribute( attributeName, "true" );
	boolString.toLowerCases();

	// Check value
	bool returnValue = true;

	// Value is true
	if ( boolString == "true" )
		returnValue = true;

	// Value is false
	else if ( boolString == "false" )
		returnValue = false;

	// Value is not recognized, assuming false
	else
	{
		LOG_WARNING( "XMLSettings: tag %s has an unrecognized value [%s], assuming [false]",  tagName.c_str(), elem.getContent().c_str() );
		returnValue = false;
	}
		
	return returnValue;
}


/**
 * @brief Returns a Vector (3 floats, x, y and z) attribute of an xml tag, given the tag's name or path within the xml
 * @note the attributes should be called x, y a and z
 * @note It can handle nested tags with /. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * For example, the instruction: settings.getAttribute( "Position" ); would return a Vector with the values 100, 50, 0
 * with the following xml content:
 *		<Position x="100" y="50" z="0">
 *		<Position/>
 * @param name		Name or path (in the xml) of the xml tag to find
 * @param defValue	Value returned in case the tag or attribute are not found. En error log will be printed
 */
Cing::Vector XMLSettings::getAttribute( const std::string& tagName, const Cing::Vector& defValue )
{
    Cing::XMLElement elem = get( tagName );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [vector], default value [%f,%f,%f]", tagName.c_str(), defValue.x, defValue.y, defValue.z );
        return defValue;
    }

    float x = elem.getFloatAttribute("x");
    float y = elem.getFloatAttribute("y");
    float z = elem.getFloatAttribute("z");

    return Cing::Vector( x, y, z );
}

/**
 * @brief Save to file
 * @param xmlFileName		File name
 */
bool XMLSettings::save( const std::string& xmlFileName )
{
	// Store the xml doc inside data folder
	std::string fullPath( dataFolder );
	fullPath += xmlFileName;

	if( !m_xmlFile.save( fullPath ) )
	{
		LOG_ERROR( "XMLSettings: Save to file error. Check this path is valid: %s", xmlFileName.c_str() );
		return false;
	}
	return true;
}

} // namespace


