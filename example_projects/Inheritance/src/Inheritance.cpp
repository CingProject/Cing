#include "Cing.h"
#include "Ogre3d/include/OgreMatrix4.h"
CREATE_APPLICATION( "Cing" );




class Spin 
{
public:
	float x, y, speed;
	float angle;
	Spin(){};
	Spin(float xpos, float ypos, float s)
	{
		x = xpos;
		y = ypos;
		speed = s;
		angle = 0.0;
	}
	void update() {
		angle += speed;
	}
};

class SpinArm: public Spin 
{
	public:
	SpinArm(){};
	SpinArm(Spin input){};
	SpinArm(float x, float y, float s)
	{
	};
	void display() {
		strokeWeight(1);
		stroke(0);

		pushMatrix();
		//translate(x, y);
		angle += speed;
		//rotate(angle);
		line(0, 0, 66, 0);
		popMatrix();	
	}
};

class SpinSpots: public Spin 
{
	public:
	float dim;
	SpinSpots(){};
	SpinSpots(Spin input){};
	SpinSpots(float x, float y, float s, float d)
	{
		dim = d;
	}
	void display() {
		noStroke();

		pushMatrix();
		//translate(x, y);
		angle += speed;
		//rotate(angle);
		ellipse(-dim/2, 0, dim, dim);
		ellipse(dim/2, 0, dim, dim);
	  popMatrix();
	}
};

SpinSpots spots;
SpinArm   arm;

void setup() 
{
	//size(200, 200);
	smooth();
	arm   = SpinArm(width/2, height/2, 0.01);
	spots = SpinSpots(width/2, height/2, -0.02, 33.0);
}



void draw() 
{
	background(204);
	arm.update();
	arm.display();
	spots.update();
	spots.display();

	//line(0,0,0,10,10,10);
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