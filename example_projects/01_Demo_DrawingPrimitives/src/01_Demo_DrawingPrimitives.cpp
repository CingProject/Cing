#include "Vision.h"
#include <gl\gl.h>
#include "Ogre3d\include\OgreRenderSystem.h"
#include "Ogre3d\include\OgreMaterialManager.h"


CREATE_APPLICATION( "Vision Demo" );

using namespace Ogre;

/*
 * This example shows of to load and draw an image where the mouse is located
 */

//Image		img; // image to load from disk and draw

MovableObject* mObject;		
const Camera* mCamera;		
SceneManager* mSceneMgr;
RenderWindow* mWindow;



void setup()
{
  //size( 800, 600 );
	//img.load( "BeachStones.jpg" );
}
/*
void PreRender() 
{
  // save matrices
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_TEXTURE);
  glPushMatrix();
  glLoadIdentity(); //Texture addressing should start out as direct.

  RenderSystem* renderSystem = mObject->_getManager()->getDestinationRenderSystem();
  Node* parentNode = mObject->getParentNode();
  renderSystem->_setWorldMatrix(parentNode->_getFullTransform());
  renderSystem->_setViewMatrix(mCamera->getViewMatrix());
  renderSystem->_setProjectionMatrix(mCamera->getProjectionMatrixRS());

  static Pass* clearPass = NULL;
  if (!clearPass)
  {
    MaterialPtr clearMat = MaterialManager::getSingleton().getByName("BaseWhite");
    clearPass = clearMat->getTechnique(0)->getPass(0);
  }
  //Set a clear pass to give the renderer a clear renderstate
  mSceneMgr->_setPass(clearPass, true, false);

  // save attribs
  glPushAttrib(GL_ALL_ATTRIB_BITS);

}

void PostRender()
{
  // restore original state
  glPopAttrib();

  // restore matrices
  glMatrixMode(GL_TEXTURE);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

void Render()
{

  GLboolean depthTestEnabled=glIsEnabled(GL_DEPTH_TEST);
  glDisable(GL_DEPTH_TEST);
  GLboolean stencilTestEnabled = glIsEnabled(GL_STENCIL_TEST);
  glDisable(GL_STENCIL_TEST);

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);

  glScalef(30.0, 30.0, 30.0);

  // I took the following sample code from here: http://www.oreillynet.com/network/2000/06/23/magazine/cube.c

  // OK, let's start drawing our planer quads.
  glBegin(GL_QUADS); 


  // Bottom Face.  Red, 75% opaque, magnified texture

  glNormal3f( 0.0f, -1.0f, 0.0f); // Needed for lighting
  glColor4f(0.9,0.2,0.2,.75); // Basic polygon color

  glTexCoord2f(0.800f, 0.800f); glVertex3f(-1.0f, -1.0f, -1.0f); 
  glTexCoord2f(0.200f, 0.800f); glVertex3f( 1.0f, -1.0f, -1.0f);
  glTexCoord2f(0.200f, 0.200f); glVertex3f( 1.0f, -1.0f,  1.0f);
  glTexCoord2f(0.800f, 0.200f); glVertex3f(-1.0f, -1.0f,  1.0f);


  // Top face; offset.  White, 50% opaque.

  glNormal3f( 0.0f, 1.0f, 0.0f);  glColor4f(0.5,0.5,0.5,.5);

  glTexCoord2f(0.005f, 1.995f); glVertex3f(-1.0f,  1.3f, -1.0f);
  glTexCoord2f(0.005f, 0.005f); glVertex3f(-1.0f,  1.3f,  1.0f);
  glTexCoord2f(1.995f, 0.005f); glVertex3f( 1.0f,  1.3f,  1.0f);
  glTexCoord2f(1.995f, 1.995f); glVertex3f( 1.0f,  1.3f, -1.0f);


  // Far face.  Green, 50% opaque, non-uniform texture cooridinates.

  glNormal3f( 0.0f, 0.0f,-1.0f);  glColor4f(0.2,0.9,0.2,.5); 

  glTexCoord2f(0.995f, 0.005f); glVertex3f(-1.0f, -1.0f, -1.3f);
  glTexCoord2f(2.995f, 2.995f); glVertex3f(-1.0f,  1.0f, -1.3f);
  glTexCoord2f(0.005f, 0.995f); glVertex3f( 1.0f,  1.0f, -1.3f);
  glTexCoord2f(0.005f, 0.005f); glVertex3f( 1.0f, -1.0f, -1.3f);


  // Right face.  Blue; 25% opaque

  glNormal3f( 1.0f, 0.0f, 0.0f);  glColor4f(0.2,0.2,0.9,.25);

  glTexCoord2f(0.995f, 0.005f); glVertex3f( 1.0f, -1.0f, -1.0f); 
  glTexCoord2f(0.995f, 0.995f); glVertex3f( 1.0f,  1.0f, -1.0f);
  glTexCoord2f(0.005f, 0.995f); glVertex3f( 1.0f,  1.0f,  1.0f);
  glTexCoord2f(0.005f, 0.005f); glVertex3f( 1.0f, -1.0f,  1.0f);


  // Front face; offset.  Multi-colored, 50% opaque.

  glNormal3f( 0.0f, 0.0f, 1.0f); 

  glColor4f( 0.9f, 0.2f, 0.2f, 0.5f);
  glTexCoord2f( 0.005f, 0.005f); glVertex3f(-1.0f, -1.0f,  1.3f);
  glColor4f( 0.2f, 0.9f, 0.2f, 0.5f);
  glTexCoord2f( 0.995f, 0.005f); glVertex3f( 1.0f, -1.0f,  1.3f);
  glColor4f( 0.2f, 0.2f, 0.9f, 0.5f);
  glTexCoord2f( 0.995f, 0.995f); glVertex3f( 1.0f,  1.0f,  1.3f); 
  glColor4f( 0.1f, 0.1f, 0.1f, 0.5f);
  glTexCoord2f( 0.005f, 0.995f); glVertex3f(-1.0f,  1.0f,  1.3f);


  // Left Face; offset.  Yellow, varying levels of opaque.

  glNormal3f(-1.0f, 0.0f, 0.0f);  

  glColor4f(0.9,0.9,0.2,0.0);
  glTexCoord2f(0.005f, 0.005f); glVertex3f(-1.3f, -1.0f, -1.0f); 
  glColor4f(0.9,0.9,0.2,0.66);
  glTexCoord2f(0.995f, 0.005f); glVertex3f(-1.3f, -1.0f,  1.0f);
  glColor4f(0.9,0.9,0.2,1.0);
  glTexCoord2f(0.995f, 0.995f); glVertex3f(-1.3f,  1.0f,  1.0f);
  glColor4f(0.9,0.9,0.2,0.33);
  glTexCoord2f(0.005f, 0.995f); glVertex3f(-1.3f,  1.0f, -1.0f);


  // All polygons have been drawn.
  glEnd();


  if (depthTestEnabled)
  {
    glEnable(GL_DEPTH_TEST);
  }
  if (stencilTestEnabled)
  {
    glEnable(GL_STENCIL_TEST);
  }


}

*/
void draw()	
{
//  PreRender();
  //Render();
  //PostRender();

	// Draw the image in 2d where the mouse is
	//img.draw2d( mouseX, mouseY, 100, 100 );
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


























//#include "Vision.h"
//CREATE_APPLICATION( "Vision Demo" );
//
///*
// * This example shows of to use the 2d drawing primitives to modify a loaded image
// */
//
//Image img, img2, img3;
//float timeVar = 0;
//
//void setup()
//{
//	// Load image from file
//	img.load( "BeachStones.jpg" );
//
//	// We can copy an image just by assigning it to another one
//	img2 = img;
//	img3 = img;
//}
//
//void draw()
//{
//	// variable to modify drawn shapes over time
//	timeVar += 0.01;
//
//	// restore original images (to draw over them)
//	img = img3;
//	img2 = img3;
//
//	// draw a line in the first image
//	strokeWeight( 8 );
//	stroke( 0, 0, 0 );
//	img.line( 256, 256, cos(timeVar)*150 + 256, sin(timeVar)*150 + 256 );
//
//	// draw some shapes in the second
//	strokeWeight(4);
//
//	stroke( 205, 205, 15 );
//	img2.ellipse( 256, 256, 50 + cos(timeVar)*20, 50 + cos(timeVar)*20 );
//
//	stroke( 205, 0, 15 );
//	img2.arc( 256, 256, 40, 40, 0, cos(timeVar)*360 );
//
//	// Size to draw the images
//	int size = 400;
//
//	// Draw a label in the first image
//	stroke( 255, 0, 0 );
//	img.text( 20, 20, "Loaded Image");
//	img.draw2d( 0, 100, size, size );
//
//	// Draw a label in the second image
//	stroke( 0, 255, 0 );
//	img2.text( 20, 20, "Cloned image" );
//	img2.draw2d( size, 100, size, size );
//}
//
//void end()
//{
//}
//
//void mousePressed()
//{
//}
//
//void mouseMoved()
//{
//}
//
//void mouseReleased()
//{
//}
//
//void keyPressed()
//{
//}
