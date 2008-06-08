#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

Image img, img2;
Image* img3;
float pTime = 0;
void setup()
{
	img.load( "BeachStones.jpg" );
	img2 = img;

	// draw a border
	stroke(0,14,25);
	img2.rect(4,4,508,508);
	img3 = new Image( img2 );
}

void draw()
{
	pTime+= 0.01;

	img.draw( 0, 0 );

	// clone image 1 into img2
	img2 = img;

	// draw a line
	strokeWeight(8);
	stroke(0,0,0);
	img2.line(256,256,cos(pTime)*150+256,sin(pTime)*150+256);

	// draw a circle
	strokeWeight(4);
	stroke(205,205,15);
	img2.ellipse(256,256,50 + cos(pTime)*20,50 + cos(pTime)*20);

	// draw the image
	img2.draw( 512, 0, 0 );

	img3->draw( 1024, 0, 0 );
}

void end()
{
}

void mousePressed()
{
	//img.save( "test.tif" );
}

void keyPressed()
{
}
