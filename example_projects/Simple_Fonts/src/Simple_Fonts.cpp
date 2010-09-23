#include "Cing.h"
CREATE_APPLICATION( "Cing Font Demo" );

/*
 * This example shows of to load and draw text on screen
 */

Font calibri, tahoma;

void setup()
{
	size( 800, 600 );

	// Load fonts
	calibri.load( "calibri.ttf", 30 ); 
	tahoma.load( "tahoma.ttf", 20 );
}

void draw()	
{
	// Draw line to set the texts on top or below it
	fill(0);
	line( 0, height/2, width, height/2 );

	// Text, left-bottom aligned (on top of the line ), with tahoma font
	fill( 255, 255, 0 );
	textAlign( LEFT, BOTTOM );
	textFont( tahoma, 20 );
	text( "This a text (tahoma)... with left-bottom alignment\nwith another line here...", 0, height/2 );

	// Text, right-top aligned (below the line), with calibri font
	fill( 0, 0, 255 );
	textAlign( RIGHT, TOP );
	textFont( calibri, 30 );
	text( "This is blue line text (calibri), bigger and top-right aligned\nwith a new line here...", width,  height/2);
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
