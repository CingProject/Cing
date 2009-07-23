#include "Cing.h"

CREATE_APPLICATION( "Cing" );

MediaPlayerVLC mediaPlayer;

void setup()
{
  size ( 800, 600 );

  // Local file
  mediaPlayer.load( "test.avi" );
  mediaPlayer.play();
} 

void draw()
{
  // Draw video on mouse position
  mediaPlayer.update( );
  mediaPlayer.getImage().draw( mouseX, mouseY, 0 );
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