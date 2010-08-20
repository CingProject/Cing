#include "Cing.h"

CREATE_APPLICATION( "Cing" );

MediaPlayer mediaPlayer;

void setup()
{
	size ( 800, 600 );	

	// Local file
	mediaPlayer.load( "test.avi" );
	//mediaPlayer.load("http://www.playthemagic.com/videos/buildingmusic/Video_Bucarest_Urban_Art_DEF_ROMANIA_Sorensen03.flv");

	mediaPlayer.loop();
} 

void draw()
{
	// Draw video on mouse position
	mediaPlayer.update( );
	mediaPlayer.getImage().draw2d( mouseX, mouseY);
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