#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

Image img, img2;
Image* img3;
float pTime = 0;
Graphics::ImageThresholdFilter   m_imgThresholdFilter; ///< Image to apply thresholding (posterizing) of an image

void setup()
{
	img.load( "BeachStones.jpg" );
	img2 = img;
	img3 = new Image( img2 );
}

void draw()
{
	pTime+= 0.01;

	// clone image 1 into img2
	img2 = img;

	// draw a line
	strokeWeight(8);
	stroke(0,0,0);
	img2.line(256,256,cos(pTime)*150+256,sin(pTime)*150+256);

	// draw some shapes
	strokeWeight(4);
	stroke(205,205,15);
	img2.ellipse(256,256,50 + cos(pTime)*20,50 + cos(pTime)*20);
	img2.arc(256,256,40,40,0,cos(pTime)*360);

	
	stroke(255,0,0);
	img.text( 20,20, "Imagen Cargada");
	img.draw( 0, 0 );

	stroke(0,255,0);
	img2.text(20,42, "Imagen Dinamica");
	
	img2.filter(BLUR);

	img2.draw( 512, 0, 0 );

	stroke(0,0,255);
	img3->text(20,20,"Imagen Clonada");
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
