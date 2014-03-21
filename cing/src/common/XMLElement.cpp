/*
 This source file is part of the Cing project
 For the latest info, see http://www.cing.cc
 
 License: MIT License (http://www.opensource.org/licenses/mit-license.php)
 
 Copyright (c) 2006- Julio Obelleiro, Jorge Cano and the Cing community
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

// Precompiled headers
#include "Cing-Precompiled.h"


#include "XMLElement.h"
#include "LogManager.h"
#include "Release.h"
#include "XMLVisitor.h"
#include "SystemUtils.h"

// Tiny xml
#include "TinyXML/include/tinyxml.h"

// Framework
#include "framework/UserAppGlobals.h"

#include <iostream>
#include <sstream>


namespace Cing
{
    
    /**
     * @internal
     * @brief Constructor. Initializes class attributes.
     */
    XMLElement::XMLElement():
    m_rootElem( NULL ), m_bIsValid  ( false )
    {
    }
    
    
    /**
     * @internal
     * @brief Constructor. Loads an xml file
     * @param xmlFileName  Name of the xml file to load. It must be in the application data folder
     */
    XMLElement::XMLElement( const std::string& xmlFileName ):
	m_rootElem( NULL ), m_bIsValid  ( false )
    {
        load( xmlFileName );
    }
    
    /**
     * @internal
     * @brief Constructor. Builds an xml element from a parent xml element
     * @param root    root root element
     * @param xmlDoc  xmlDoc doc associated
     */
    XMLElement::XMLElement( TiXmlElement* root, XMLDocSharedPtr &xmlDoc )
    {
        m_xmlDoc    = xmlDoc;
        m_rootElem  = root;
        m_bIsValid = true;
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
     * @brief Loads xml data in the form of a string
     * @param xmlData  xml data to be parsed and loaded
     * @return true of the data was correctly loaded, false otherwise
     */
    bool XMLElement::parse( const std::string& xmlData )
    {
        // Check if load has already been called
        if ( isValid() )
            end();
        
        // Load the xml data
        m_xmlDoc = XMLDocSharedPtr( new TiXmlDocument() );
        m_xmlDoc->Parse( xmlData.c_str(), 0, TIXML_ENCODING_UTF8 );
        
        // Error loading file?
        if ( m_xmlDoc->Error() )
        {
            LOG_ERROR( "Error loading %s: %s", xmlData.c_str(), m_xmlDoc->ErrorDesc() );
            return false;
        }
        
        // Get root element
        m_rootElem = m_xmlDoc->RootElement();
        if ( !m_rootElem )
        {
            LOG_ERROR( "Error loading %s, There is no root element", xmlData.c_str() );
            return false;
        }
        
        
        LOG( "XMLElement: xml data successfuly loaded" );
        
        m_bIsValid = true;
        return true;
    }
    
    /**
     * @internal
     * @brief Loads an xml file
     * @param xmlFileName  Name of the xml file to load. It must be in the application data folder
     * @return true of the file was correctly loaded, false otherwise
     */
    bool XMLElement::load( const std::string& xmlFileName )
    {
        // Check if load has already been called
        if ( isValid() )
            end();
        
		// Load the xml file
		std::string fileAbsPath = xmlFileName;

		// If it's a relative path, add the data folder to the path.
		if ( isPathAbsolute( xmlFileName ) == false  )
			fileAbsPath = dataFolder + xmlFileName;
		
        LOG("Trying to load XML file: %s", fileAbsPath.c_str());
        m_xmlDoc = XMLDocSharedPtr( new TiXmlDocument( fileAbsPath.c_str() ) );
        m_xmlDoc->LoadFile();
        
        // Error loading file?
        if ( m_xmlDoc->Error() )
        {
            LOG_ERROR( "Error loading %s: %s", xmlFileName.c_str(), m_xmlDoc->ErrorDesc() );
            return false;
        }
        
        // Get root element
        m_rootElem = m_xmlDoc->RootElement();
        if ( !m_rootElem )
        {
            LOG_ERROR( "Error loading %s, There is no root element", xmlFileName.c_str() );
            return false;
        }
        
        
        LOG( "XMLElement: File %s succesfuly loaded", fileAbsPath.c_str() );
        
        m_bIsValid = true;
        return true;
    }
    
    /**
     * @brief Inits an xml (created from code instead of loaded from file)
     */
    void XMLElement::init( const std::string& topLevelName )
    {
        // Check if load has already been called
        if ( isValid() )
            end();
        
        // Create the empty xml
        m_xmlDoc = XMLDocSharedPtr( new TiXmlDocument());
        
        // Create the root element
        m_xmlDoc->LinkEndChild(new TiXmlDeclaration( "1.0", "", "" ) );
        m_xmlDoc->LinkEndChild(new TiXmlElement( topLevelName ) );
        
        m_rootElem = m_xmlDoc->RootElement();
        if ( !m_rootElem )
        {
            LOG_ERROR( "Error creating xml file. There is no root element" );
            return;
        }
        
        m_bIsValid = true;
    }
    
    /**
     * @internal
     * @brief Releases the class resources.
     * After this call no method of this object can be called without calling init method again.
     */
    void XMLElement::end()
    {
        // @note The xmlDoc is not deleted because it is a shared pointer (as it can be shared among many XMLElement)
        m_bIsValid = false;
    }
    
    /**
     * @brief Returns the number of children for this element
     * @return the number of children for this element
     */
    int XMLElement::getChildCount()
    {
        // Check state
        if ( !m_rootElem )
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
    
    /**
     * @brief Returns a children of the XML element (which will be another element)
     * @return index index of the child that will be returned
     */
    XMLElement XMLElement::getChild( int index )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call getChild() in a XMLElement no correctly initialized. You should call load() before using this object)" );
            return XMLElement();
        }
        
        // Check number of children
        if ( ( index < 0 ) || ( index >= getChildCount()) )
        {
            LOG_ERROR( "Error in XMLElement::getChild. Child index requested out of bounds" );
            return XMLElement();
        }
        
        // get child
        int count = 0;
        TiXmlElement *child = m_rootElem->FirstChild()->ToElement();
        while ( count < index )
        {
            // Go to next child
            child = child->NextSibling()->ToElement();
            
            // Check if child is ok
            if ( child == NULL )
            {
                LOG_ERROR( "Error in XMLElement::getChild. Requested index is out of range" );
                return XMLElement();
            }
            
            count++;
        }
        
        // Return found element
        return XMLElement( child, m_xmlDoc );
    }
    
    /**
     * @brief Returns a children of the XML element referred by its path. Path element separator is /
     * @return path path to the child that will be returned
     */
    XMLElement XMLElement::getChild( const std::string& path )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call getChild() in a XMLElement no correctly initialized. You should call load() before using this object)" );
            return XMLElement();
        }
        
        // Find the requested element
        TiXmlElement* child = NULL;
        std::string token;
        std::istringstream pathToTokenize( path );
        std::vector< std::string > tokens;
        
        // Split the path
        while ( getline(pathToTokenize, token, '/') )
            tokens.push_back( token );
        
        // Check more than one token
        if ( tokens.empty() )
        {
            LOG_ERROR( "Error in XMLElement::getChild. path received is empty" );
            return XMLElement();
        }

        // Iterate through all the tokens in the list
		TiXmlElement* elem = m_rootElem;
        for ( size_t i = 0; i < tokens.size(); ++i )
        {
			// security check
			if ( !elem )
			{
				LOG_ERROR( "Error in XMLElement::getChild. path received does not correspond to any child in the XML file. %s not found", path.c_str() );
				return XMLElement();
			}

			// get the next child node
			TiXmlNode* childNode = elem->FirstChild( tokens[i].c_str() );        
			// Get first child that is not a comment (this prevents that if there is a comment with the same text as a label, the label is returned first)
			while ( childNode && (childNode->ToElement() == NULL) ) // it can be not null, but be null when caseted to element (a comment for example)
			{
				childNode = m_rootElem->IterateChildren(childNode);
			}
            
			// If it's a tag we should be able to convert it to element
			TiXmlElement* childElem = childNode? childNode->ToElement(): NULL;
			if ( !childElem )
			{
				LOG_ERROR( "Error in XMLElement::getChild. path received does not correspond to any child in the XML file. %s not found", path.c_str() );
				return XMLElement();
			}

			/// Move to next
			elem = childElem;
        }
        
        // Return found element
        return XMLElement( elem, m_xmlDoc );
    }
    
    /**
     * @brief Fills an array of XMLElemnt with all the xml elements of the file
     * @param children Array where the children will be stored
     * @param path If path is specified, only children that match that path will be stored (optional parameter)
     */
    void XMLElement::getChildren( XMLElementArray& children, const String& path /*= "NO_PATH"*/ )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call getChildren() in a XMLElement no correctly initialized. You should call load() before using this object)" );
            return;
        }
        
        // Visit all the nodes to fill the children array
        XMLVisitor visitor( m_xmlDoc, *m_rootElem, children, path );
        m_rootElem->Accept( &visitor );
    }
    
    /**
     * @brief Returns the text content of an xml element
     * @return the text content of an xml element
     */
    String XMLElement::getContent()
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call getContent() in a XMLElement no correctly initialized. You should call load() before using this object)" );
            return String();
        }
        
        return m_rootElem->GetText()? m_rootElem->GetText(): "";
    }
    
    /**
     * @brief Returns an int attribute of the xml Element.
     * @param name    Name of the attribute to be returned
     * @param defaultValue Default value that will be returned in case the attribute does not exist
     * @return        an int attribute of the xml Element.
     */
    int XMLElement::getIntAttribute( const String& name, int defaultValue /*= 0 */)
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call getIntAttribute() in a XMLElement no correctly initialized. You should call load() or assign an initialized XMLElement before using this object)" );
            return defaultValue;
        }
        
        // Try to get the attribute's value
        int value = defaultValue;
        int retValue = m_rootElem->QueryIntAttribute( name.c_str(), &value );
        
        // Error check
        if ( retValue == TIXML_WRONG_TYPE )
        {
            LOG_ERROR( "XMLElement::getIntAttribute: tag %s's attribute %s type is not [int], returning default value %d", m_rootElem->Value(), name.c_str(), defaultValue );
            value = defaultValue;
        }
        else if ( retValue == TIXML_NO_ATTRIBUTE )
        {
            LOG_ERROR( "XMLElement::getIntAttribute: tag %s does not contain an attribute named %s, returning default value %d", m_rootElem->Value(), name.c_str(), defaultValue );
            value = defaultValue;
        }
        
        return value;
    }
    
    /**
     * @brief Returns a float attribute of the xml Element.
     * @param name    Name of the attribute to be returned
     * @param defaultValue Default value that will be returned in case the attribute does not exist
     * @return        a float attribute of the xml Element.
     */
    float XMLElement::getFloatAttribute( const String& name, float defaultValue /*= 0.0f*/ )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call getIntAttribute() in a XMLElement no correctly initialized. You should call load() or assign an initialized XMLElement before using this object)" );
            return defaultValue;
        }
        
        // Try to get the attribute's value
        float value = defaultValue;
        int retValue = m_rootElem->QueryFloatAttribute( name.c_str(), &value );
        
        // Error check
        if ( retValue == TIXML_WRONG_TYPE )
        {
            LOG_ERROR( "XMLElement::getFloatAttribute: tag %s's attribute %s type is not [float], returning default value %d", m_rootElem->Value(), name.c_str(), defaultValue );
            value = defaultValue;
        }
        else if ( retValue == TIXML_NO_ATTRIBUTE )
        {
            LOG_ERROR( "XMLElement::getFloatAttribute: tag %s does not contain an attribute named %s, returning default value %d", m_rootElem->Value(), name.c_str(), defaultValue );
            value = defaultValue;
        }
        return value;
    }
    
    /**
     * @brief Returns a double attribute of the xml Element.
     * @param name    Name of the attribute to be returned
     * @param defaultValue Default value that will be returned in case the attribute does not exist
     * @return        a double attribute of the xml Element.
     */
    double XMLElement::getDoubleAttribute( const String& name, double defaultValue /*= 0.0*/ )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call getIntAttribute() in a XMLElement no correctly initialized. You should call load() or assign an initialized XMLElement before using this object)" );
            return defaultValue;
        }
        
        // Try to get the attribute's value
        double value = defaultValue;
        int retValue = m_rootElem->QueryDoubleAttribute( name.c_str(), &value );
        
        // Error check
        if ( retValue == TIXML_WRONG_TYPE )
        {
            LOG_ERROR( "XMLElement::getDoubleAttribute: tag %s's attribute %s type is not [double], returning default value %d", m_rootElem->Value(), name.c_str(), defaultValue );
            value = defaultValue;
        }
        else if ( retValue == TIXML_NO_ATTRIBUTE )
        {
            LOG_ERROR( "XMLElement::getDoubleAttribute: tag %s does not contain an attribute named %s, returning default value %d", m_rootElem->Value(), name.c_str(), defaultValue );
            value = defaultValue;
        }
        return value;
    }
    
    /**
     * @brief Returns a String attribute of the xml Element.
     * @param name    Name of the attribute to be returned
     * @param defaultValue Default value that will be returned in case the attribute does not exist
     * @return        a String attribute of the xml Element.
     */
    String XMLElement::getStringAttribute( const String& name, const String& defaultValue /*= ""*/ )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call getStringAttribute() in a XMLElement no correctly initialized. You should call load() or assign an initialized XMLElement before using this object)" );
            return defaultValue;
        }
        
        // Get the attribute
        const char* result = m_rootElem->Attribute( name.c_str() );
        
        // If it does not exist, return the default value
        if ( !result )
        {
            LOG_ERROR( "XMLElement::getStringAttribute: tag %s does not contain an attribute named %s, returning default value %d", m_rootElem->Value(), name.c_str(), defaultValue.c_str() );
            return defaultValue;
        }
        return result;
    }
    
    /**
     * @brief Returns the name of the elemtn
     * @return the name of the element
     */
    String XMLElement::getName()
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call getName() in a XMLElement no correctly initialized. You should call load() before using this object)" );
            return String();
        }
        
        return m_rootElem->Value();
    }

    /*
     * Inserts a new node (xml tag) and returns it.
     */
    XMLElement XMLElement::insertChildNode ( const std::string& name )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call insertChildNode() in a XMLElement no correctly initialized. You should call load() or init() before using this object" );
            return String();
        }
        
        TiXmlElement * newNode = new TiXmlElement( name );
        m_rootElem->LinkEndChild( newNode );
        return XMLElement( newNode, m_xmlDoc );
    }
    
    /*
     * Inserts a comment in the current node.
     */
    void XMLElement::insertComment ( const std::string& text )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call insertComment() in a XMLElement no correctly initialized. You should call load() or init() before using this object" );
            return;
        }
        
        TiXmlComment * comment = new TiXmlComment();
        comment->SetValue( text );
        m_rootElem->LinkEndChild( comment );
    }    
    
    /**
     * @brief Saves this document to file
     * @param xmlFileName    Name of the file
     * @return        none
     */
    bool XMLElement::save( const std::string& xmlFileName )
    {
        // Check state
        if ( !m_bIsValid )
        {
            LOG_ERROR( "Trying to call save() in a XMLElement no correctly initialized. You should call load() or init() before using this object" );
            return false;
        }
        
        std::string path = xmlFileName;
        if ( isPathAbsolute( path ) == false )
        {
            path = dataFolder + xmlFileName;
        }
        
        if ( !m_xmlDoc->SaveFile( path.c_str() ) )
        {
            LOG_ERROR( "XMLElement: Save to file error. Check input path: %s", xmlFileName.c_str() );
            return false;
        }
        return true;
    }
    
    /**
     * @brief Sets a double attribute
     * @param name    Name of the attribute
     * @param val   Value
     */
    void XMLElement::setAttribute( const std::string& name, double val )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call setAttribute() in a XMLElement no correctly initialized. You should call load() before using this object)" );
            return;
        }
        m_rootElem->SetDoubleAttribute( name.c_str(), val );
    }
    
    /**
     * @brief Sets an int attribute
     * @param name    Name of the attribute
     * @param val   Value
     */
    void XMLElement::setAttribute( const std::string& name, int val )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call setAttribute() in a XMLElement no correctly initialized. You should call load() before using this object)" );
            return;
        }
        m_rootElem->SetAttribute( name.c_str(), val );
    }
    
    /**
     * @brief Sets an int attribute
     * @param name    Name of the attribute
     * @param val   Value
     */
    void XMLElement::setAttribute( const std::string& name, const std::string& val )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call setAttribute() in a XMLElement no correctly initialized. You should call load() before using this object)" );
            return;
        }
        m_rootElem->SetAttribute( name.c_str(), val );
    }
    
    /**
     * @brief Sets value for this element
     * @param val   Value
     */
    void XMLElement::setContent( int val )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call setContent() in a XMLElement no correctly initialized. You should call load() before using this object)" );
            return;
        }
        
		setContent( toString(val) );
    }
    
    /**
     * @brief Sets value for this element
     * @param val   Value
     */
    void XMLElement::setContent( float val )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call setContent() in a XMLElement no correctly initialized. You should call load() before using this object)" );
            return;
        }
        
		setContent( toString(val) );
    
	}
    
    /**
     * @brief Sets value for this element
     * @param val   Value
     */
    void XMLElement::setContent( double val )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call setContent() in a XMLElement no correctly initialized. You should call load() before using this object)" );
            return;
        }
    
		setContent( toString(val) );
	}
    
    /**
     * @brief Sets value for this element
     * @param val   Value
     */
    void XMLElement::setContent( const std::string& val )
    {
        // Check state
        if ( !m_rootElem )
        {
            LOG_ERROR( "Trying to call setContent() in a XMLElement no correctly initialized. You should call load() before using this object)" );
            return;
        }

        
        // Set content (or create inner node if it has none yet)
		if ( m_rootElem->FirstChild() )
            m_rootElem->FirstChild()->SetValue( toString(val) );
        else {
            TiXmlText* newText = new TiXmlText( toString(val) );
			m_rootElem->LinkEndChild(newText);
        }
	}
    
} // namespace Cing
