#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

Image		img; // image to load from disk and draw

void setup()
{
	img.load( "BeachStones.jpg" );
}

void draw()	
{
	// Draw the image in 2d where the mouse is
	img.draw2d( mouseX, mouseY, 100, 100 );
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
