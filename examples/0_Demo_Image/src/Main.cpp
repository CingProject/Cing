#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

Image img, img2;
Image* img3;
void setup()
{
	img.load( "BeachStones.jpg" );
	img2 = img;
	img3 = new Image( img2 );
}

void draw()
{
	img.draw( 0, 0 );
	img2.draw( 100, 100 );
	img3->draw( -200, -200, 100 );
	
}

void end()
{
}

void mousePressed()
{
	img.save( "test.tif" );
}

void keyPressed()
{
}