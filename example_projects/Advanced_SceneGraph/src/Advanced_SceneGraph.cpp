#include "Cing.h"
CREATE_APPLICATION( "Cing" );

/*   
 * This example shows how to use the SceneGraph capabilities
 * Generally speaking, a scene graph is a structure that arranges the spatial representation of a graphical scene.
 * Cing is based on Ogre, which is a 3d graphics engine that organizes the 3d objects in a Scene Graph.
 * A Scene Graph creates hierarchies (trees) of Scene Nodes, and the 3d objects are attached to these Nodes.
 * Parent nodes affect child nodes and this can be used to create complex transformations 
 *
 * This is a simple example to highlight some of the possibilities that a Scene Graph offers and how to access them
 */

PointLight	light;
Box			box;
Sphere		sphere1, sphere2, sphere3;
float		angle = 0.02;

void setup()
{
	size(1024, 768);
	background(0);

	// Setup lighting
	ambientLight(30);
	light.init(200, 200, 200, 0, 3, 0);

	// Uncomment this line to see the position of the light
	//light.drawDebug(true);

	// Init 3d object's properties
	box.init(50, 25, 75);
	box.setPosition(width/2.0f, height/2.0f);

	sphere1.init(30);
	sphere2.init(60);
	sphere3.init(100);

	// Now, we make the spheres children of the box's (scene node)
	box.addChild( sphere1 );
	box.addChild( sphere2 );
	box.addChild( sphere3 );

	// Translate the sphere (relative to its parent, the box)
	// This way, when we rotate the box (parent object), the spheres will orbitate around it
	sphere1.setPosition( 2, 0, 0 );
	sphere2.setPosition( 5, 0, 0 );
	sphere3.setPosition( 7, 0, 0 );

	// Add the second light as child of one of the spheres
	sphere1.addChild( light );
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
}