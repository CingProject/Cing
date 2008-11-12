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
	
	for (int i = 10; i<92; i ++)
	{
		for (int j = 10; j<66; j ++)
		{		
			//cvEllipse( &mCanvas.getCVImage(),cvPoint(i*10,j*10),cvSize(5,5),0,0,360,CV_RGB(255,255,255),-1 );
			cvEllipse( &mCanvas.getCVImage(),cvPoint(i*10,j*10),cvSize(5,5),0,0,360,CV_RGB(0,0,0),1, 8, 0 );
/*
				distance = Vector(i*10 - mouseX, j*10 - mouseY,0).length()/100.0;
			  cvEllipse( &mCanvas.getCVImage(),cvPoint(i*10,j*10),cvSize(distance,distance),0,0,360,CV_RGB(255,255,255),-1 );
			  cvEllipse( &mCanvas.getCVImage(),cvPoint(i*10,j*10),cvSize(distance,distance),0,0,360,CV_RGB(0,0,0),1, 16, 0 );


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

  cvEllipse( &mCanvas.getCVImage(),cvPoint(width/2,height/2),cvSize(360,360),0,0,360,CV_RGB(0,0,0),1, 8, 0 );

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