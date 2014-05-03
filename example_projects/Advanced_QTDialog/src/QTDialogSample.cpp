// Precompiled headers
#include "Cing-Precompiled.h"

#include "QTDialogSample.h"

#include <QMatrix>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

#include "ui_ControlPanelDialog.h"


//-------------------------------------------------------------------------------------------------
QTDialogSample::QTDialogSample(QWidget* parent)
	: QDialog(parent)
{

	// Create the UI for the form
	m_ui = new Ui::ControlPanelDialog();
	m_ui->setupUi(this);

	//// Set the dialog so that it is not resizable
	//m_ui->layoutWidget->setFixedSize( sizeHint() );

	// Inverval for update on this class (for example to update GUI elements based on app state)
	m_timer.setInterval(33);	//< update the window at 10 Hz
	m_timer.start();
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(Update()));
	

	// connect the widgets with the local handlers
	connect(m_ui->randomBackgroundColor, SIGNAL(clicked()), this, SLOT(setRandomBackgroundColor()));

 //   connect(m_ui->goToStateButton, SIGNAL(clicked()), this, SLOT(GoToState()));
	//connect(m_ui->useShortTransitionsCheckbox, SIGNAL(clicked()), this, SLOT(UseShortTransitionsChanged()));
	//connect(m_ui->startIllusionButton, SIGNAL(clicked()), this, SLOT(StartIllusionXML()));
	//connect(m_ui->fakeSignalGenerationEnabled, SIGNAL(clicked()), this, SLOT(FakeSignalGenerationEnabledClicked()));
	//connect(m_ui->meditionsVersion_1, SIGNAL(clicked()), this, SLOT(SelectedTrickStateOption1()));
	//connect(m_ui->meditionsVersion_2, SIGNAL(clicked()), this, SLOT(SelectedTrickStateOption2()));
	//connect(m_ui->meditionsVersion_3, SIGNAL(clicked()), this, SLOT(SelectedTrickStateOption3()));
	//connect(m_ui->cameraZoom, SIGNAL(valueChanged(int)), this, SLOT(UpdateInputCamParams(int)));
	//connect(m_ui->centerXSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateInputCamParams(int)));
	//connect(m_ui->centerYSlider, SIGNAL(valueChanged(int)), this, SLOT(UpdateInputCamParams(int)));
	//connect(m_ui->resultsSpinBox, SIGNAL(valueChanged(int)), this, SLOT(BrainTypeInfoChanged(int)));
	//connect(m_ui->signalMultiplier, SIGNAL(valueChanged(int)), this, SLOT(SignalMultiplierChanged(int)));
	//connect(m_ui->resultsComboForced, SIGNAL(clicked()), this, SLOT(ForceBrainTypeChanged()));
	//connect(m_ui->resultsComboReal, SIGNAL(clicked()), this, SLOT(ForceBrainTypeChanged()));
	//connect(m_ui->selectScreenshot01, SIGNAL(clicked()), this, SLOT(SelectScreenshot01()));
	//connect(m_ui->selectScreenshot02, SIGNAL(clicked()), this, SLOT(SelectScreenshot02()));
	//connect(m_ui->selectScreenshot03, SIGNAL(clicked()), this, SLOT(SelectScreenshot03()));
	//connect(m_ui->selectScreenshot04, SIGNAL(clicked()), this, SLOT(SelectScreenshot04()));
	//connect(m_ui->selectScreenshot05, SIGNAL(clicked()), this, SLOT(SelectScreenshot05()));
	//connect(m_ui->takeScreenshot01, SIGNAL(clicked()), this, SLOT(TakeScreenshot01()));
	//connect(m_ui->takeScreenshot02, SIGNAL(clicked()), this, SLOT(TakeScreenshot02()));
	//connect(m_ui->takeScreenshot03, SIGNAL(clicked()), this, SLOT(TakeScreenshot03()));
	//connect(m_ui->takeScreenshot04, SIGNAL(clicked()), this, SLOT(TakeScreenshot04()));
	//connect(m_ui->takeScreenshot05, SIGNAL(clicked()), this, SLOT(TakeScreenshot05()));
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
