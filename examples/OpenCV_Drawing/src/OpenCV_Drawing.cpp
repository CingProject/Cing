#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );
Graphics::Image mCanvas;

void setup()
{
	mCanvas.init( width, height, RGB);
	showFps(true);
}

void draw()
{
	mCanvas = 155;
	float distance = 0.0;
	for (int i = 3; i<100; i ++)
	{
		for (int j = 3; j<74; j ++)
		{		

				distance = Vector(i*10 - mouseX, j*10 - mouseY,0).length()/100.0;
			  cvEllipse( &mCanvas.getCVImage(),cvPoint(i*10,j*10),cvSize(distance,distance),0,0,360,CV_RGB(255,255,255),-1 );
			  cvEllipse( &mCanvas.getCVImage(),cvPoint(i*10,j*10),cvSize(distance,distance),0,0,360,CV_RGB(0,0,0),1, 16, 0 );
/*
			cvEllipseAA( &mCanvas.getCVImage(),  ///-> Image.
									 cvPoint(i*10,j*10),		  ///-> Center of the ellipse.
									 cvSize(5,5),		        ///-> Length of the ellipse axes. 
									 0,											///->	Rotation angle.
									 0,											///-> Starting angle of the elliptic arc.
									 360,										///-> Ending angle of the elliptic arc.
									 0,
									 0);									
*/
		} 
	} 
	mCanvas.updateTexture();
	mCanvas.draw2d(0,0,width,height);
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