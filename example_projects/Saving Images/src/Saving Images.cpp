#include "Cing.h"

CREATE_APPLICATION( "Cing" );

void setup()
{
	smooth();
}

void draw()
{
	background( 255 );

	translate( width/2, height/2);

	for (int i = 0; i < 20 ; i++)
	{
		rotate( 2*PI/20 );
		line( 0, 0, mouseX, 0);
		translate(mouseX,0,0);
		ellipse(0,0,10,10);
		translate(-mouseX,0,0);
	}


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