#include "Cing.h"
CREATE_APPLICATION( "Cing Demo" );

/*
 * This example shows of to load and draw an image where the mouse is located
 */

Image myImage;

void setup()
{
	size(1024, 768);
	// Load the image from disk
	myImage.load("MissAmericaRedBlue.jpg");
}

void draw()
{
    // Draw the image in 2d where the mouse is
	myImage.draw(mouseX,mouseY,0);
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