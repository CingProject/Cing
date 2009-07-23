// Code from: http://processing.org/learning/basics/mouse2d.html

// Moving the mouse changes the position and size of each box.

#include "Cing.h"
CREATE_APPLICATION( "Cing" );

void setup() {
	size(200, 200); 
	noStroke();
	rectMode(CENTER);
}

void draw() {   
	background(51); 
	fill(255, 80);
	rect(mouseX, height/2, mouseY/2+10, mouseY/2+10);
	fill(255, 80);
	int inverseX = width-mouseX;
	int inverseY = height-mouseY;
	rect(inverseX, height/2, (inverseY/2)+10, (inverseY/2)+10);
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