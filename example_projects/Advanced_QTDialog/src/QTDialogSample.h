#ifndef QTDialogSample_H
#define QTDialogSample_H

#include <QDialog>
#include <QImage>
#include <QLineEdit>
#include <QTimer>

#include "Cing.h"

namespace Ui
{
	class Dialog;
}

class QPixmap;
class QLabel;

class QTDialogSample : public QDialog
{
	Q_OBJECT


public:
	QTDialogSample(QWidget* parent);
	~QTDialogSample();

	// Overridden to QDialog to detect when window is closed and then restore its position
	void reject();

	// Restores the window position and shows it
	void restore(bool setGeometry = true);

private:

	// Slots are methods that can be connected to QT UI events so that for example a method is called when a button is pushed.
public slots:
	
	// This will be udpated on a timer
	void Update();

	// This will be called when a button is pushed to change bg color to random color
	void setRandomBackgroundColor();
	
	// This will be called when the white level slider changes, to change the background color.
	void setWhiteLevel(int newValue);


private:
	Ui::Dialog*		m_ui;
	QTimer						m_timer;

};

#endif // QTCONTROLPANEL_H
