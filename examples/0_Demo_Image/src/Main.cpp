#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

Image img, img2;
Image* img3;
float pTime = 0;
void setup()
{
	img.load( "BeachStones.jpg" );
	img2 = img;
	img2.setUpdateTexture(true); 
	img3 = new Image( img2 );
	img3->setUpdateTexture(true); 
}

void draw()
{
	// draw loaded image
	img.draw( 0, 0 );
	
	// copy img into img2 , maybe its clear some like... img2 = img.clone()
	img2 = img;

	// draw an animated line inside img2
	pTime+= 0.01;
	img2.line(256,256,cos(pTime)*150+256,sin(pTime)*150+256);
	
	// draw the image
	img2.draw( -320, -320, 100 );
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