#include "Vision.h"
#include "SegoviaParticleSystem.h"
#include "PathStorage.h"

CREATE_APPLICATION( "Play the Magic - Segovia 2008 - Noche de La Luna Llena" );

std::list< SegoviaParticleSystem* > pSystems;
std::list< PathStorage* >						paths;
bool																debugPaths, creatingNewPath;

void setup()
{
	showFps( true );

	// Disable camera control
	useMouseCameraControl( false );
	useKeyboardCameraControl( false );

	// Init vars
	debugPaths			= true;
	creatingNewPath = false;
}

void draw()
{
	// Update systems and delete dead ones
	std::list< SegoviaParticleSystem* >::iterator it = pSystems.begin();
	while( it != pSystems.end() )
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
		else
			++it;
	}
}

void end()
{
	// Save paths
	std::ofstream f ( "Paths.txt" );
	std::list< PathStorage* >::iterator it = paths.begin();
	for ( ; it != paths.end(); ++it )
		f << (*it)	;

}

void mousePressed()
{
	if ( mouseButton == LEFT )
	{
		// Creating new path
		if ( creatingNewPath )
		{
			paths.back()->addPoint( mouseX, height - mouseY, 0 );
		}
		// Testing particles
		else
		{
			// config
			int nLines = 3;
			
			// Create new particle system
			pSystems.push_back( new SegoviaParticleSystem( mouseX, height - mouseY, 0, nLines, paths ) );
		}
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

	// Configuration

	// Create new path
	else if ( key == 'p' )
	{
		// Start new path
		if ( !creatingNewPath )
		{
			creatingNewPath = true;
			paths.push_back( new PathStorage( true ) );
		}
		// End of path
		else
			creatingNewPath = false;
	}
	// Debug paths
	else if ( key == 'd' )
	{
		debugPaths = !debugPaths;
		std::for_each( paths.begin(), paths.end(), std::bind2nd( std::mem_fun( &PathStorage::setVisible ), debugPaths ) );
	}
}