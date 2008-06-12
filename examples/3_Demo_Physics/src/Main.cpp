#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

#define MAX_ELEMENTS 300

// 3d primitives
PhysicsSphere		sphere[MAX_ELEMENTS];					// Sphere 3d primitive
PhysicsBox			box[MAX_ELEMENTS];						// Box (cube) 3d primitive
PhysicsPlane		plane;					// Plane 3d primitive
PointLight			light, light2;	// Lights in the scene

void setup()
{
	// sphere
	int coordX = -5000;
	int coordY = 100;
	for ( int i = 0; i < MAX_ELEMENTS; i++ )
	{
		sphere[i].init( 100 + random( -50, 50 ) );
		sphere[i].setPosition( coordX + random ( -20, 20 ), coordY, 200 );

		coordY += 150;
		if ( (i % 10) ==  0 )
		{
			coordX += 200;
			coordY = 100;
		}
	}

	// box
	coordX = -5000;
	coordY = 100;
	for ( int i = 0; i < MAX_ELEMENTS; i++ )
	{
		box[i].init( 100 + random( -50, 50 ) );
		box[i].setPosition( coordX + random ( -20, 20 ), coordY, 0 );
	
		coordY += 150;
		if ( (i % 10) ==  0 )
		{
			coordX += 200;
			coordY = 100;
		}
	}

	// plane
	plane.init( 100000 );
	plane.setPosition( 0, -500, 0 );
	plane.setTexture( "BeachStones.jpg" );
	plane.setSelfIlluminationColor( 60, 0, 30 );
	plane.setOrientation( Vector::UNIT_X, 20 );

	// Init lights and set the ambient light
	ambientLight( 50, 50, 50 );
	light.init( 255, 255, 255, 0, 0, 200 );	
	light2.init( 255, 100, 160, 100, 0, 200 );
}

void draw()
{
}

void end()
{
}

void mousePressed()
{
	if ( mouseButton == LEFT )
	{
		// Activate object's physics
		//sphere.activatePhysics();
		for ( int i = 0; i < MAX_ELEMENTS; i++ )
		{
			int randomN				= random( 0, 100 );
			bool staticObject = randomN > 95? true: false;
			box[i].enablePhysics( staticObject );
			sphere[i].enablePhysics( staticObject );
		}
		plane.enablePhysics( true );
	}
	else
	{
		// Deactivate object's physics
		//sphere.deActivatePhysics();
		for ( int i = 0; i < MAX_ELEMENTS; i++ )
		{
			box[i].disablePhysics();
			sphere[i].disablePhysics();
		}
		plane.disablePhysics();
	}
}

void keyPressed()
{
}