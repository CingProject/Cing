#include "Vision.h"
#include "BetaCairo.h"
#include <Ogre.h>

using namespace Ogre;
using namespace BetaCairo;

BetaCairo::CairoManager* mCairo;
BetaCairo::Drawing* mCanvas2d;

MovablePlane* mPlane;
Entity* mPlaneEnt;
SceneNode* mPlaneNode;

MaterialPtr VectorDrawingMaterial;

CREATE_APPLICATION( "Vision Demo" );

void setup()
{	
	showFps(true);

	// Create the Cairo Instance
	// - This is a singleton, only create it once, and delete it before you delete Root.
	mCairo = new BetaCairo::CairoManager();

	// Set the DPI to 96
	mCairo->setDPI(96.0f);

	// Create a widthxheightpx Drawing called "Vector Draw", With no alpha transparency (PF_RGB). It has 4 mipmap levels.
	mCanvas2d = mCairo->createDrawing("Vector Draw", width/2,height/2, PF_RGB, 4);

	// Set the drawing colour to a light gray.
	mCanvas2d->setColour(Colour::fromRGB(88,129,183));

	// Now fill the entire image with the colour.
	mCanvas2d->paint();

	// Set our unit scale to pixels.
	mCanvas2d->setScalar(PIXEL);

	// Set the lines we stroke to have a 2 pixel width.
	mCanvas2d->setLineThickness(2, PIXEL);

	// Set the colour to a dark gray
	mCanvas2d->setColour(Colour::fromRGB(210,144,183));

	// Move halfway across the image, on the top.
	mCanvas2d->moveTo(0,0);

	// Draw a path downwards, to the bottom.
	mCanvas2d->lineTo(10,10);

	// Stroke the lines we just drew.
	mCanvas2d->stroke();

	// Commit the changes to the Texture.
	mCanvas2d->update();

	// Create a material named "GridMaterial", based on this Drawing.
	VectorDrawingMaterial = mCairo->attachDrawingToANewMaterial("VectorDrawingMaterial", mCanvas2d);

	// Let's create large flat plane to put it on.
	mPlane = new MovablePlane("VectorDrawingPlane");
	mPlane->d = 0;
	mPlane->normal = Vector3::UNIT_Y;
	MeshManager::getSingleton().createPlane("VectorDrawingPlane",ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, *mPlane, width, height, 4, 4, true, 1, 1, 1, Vector3::UNIT_Z);
	mPlaneEnt = ogreSceneManager->createEntity("Plane", "VectorDrawingPlane");

	// Create the node and attach the plane to it.
	mPlaneNode = ogreSceneManager->getRootSceneNode()->createChildSceneNode();
	mPlaneNode->attachObject(mPlaneEnt);
	mPlaneNode->setPosition(Vector3(width/2,height/2,0));
	mPlaneNode->setDirection(0,-1,0);
	// And attach the GridMaterial to it.
	mPlaneEnt->setMaterialName("VectorDrawingMaterial");

}

void draw()
{ 
	mCanvas2d->setOperator( BetaCairo::Operator_Clear ); 
	// Draw a path downwards, to the bottom.
	mCanvas2d->lineTo(random(100),mouseY);

	// Stroke the lines we just drew.
	mCanvas2d->stroke();

	// Commit the changes to the Texture.
	mCanvas2d->update();
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