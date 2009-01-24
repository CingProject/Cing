#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

float angle = 0;
void setup()
{

	rect(0, 0, 50, 50);   //White rectangle 
	pushMatrix(); 
	translate(30, 20); 
	fill(0);  
	rect(0, 0, 50, 50);   //Black rectangle 
	popMatrix(); 
	fill(102);  
	rect(15, 10, 50, 50); //Gray rectangle

}

void draw()
{
	fill(255);
	Graphics::GraphicsManager::getSingleton().m_canvas->rect(0,0,width,height);
	angle += 0.01;

	fill(0);  

	pushMatrix(); 
	//translate(100, 100); 
	rotate(angle);
	rect(10, 10, 50, 50);   //Black rectangle 
	popMatrix(); 


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