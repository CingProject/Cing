/*
 * This example shows the basics about a Cing application subclassing instead of using
 * the Processing like function approach to control the application flow
 * It draws a line from the center of the screen to the mouse.
 * Press the mouse to clear the background.
 */

#pragma once


class SimpleApp: public UserApplicationBase
{
public:
    // Lifecycle
    void setup          ();
    void draw           ();
    //void end            ();
    
    // Events
    void mousePressed   ();
    //void mouseMoved     ();
    //void mouseReleased  ();
    void keyPressed     ();
};
