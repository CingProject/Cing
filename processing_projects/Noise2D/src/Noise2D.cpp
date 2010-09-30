// Code from: http://processing.org/learning/basics/noise2d.html

// Noise2D by Daniel Shiffman.
// Using 2D noise to create simple texture.

#include "Cing.h"
CREATE_APPLICATION( "Cing" );

float increment = 0.02;

void setup() {
	size(200,200);
}

void draw() {
	background(0);

	loadPixels();

	float xoff = 0.0; // Start xoff at 0
	float yoff = 0.0; 
	float bright = 0.0f;

	// For every x,y coordinate in a 2D space, calculate a noise value and produce a brightness value
	for (int x = 0; x < width; x++) {

		xoff += increment;		// Increment xoff 
		yoff = 0.0;				// For every xoff, start yoff at 0
		for (int y = 0; y < height; y++) {
			yoff += increment; // Increment yoff

			// Calculate noise and scale by 255
			bright = noise(xoff+mouseX,yoff+mouseY)*255;

			// Try using this line instead
			// bright = random(0,255);

			// Set each pixel onscreen to a grayscale value
			pixels[x+y*width] = color(bright);
		}
	}

	updatePixels();
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