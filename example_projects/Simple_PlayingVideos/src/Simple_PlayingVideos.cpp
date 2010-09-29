#include "Cing.h"

CREATE_APPLICATION( "Cing" );

/*
 * This example shows of to load and play a movie file using the object MediaPlayer.
 * MediaPlayer can handle a wide variety of formats (.mov, .avi, .flv...etc) and streaming of video over http.
 * It also supports audio/volume control.
 */


MediaPlayer mediaPlayer;

void setup()
{
	size ( 720, 480 );	

	// Local file
	mediaPlayer.load( "CountDown.wmv" );

	// Load video streaming through http
	// NOTE: Windows only for now...
	//mediaPlayer.load("http://www.playthemagic.com/videos/buildingmusic/testVideo.flv");

	// Play it in loop
	mediaPlayer.loop();
} 

void draw()
{
	// Draw video on mouse position
	mediaPlayer.update();
	mediaPlayer.getImage().draw(0, 0);
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