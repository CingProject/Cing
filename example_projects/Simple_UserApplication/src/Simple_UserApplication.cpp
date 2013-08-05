#include "Cing.h"
#include "Simple_UserApplication.h"

CREATE_USER_APPLICATION( "Cing Demo", SimpleApp );

/*
 * This example shows the basics about a Cing application subclassing instead of using
 * the Processing like function approach to control the application flow
 * It draws a line from the center of the screen to the mouse.
 * Press the mouse to clear the background.
 */


void SimpleApp::setup()
{
	// Set window size and smooth drawing
	size(800, 800);
	smooth();
}

void SimpleApp::draw()
{
	// Associate the stroke grayscale value to the vertical position of the mouse
	float value = map(mouseY, 0, height, 0, 255);
	stroke(value);

	// Draw a line from the center of the window to the mouse
	strokeWeight(2);
	line(width/2, height/2, mouseX, mouseY);
}


void SimpleApp::mousePressed()
{
	// Clear the background when the mouse is pressed
	background(192, 64, 0);
}
