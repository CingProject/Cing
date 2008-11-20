#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );


void setup()
{

	background(100);
	smooth();
	noStroke();

	int diameter = 150;
	
	// limpiar esto...
	int* angs = new int[8];
	angs[0] = 30;
	angs[1] = 10;
	angs[2] = 45;
	angs[3] = 32;
	angs[4] = 60;
	angs[5] = 38;
	angs[6] = 75;
	angs[7] = 67;

	float lastAng = 0;

	for (int i = 0; i < 8 ; i++){
		fill(angs[i] * 3.0);
		arc(width/2, height/2, diameter, diameter, lastAng, lastAng+(angs[i]));
		lastAng += (angs[i]);  
	}

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