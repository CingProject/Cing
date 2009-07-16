#include "Cing.h"
CREATE_APPLICATION( "Cing" );

/*
 * This example shows of to load and draw an image where the mouse is located
 */
 
Image		img; // image to load from disk and draw

void setup()
{
  size( 800, 600 );
	img.load( "BeachStones.jpg" );
}

void draw()	
{
	// Draw the image in 2d where the mouse is
	img.draw( mouseX, mouseY, 0, 100, 100 );
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
