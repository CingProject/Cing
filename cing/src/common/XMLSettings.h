#ifndef _XML_SETTINGS_H
#define _XML_SETTINGS_H

// Precompiled headers
#include "Framework-Precompiled.h"

#include "common/XMLElement.h"
#include "common/CommonTypes.h"
#include "common/XMLElement.h"

class XMLSettings
{

public:

	// Load/Release
	bool load( const std::string& pathToXML );

	// methods to load properties directly from xml
	Cing::XMLElement	get( const std::string& name );
    std::string 		get( const std::string& name, const std::string& defValue );
    float       		get( const std::string& name, float defValue );
    double       		get( const std::string& name, double defValue );
    int         		get( const std::string& name, int defValue );
	Cing::Vector		get( const std::string& name, const Cing::Vector& defValue );
	unsigned long		get( const std::string& name, unsigned long defValue );
	bool				getBool( const std::string& name, bool defValue );
	
	Cing::XMLElement&	getRootNode(){ return m_xmlFile; };

protected:
	Cing::XMLElement	m_xmlFile; ///< Root node of the xml file

};
#endif