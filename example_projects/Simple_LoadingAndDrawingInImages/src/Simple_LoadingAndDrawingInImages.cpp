#include "Cing.h"
CREATE_APPLICATION( "Cing Demo" );

/*
 * This example shows of to load and draw an image where the mouse is located
 * The image should be on the data folder of the project.
 */

Image myImage;

void setup()
{
	size(1024, 768);

	// Load the image from disk
	//myImage = loadImage("muybridge.jpg");
	myImage.load("muybridge.jpg");

}

void draw()
{
    // Draw the image in 2d where the mouse is located
	myImage.draw(mouseX,mouseY);
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