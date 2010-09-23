#include "Cing.h"
CREATE_APPLICATION( "Cing Demo" );

/*
 * This example shows the use of transformations and 2d drawing 
 * It draws a rotating rectangle on the screen. 
 */

// Used to rotate the rectangle
float angle = 0;


void setup()
{
	// Window size and smooth shapes
	size(800, 800);
	smooth();

	// Set background color
	background(200);
}

void draw()
{
	// Apply transformations before drawing
	translate (width/2,height/2);
	angle += 0.1;
	rotate(angle);
	scale(mouseX/400.0,mouseY/400.0);
	translate (100,100);

	// Finally draw the rectangle
	fill(255, 70);
	stroke(0, 70);
	rect(0,0,50,50);
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