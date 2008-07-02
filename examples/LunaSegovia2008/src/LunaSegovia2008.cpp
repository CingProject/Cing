#include "Vision.h"
#include "SegoviaParticleSystem.h"

CREATE_APPLICATION( "Play the Magic - Segovia 2008 - Noche de La Luna Llena" );

std::list< SegoviaParticleSystem* > pSystems;

void setup()
{
	showFps( true );
}

void draw()
{
	// Update systems and delete dead ones
	std::list< SegoviaParticleSystem* >::iterator it = pSystems.begin();
	for ( ; it != pSystems.end(); ++it )
	{
		SegoviaParticleSystem* pSystem = *it;
		if ( pSystem && !pSystem->update() )
		{
			it = pSystems.erase( it );
			delete pSystem;

			// if it was the last -> do nothing else
			if ( pSystems.empty() )
				break;
		}
	}
}

void end()
{
}

void mousePressed()
{
	if ( mouseButton == LEFT )
	{
		// config
		int nLines = 3;
		
		// Create new particle system
		pSystems.push_back( new SegoviaParticleSystem( mouseX, height - mouseY, 0, nLines ) );
	}
}

void mouseMoved()
{
}

void mouseReleased()
{
}

void keyPressed()
{
	// Drawing modes
	if ( key == '1' )
		setRenderMode( DRAW_WIREFRAME );
	else if ( key == '2' )
		setRenderMode( DRAW_SOLID );
	else if ( key == '3' )
		setRenderMode( DRAW_POINTS );
}