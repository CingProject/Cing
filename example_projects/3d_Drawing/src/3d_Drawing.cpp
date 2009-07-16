#include "Cing.h"

CREATE_APPLICATION( "Cing" );

void setup()
{
	size( 800, 600 );
	showFps(true);
}

void draw()
{
	fill(255,255,255);

	stroke(255,0,0);
	line(0, 0, 0,1000, 0, 0);

	stroke(0,255,0);
	line(0, 0, 0, 0, 1000, 0);	

	stroke(0,0,255);
	line(0, 0, 0,0, 0, -1000);	
   
	stroke(0,0,0);

	beginShape();
	vertex(10, 10);
	vertex(110, 10);
	vertex(110, 110);
	vertex(10, 110);
	endShape(CLOSE);

	beginShape(POINTS);
	vertex(130, 10);
	vertex(230, 10);
	vertex(230, 110);
	vertex(130, 110);
	endShape(CLOSE);

	beginShape(LINES);
	vertex(340, 10);
	vertex(440, 10);
	vertex(440, 110);
	vertex(340, 110);
	endShape();

	noFill();
	beginShape();
	vertex(450, 10);
	vertex(550, 10);
	vertex(550, 110);
	vertex(450, 110);
	endShape();
	
	noFill();
	beginShape();
	vertex(560, 10);
	vertex(660, 10);
	vertex(660, 110);
	vertex(560, 110);
	endShape(CLOSE);

	fill(255,255,255);
	beginShape(TRIANGLES);
	vertex(30, 175);
	vertex(40, 120);
	vertex(50, 175);
	vertex(60, 120);
	vertex(70, 175);
	vertex(80, 120);
	endShape();

	beginShape(TRIANGLE_STRIP);
	vertex(130, 175);
	vertex(140, 120);
	vertex(150, 175);
	vertex(160, 120);
	vertex(170, 175);
	vertex(180, 120);
	vertex(190, 175);
	endShape();

	beginShape(TRIANGLE_FAN);
	vertex(257.5, 150);
	vertex(257.5, 115); 
	vertex(292, 150); 
	vertex(257.5, 185); 
	vertex(222, 150); 
	vertex(257.5, 115); 
	endShape();
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