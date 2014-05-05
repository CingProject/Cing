// Precompiled headers
#include "Cing-Precompiled.h"

#include "QTDialogSample.h"

#include <QMatrix>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

#include "ui_Dialog.h"


//-------------------------------------------------------------------------------------------------
QTDialogSample::QTDialogSample(QWidget* parent)
	: QDialog(parent)
{

	// Create the UI for the form
	m_ui = new Ui::Dialog();
	m_ui->setupUi(this);

	//// Set the dialog so that it is not resizable
	//m_ui->layoutWidget->setFixedSize( sizeHint() );

	// Inverval for update on this class (for example to update GUI elements based on app state)
	m_timer.setInterval(33);	//< update the window at 10 Hz
	m_timer.start();
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(Update()));

	// connect the widgets with the local handlers
	connect(m_ui->randomBackgroundColor, SIGNAL(clicked()), this, SLOT(setRandomBackgroundColor()));
	connect(m_ui->whiteLevel, SIGNAL(valueChanged(int)), this, SLOT(setWhiteLevel(int)));
}

//-------------------------------------------------------------------------------------------------
QTDialogSample::~QTDialogSample()
{
}



//-------------------------------------------------------------------------------------------------
void QTDialogSample::Update()
{
	// update the fps
	double fps = getFps();
	QString fpsStr = "FPS = ";
	fpsStr += QString::number(fps, 3, 2);
	m_ui->fpsLabel->setText(fpsStr);

}


//-------------------------------------------------------------------------------------------------
void QTDialogSample::reject()
{
	QSettings settings("Cing", "QTDialogSample");
	settings.setValue("mainWindowGeometry", saveGeometry());
	QDialog::reject();
	ShowCursor(FALSE);
}

//-------------------------------------------------------------------------------------------------
void QTDialogSample::restore(bool setGeometry /*= true*/)
{
	if (setGeometry)
	{
		QSettings settings("Cing", "QTDialogSample");
	    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
	}
	QDialog::show();
	ShowCursor(TRUE);
}

//-------------------------------------------------------------------------------------------------
void QTDialogSample::setRandomBackgroundColor()
{
	float r = random(0, 255);
	float g = random(0, 255);
	float b = random(0, 255);
	background(r, g, b);
}

//-------------------------------------------------------------------------------------------------
void QTDialogSample::setWhiteLevel(int newValue)
{
	background( newValue, newValue, newValue );
}

