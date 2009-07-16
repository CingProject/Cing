#include "Cing.h"

CREATE_APPLICATION( "Cing" );

void setup()
{
	Color pink = Color(255, 102, 204);
	loadPixels();
	for (int i = 0; i < (width*height/2)-width/2; i++) {
		pixels[i] = pink;
	}
	updatePixels();
}

void draw()
{
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