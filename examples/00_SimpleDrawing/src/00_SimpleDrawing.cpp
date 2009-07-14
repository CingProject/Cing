// This example shows the basic about a Cing application
// It draws a line from the center of the screen to the mouse.
// Press the mouse to clear the background.

#include "Vision.h"

CREATE_APPLICATION( "Cing Demo" );

void setup()
{
	// Init values for size and stroke color
	size(800, 800);
	stroke(255);
}

void draw()
{
	// Draw a line from the center of the window to the mouse
	line(width/2, height/2, mouseX, mouseY);
}

void end()
{
}

void mousePressed()
{
	// Clear the background when the mouse is pressed
	background(192, 64, 0);
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