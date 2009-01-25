#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

// Angle variable
float angle = 0;

Box myBox;
void setup()
{
	smooth();
	angle += 0.001;
}

void draw()
{
	// Clear background with gray
	//background(100);

	angle += 0.01;

	// Apply transforms and draw
	pushMatrix(); 
		rotate(angle);
		translate( 100, 100 );
		scale(1,2);
		stroke(0);
		fill(255);
		rect(0, 0, 100, 100);
			pushMatrix();
				rotate(angle);
				translate( 100, 100 );
				scale(1,2);
				stroke(0);
				fill(255);
				rect(0, 0, 100, 100);
			popMatrix();
	popMatrix();
		//  rect(100, 100, 110, 110);
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