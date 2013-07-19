#include "Cing.h"
CREATE_APPLICATION( "Cing" );

/*   
 * This example shows how to use shadows in Cing (Stencil Shadows in this case)
 * keys: 
 *	- o: shadows on/off
 *	- 1: set light attenuation
 *	- 2: remove light attenuation (light never ends)
 */

PointLight			light;
Box					box;
Sphere				sphere1, sphere2, sphere3;
float				angle = 0.02;
Box					ground;
bool				shadowsOn = true;

void setup()
{
	size(1024, 768);
	background(0);
	enableShadows( STENCIL_MODULATIVE );
	applyCoordinateSystemTransform(OPENGL3D);

	// Setup lighting
	ambientLight(10);
	light.init(255, 255, 255, width/2 + 100, height/2 + 100, 100);
	
	// Uncomment this line to see the position of the light
	light.drawDebug();

	// Init 3d object's properties
	box.init(10, 4, 14);
	box.setPosition(width/2.0f, 100);
	sphere1.init(6);
	sphere2.init(12);
	sphere3.init(20);

	// Now, we make the spheres children of the box's (scene node)
	box.addChild( sphere1 );
	box.addChild( sphere2 );
	box.addChild( sphere3 );

	// Translate the sphere (relative to its parent, the box)
	// This way, when we rotate the box (parent object), the spheres will orbitate around it
	sphere1.setPosition( 2, -2, 0 );
	sphere2.setPosition( 5, -7, 0 );
	sphere3.setPosition( 7, -12, 0 );
	
	// Ground
	ground.init(5000, 25, 5000 );
	ground.setPosition( width/2, -200 );

	// Camera control
	enableDefault3DCameraControl();
}

void draw()
{
	// Rotate box and position (and the spheres will orbitate around it)
	box.rotateY( angle );
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
	// Turn light shadows on/off
	if ( key == 'o' )
	{
		shadowsOn = !shadowsOn;
		light.castShadows(shadowsOn);
	}
	// You can control the light attenuation over distance
	else if ( key == '1' )
	{
		// With some attenuation
		light.setAttenuation( 100000, 1, 0.0025, 0 );
	}
	else if ( key == '2' )
	{
		// With no attenuation
		light.setAttenuation( 100000, 1, 0, 0 );
	}
}