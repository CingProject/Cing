#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );


void setup() 
{
	//size(200, 200); 
	noStroke();
	//colorMode(RGB, 255, 255, 255, 100);
	rectMode(CENTER);
}

void draw() 
{   
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