// Precompiled headers
#include "Framework-Precompiled.h"

#include "XMLSettings.h"
#include "Cing.h"
#include <string>

// Loads the xml file 
bool XMLSettings::load( const std::string& pathToXML )
{
	return m_xmlFile.load( pathToXML );
}


/**
 * @brief Returns an xml element associated with a received tag name.
 * @note It can handle nested elements with /. For example, "Parent/Child" will return an element called Child 
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
 * @brief Returns the string contents of an an xml element given a tag name or path within the xml
 * @note It can handle nested elements with /. For example, "Parent/Child" will return an element called Child 
 * that is nested into an element called Parent.
 * This would be in the xml:
 *		<Parent>
 *			<Child>Child contents<Child/>
 *		<Parent/>
 * @param name		Name or path (in the xml) of the xml element to find
 * @param defValue	Value returned in case the element is not found
 */
std::string XMLSettings::get( const std::string& name, const std::string& defValue )
{
    Cing::XMLElement elem = get( name );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [string], defValue %s", name.c_str(), defValue.c_str() );
        return defValue;
    }
    return elem.getContent();
}

float XMLSettings::get( const std::string& name, float defValue )
{
    Cing::XMLElement elem = get( name );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [float], defValue %f", name.c_str(), defValue );
        return defValue;
    }
    return stringToFloat( elem.getContent() );
}

double XMLSettings::get( const std::string& name, double defValue )
{
    Cing::XMLElement elem = get( name );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [double], defValue %f", name.c_str(), defValue );
        return defValue;
    }
    return stringToFloat( elem.getContent() );
}

unsigned long XMLSettings::get( const std::string& name, unsigned long defValue) {
	Cing::XMLElement elem = get( name );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [unsigned long], defValue %ld", name.c_str(), defValue );
        return defValue;
    }
    return stringToUint32( elem.getContent() );
}

int XMLSettings::get( const std::string& name, int defValue )
{
    Cing::XMLElement elem = get( name );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [int], defValue %i", name.c_str(), defValue );
        return defValue;
    }
    return stringToInt( elem.getContent() );
}

Cing::Vector XMLSettings::get( const std::string& name, const Cing::Vector& defValue )
{
    Cing::XMLElement elem = get( name );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [vector], defValue [%f,%f,%f]", name.c_str(), defValue.x, defValue.y, defValue.z );
        return defValue;
    }

    float x = elem.getFloatAttribute("x");
    float y = elem.getFloatAttribute("y");
    float z = elem.getFloatAttribute("z");

    return Cing::Vector( x, y, z );
}



bool XMLSettings::getBool( const std::string& name, bool defValue )
{
	// Get it as string value
	Cing::XMLElement elem = get( name );
    if ( !elem.isValid() )
    {
        LOG_ERROR( "XMLSettings: %s not found [bool], defValue [%s]",  name.c_str(), defValue? "true": "false" );
        return defValue;
    }

	return  elem.getContent() == "true"? true: false;
}


