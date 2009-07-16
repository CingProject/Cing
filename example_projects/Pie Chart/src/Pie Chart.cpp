#include "Cing.h"

CREATE_APPLICATION( "Cing" );


void setup()
{

	background(100);
	smooth();
	noStroke();

	int diameter = 150;
	
	// TODO: clean up
	int angs[] = {30, 10, 45, 35, 60, 38, 75, 67};

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