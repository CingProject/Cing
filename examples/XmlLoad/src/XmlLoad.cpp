#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );


// The following short XML file called "sites.xml" is parsed 
// in the code below. It must be in the project's "data" directory
// <?xml version="1.0"?>
// <websites>
//   <site id="0" url="processing.org">Processing</site>
//   <site id="1" url="mobile.processing.org">Processing Mobile</site>
// </websites>

//XMLElement xml;
//xml.load( "xmlFileTest.xml" );
//int nElems = xml.getChildCount();
//int a = 0;
void setup()
{
	XMLElement xml;
	xml.load( "xmlFileTest.xml" );
	int nElems = xml.getChildCount();
	int a = 0;
}

void draw()
{
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