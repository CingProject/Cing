#include "Cing.h"
CREATE_APPLICATION( "Cing" );

/*
 * This example shows the use of the Cing::String class
 */

void setup()
{
}

void draw()
{
	// Show debut output window
	showDebugOutput();
	
	// Little instructions
	textSize(20);
	text("Mouse click will print output of some string operations on the console", 10, height/2);
}

void end()
{
}

void mousePressed()
{
	// Creating String object
	String p = "potato";

	// Comparing String objects, see reference below.
	if ( p == "potato" ) {
		println("Yes, the contents of p and potato are the same.");
	}

	// Modifying Strings
	p.toUpperCases();

	// Printing Strings
	// println send the text to the Debug Output on VStudio (windows) or the DebutOutput console if active
	println(p.toChar());
	print(" is higher case");

	// Searching indexes
	String a = "T";
	println("The letter T is the character %i" , p.indexOf( a ));
	println("Nmber of Characters in potato: %i" , p.length());
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