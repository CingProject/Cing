#include "Vision.h"
#include "BetaCairo.h"
#include <Ogre.h>

using namespace Ogre;
using namespace BetaCairo;

CairoManager* mCairo;
Drawing* mCanvas2d;

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

	
	// Create a widthXheight drawing called "Vector Draw", With no alpha transparency (PF_RGB). It has 4 mipmap levels.
	mCanvas2d = mCairo->createDrawing("Vector Draw", width,height, PF_RGB, 0);
	mCanvas2d->setAntiAliasing(AA_None);

  cairo_set_tolerance (mCanvas2d->getCairoT(),1.0);

	// Set our unit scale to pixels.
	mCanvas2d->setScalar(PIXEL);

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
	// Clear drawing
	mCanvas2d->setOperator( BetaCairo::Operator_Clear );
	mCanvas2d->paint();
	mCanvas2d->setOperator( BetaCairo::Operator_Source );
	
	// Set the drawing colour to a light gray.
	mCanvas2d->setColour(Colour::fromRGB(135,135,135));
	mCanvas2d->paint();
  

	mCanvas2d->setColour(Colour::fromRGB(0,0,0));
	
	// Set the lines we stroke to have a 2 pixel width.
	mCanvas2d->setLineThickness(1, PIXEL);

	for (int i = 10; i<92; i ++)
	{
		for (int j = 10; j<66; j ++)
		{
			mCanvas2d->arc(i*10, j*10, 5.0, 0, 360);
			mCanvas2d->stroke();	
		} 
	} 

	mCanvas2d->setColour(Colour::fromRGB(0,0,0));

	mCanvas2d->arc(width/2, height/2, 380.0, 0, 360);
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