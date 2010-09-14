#include "Cing.h"
CREATE_APPLICATION( "Cing Demo" );

/*   
 * This example shows of the creation of 3dObjects and 
 * how to control their position and other attributes
 */

Sphere		spheres[110];		// Container of the 3d objects
int			nSpheres	= 110;  // Number of spheres
float		diameter	= 30;   // Radius
float		maxDist		= 160;  // Max distance between connections
float		ranValue	= 3;    // Random factor
PointLight	light;				// Lights in the scene

void setup() {

	size(1024, 768);
	background(94);

	// Remove ambient light
	ambientLight(0);

	// Set light position and color
	light.init(0, 255, 255, 0, 0, 200);

	// Create spheres (set their position
	for (int i=0; i< nSpheres; i++) {		
		// Init sphere properties
		spheres[i].init(diameter);
		spheres[i].setPosition( random(100,width-100), random(100,height-100), random(30, -30));
	}
}

void draw() {

	// Update light pos to follow the mouse
	light.setPosition( mouseX, mouseY, 200 );

	// Update sphere positions
	for (int i=0; i< nSpheres; i++) {

		// modificamos su posicion aleatoriamente
		Vector pos = spheres[i].getPosition();
		pos.x	+= random(-ranValue,ranValue);
		pos.y	+= random(-ranValue,ranValue);
		pos.z	+= random(-ranValue,ranValue);

		// Constrain positions in x and y
		pos.x = constrain( pos.x, 100, width-100);
		pos.y = constrain( pos.y, 100, height-100);

		// Set sphere position
		spheres[i].setPosition(pos);

		// Now, using line primitives in 3d, we will connect all the spheres that are close
	    for (int j = 0; j < nSpheres; j++) {
			// Do not connect a sphere with itself!
			if (i!=j) {
				Vector otherPos = spheres[j].getPosition();

				// Calculate distance between the spheres
				float dist = pos.distance(otherPos);

				// If the distnace is lower than maxDist -> draw the connection
				if ( dist < maxDist ) {
					stroke(255-dist,255-dist);
					line( pos.x, pos.y, pos.z, otherPos.x, otherPos.y, otherPos.z);
				}
			}	
		}
	}

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