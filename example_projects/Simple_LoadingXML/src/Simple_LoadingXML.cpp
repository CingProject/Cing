#include "Cing.h"
CREATE_APPLICATION( "Cing" );

/*
 * Example extracted from http://processing.org/reference/XMLElement.html
 * The following short XML file called "xmlFileTest.xml" is parsed 
 * in the code below. It must be in the project's "data" directory
 *
 * <?xml version="1.0"?>
 * <xmlTest>
 *  <websites>
 *    <site id="0" url="cing.cc">Cing</site>
 *    <site id="1" url="processing.org">Processing</site>
 *    <site id="2" url="mobile.processing.org">Processing Mobile</site>
 *   </websites>
 * </xmlTest>
 *
 */

XMLElement xml;

void setup()
{
	// Load xml from data folder
	xml.load( "xmlFileTest.xml" );

	// Size of th text to print on scren
	textSize( 20 );
}

void draw()
{
	// Find elements by index or path (path in this case)
	XMLElement websites = xml.getChild("websites");

	// Print content of xml file
	text( "XML nodes and content from file xmlFileTest in data folder", 100, 50 );

	// Get first site
	XMLElement site1 = websites.getChild(0);
	String site1Text = site1.getContent();
	text( site1Text, 120, 100 );
	text( "ID: " + intToString( site1.getIntAttribute( "id" ) ), 220, 100 );
	text( "Url: " + site1.getStringAttribute( "url" ), 270, 100 );

	// Get second  site
	XMLElement site2 = websites.getChild(1);
	String site2Text = site2.getContent();
	text( site2Text, 120, 120 );
	text( "ID: " + intToString( site2.getIntAttribute( "id" ) ), 220, 120 );
	text( "Url: " + site2.getStringAttribute( "url" ), 270, 120 );
}

void end()
{
}

void mousePressed()
{
}

void mouseMoved()
{
}

void mouseReleased()
{
}

void keyPressed()
{
}