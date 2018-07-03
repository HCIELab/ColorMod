#include "Screen.h"
#include "Config.h"

#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtGui/QMouseEvent>

Screen::Screen(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Screen)
{
	// Setup UI
	ui->setupUi(this);

	// Set viewer style to black color (using QStyle)
	ui->Display->setStyleSheet("QLabel { background-color : black; }");
	// Set Window properties
	QIcon icon;
	icon.addFile(QStringLiteral("Resources/screen.ico"), QSize(), QIcon::Normal, QIcon::Off);
	setWindowIcon(icon);
}

Screen::~Screen()
{
	delete ui;
}

void Screen::UpdateRendering(const QImage &Frame)
{
	// Display image to a screen
	ui->Display->setPixmap(QPixmap::fromImage(Frame));

	return;
}

void Screen::mousePressEvent(QMouseEvent *event)
{
	// Set Mouseevent for check the position of screen cursor, only when its reqeusted
	if (event->button() == Qt::LeftButton && this->isRequestCamProCalibrate)
	{
		if (this->mousePressCount < 4)
		{
			this->mousePressCount += 1;
			emit MousePressPosition(event->pos().x(), event->pos().y(), mousePressCount);
		}
		else
			this->isRequestCamProCalibrate = false;
	}

	return;
}

void Screen::UpdateIsRequestCamProCalibrate(const bool &IsRequestCamProCalibrate)
{
	// Reset mouseevent pressed count
	this->mousePressCount = 0;
	// Update status of camera-projector calibration request
	this->isRequestCamProCalibrate = IsRequestCamProCalibrate;

	return;
}