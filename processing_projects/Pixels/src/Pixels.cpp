// Code from: http://processing.org/reference/pixels.html

// Using the pixels array

#include "Cing.h"
CREATE_APPLICATION( "Cing" );

void setup() {
	size(200,200);

	Color pink = Color(255, 102, 204);
	loadPixels();
	for (int i = 0; i < (width*height/2)-width/2; i++) {
		pixels[i] = pink;
	}
	updatePixels();
}

void draw() {
}

void end() {
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