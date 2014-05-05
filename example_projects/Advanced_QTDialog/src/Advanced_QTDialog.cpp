#include "Cing.h"
CREATE_APPLICATION( "Simple_QTDialog" );

/*
 * This example shows the use of simple Dialog window created using QT
 */

#include <QApplication>
#include "QTDialogSample.h"

QTDialogSample*		dialog;	// Dialog specific class.
QApplication*		qtApp;	// If running QT on the main thread, we need the app pointer to process UI events


void setup()
{
	ShowCursor(TRUE);
	vSync(false);

	// Create QT application and control panel dialog
	int numArgs = 0;
	qtApp	= new QApplication(numArgs, NULL);
	dialog	= new QTDialogSample(NULL);

	// Show the dialog and set this Control Panel as the event handler for UI events
	if ( dialog )
	{
		dialog->show();
		dialog->setModal(false);
	}

}

void draw()
{
	// In this sample QT is running on the main thread, so we need to process the UI events here to keep UI responsive
	if ( qtApp )
	{
		qtApp->processEvents();
	}

	
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
	if ( toupper(key) == 'D' )
	{
		// Restore the window if D is pressed
		if ( dialog )
		{
			dialog->restore();
		}
	}
}