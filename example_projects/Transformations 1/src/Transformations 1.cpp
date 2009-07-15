#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );
/*
// Angle variable
float angle = 0;


void setup()
{
	smooth();
	angle += 0.001;
}

void draw()
{
	angle += 0.01;

	// Apply transforms and draw
	pushMatrix(); 
		rotate(angle);
		translate( 10, 10 );
		scale(3,1);
		stroke(0);
		fill(255);
		rect(0, 0, 100, 100);
			pushMatrix();
				rotate(angle);
				translate( 100, 100 );
				scale(3,1);
				stroke(0);
				fill(255);
				rect(0, 0, 100, 100);
			popMatrix();
	popMatrix();
}*/

float angle = 0;

void setup()
{
	  smooth();
}

void draw()
{
	angle += 0.1;
	rotate( angle);
	scale(2,3);
	translate (100,100);
	ellipse(0,0,50,80);
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