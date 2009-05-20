#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );


// The following short XML file called "xmlFileTest.xml" is parsed 
// in the code below. It must be in the project's "data" directory
// <?xml version="1.0"?>
// <xmlTest>
//  <websites>
//    <site id="0" url="www.playthemagic.com/vision">Vision</site>
//    <site id="1" url="processing.org">Processing</site>
//    <site id="2" url="mobile.processing.org">Processing Mobile</site>
//    </websites>
//  </xmlTest>

void setup()
{
  
  XMLElement xml;
	xml.load( "xmlFileTest.xml" );

  // Get element "websites" (the root is xmlTest)
  XMLElement websites = xml.getChild(0);

  // Number of websites
	int nElems = websites.getChildCount();

  // Find elements by index
  XMLElement site = websites.getChild(0);
  String siteText = site.getContent();
  println(siteText);    


  // Find elements by path (from root node, var called xml)
  XMLElement site2 = xml.getChild("websites/site");
  String site2Text = site2.getContent();
  println(site2Text);    

  // Get the name of the root noe
  println( xml.getName() );

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