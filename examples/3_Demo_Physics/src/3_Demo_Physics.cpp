#include "Vision.h"

CREATE_APPLICATION( "Vision Demo" );

// Number of spheres and boxes that are goint to be created
#define MAX_ELEMENTS 300

// 3d primitives
PhysicsSphere		sphere[MAX_ELEMENTS];	// SphereS
PhysicsBox			box[MAX_ELEMENTS];		// Boxes 
PhysicsPlane		plane;								// Plane 
PointLight			light;								// Lights in the scene

void setup()
{
	// init spheres and boxes
	int coordX = -5000;
	int coordY = 100;
	for ( int i = 0; i < MAX_ELEMENTS; i++ )
	{
		// Set size and position (with some random)
		sphere[i].init( 100 + random( -50, 50 ) );
		sphere[i].setPosition( coordX + random ( -20, 20 ), coordY, 200 );

		box[i].init( 100 + random( -50, 50 ) );
		box[i].setPosition( coordX + random ( -20, 20 ), coordY, 0 );

		// Control coordinates to place them in a grid way
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
	plane.setOrientation( Vector::UNIT_X, 20 );

	// Init lights and set the ambient light
	ambientLight( 50, 50, 50 );
	light.init( 255, 255, 255, 0, 0, 200 );	
}

void draw()
{
}

void end()
{
}

void mousePressed()
{
	// Activate or deactivate physics
	if ( mouseButton == LEFT )
	{
		// Activate object's physics
		for ( int i = 0; i < MAX_ELEMENTS; i++ )
		{
			// some of the objects will be static
			int randomN				= random( 0, 100 );
			bool staticObject = randomN > 95? true: false;

			// Enable the physics
			box[i].enablePhysics( staticObject );
			sphere[i].enablePhysics( staticObject );
		}

		// Enable physics plate as static
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
	// Apply force to all objects when space bar is pressed
	if ( key == ' ' )
	{
		float forceScale = 100;
		for ( int i = 0; i < MAX_ELEMENTS; i++ )
		{
			box[i].applyCentralImpulse( Vector::UNIT_Y * forceScale );
			sphere[i].applyCentralImpulse( Vector::UNIT_Y * forceScale );
		}
	}
}