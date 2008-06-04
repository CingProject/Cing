#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

Sphere			sphere;					// Sphere 3d primitive
Box					box;						// Box (cube) 3d primitive
PointLight	light, light2;	// Lights in the scene
Object3D    head;						// 3D head



void setup()
{
	// sphere
	sphere.init( 2 );
	sphere.setDiffuseColor( 0, 0, 200 );

	// box
	box.init( 3 );
	box.setPosition( 500, 100, 100 );
	box.setTexture( "BeachStones.jpg" );
	box.setDiffuseColor( 255, 0, 0 );


	// Init lights and set the ambient light
	ambientLight( 50, 50, 50 );
	light.init( 0, 255, 0, -100, 0, 1000 );
	light2.init( 100, 100, 160, 100, 0, 1000 );
}

void draw()
{
}

void end()
{
}

void mousePressed()
{
}

void keyPressed()
{
}