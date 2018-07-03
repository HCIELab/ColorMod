#include "PaintingArea.h"
#include "PaintingUI.h"

#include "Config.h"

PaintingUI::PaintingUI(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::PaintingUI)
{
	ui->setupUi(this);
	paintingArea = new PaintingArea();
	paintingImageAnalysis = new PaintingImageAnalysis();
	setCentralWidget(paintingArea);

	createActions();
	int image = N_IMAGE_CAPTURED_FOR_PAINT;
	ui->statusBar->showMessage(tr("ColorFab PaintingUI"));

	// -- Setup window geometry
	this->setGeometry((M_APP_QT_POS_X * 2) + parent->size().width(),
					  M_APP_QT_POS_Y,
					  M_APP_CAM_WIN_SIZE_W,
					  M_APP_CAM_WIN_SIZE_H);
	this->resize(640, 480);
}

void PaintingUI::closeEvent(QCloseEvent *event)
{
	if (maybeSave()) 
	{
		paintingImageAnalysis->~PaintingImageAnalysis();
		event->accept();
	}
	else {
		paintingImageAnalysis->~PaintingImageAnalysis();
		event->ignore();
	}
}

void PaintingUI::open() 
{
	string one = "Data/Painting/1.jpg";
	string two = "Data/Painting/2.jpg";
	string three = "Data/Painting/3.jpg";
	string four = "Data/Painting/4.jpg";

	string fileName;
	if (imageIndex == 0) fileName = "Data/Painting/1.jpg";
	else if (imageIndex == 1) fileName = "Data/Painting/2.jpg";
	else if (imageIndex == 2) fileName = "Data/Painting/3.jpg";
	else if (imageIndex == 3) fileName = "Data/Painting/4.jpg";

	paintingArea->openImage(QString::fromStdString(fileName));
	ui->statusBar->showMessage(QString::fromStdString(fileName));
}

void PaintingUI::save()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QByteArray fileFormat = action->data().toByteArray();
	saveFile(fileFormat);

}

void PaintingUI::next() 
{
	maybeSave();
	imageIndex = (imageIndex + 1) % 4;
	open();
}

void PaintingUI::previous() 
{
	maybeSave();
	imageIndex = (4 + imageIndex - 1) % 4;
	open();
}

void PaintingUI::extractColors()
{
	string fileName;
	if (imageIndex == 0) fileName = "1.jpg";
	else if (imageIndex == 1) fileName = "2.jpg";
	else if (imageIndex == 2) fileName = "3.jpg";
	else if (imageIndex == 3) fileName = "4.jpg";

	string temp_fileName = "Data/Painting/" + fileName;
	Mat img = imread(temp_fileName, CV_LOAD_IMAGE_COLOR);
	Mat blur;
	for (int i = 1; i < 9; i = i + 2) {
		GaussianBlur(img, blur, Size(i, i), 0, 0);
	}
	Mat greyscaleimage;
	cv::cvtColor(blur, greyscaleimage, CV_BGR2GRAY);
	colorPos = paintingImageAnalysis->extractColors(greyscaleimage);
	std::string newFileName = "Data/Painting/view_" + fileName;
	rowNum = greyscaleimage.rows;
	colNum = greyscaleimage.cols;

	imwrite(newFileName, greyscaleimage);
	paintingArea->openImage(QString::fromStdString(newFileName));
	ui->statusBar->showMessage(QString::fromStdString(newFileName));
}

void PaintingUI::createImg4Projection()
{
	//maybeSave();
	string fileName;
	if (imageIndex == 0) fileName = "view_1.jpg";
	else if (imageIndex == 1) fileName = "view_2.jpg";
	else if (imageIndex == 2) fileName = "view_3.jpg";
	else if (imageIndex == 3) fileName = "view_4.jpg";
	string temp_fileName = "Data/Painting/" + fileName;
	paintingArea->saveImage(QString::fromStdString(temp_fileName), "jpg");

	Mat userPainting = imread(temp_fileName, CV_LOAD_IMAGE_COLOR);
	map<string, vector<pair<int, int>>> match = paintingImageAnalysis->matchUserPainting(userPainting, colorPos);
	Mat projImg = paintingImageAnalysis->createImg4Proj(match, rowNum, colNum);
	std::string newFileName = "Data/Painting/projection_" + fileName;
	imwrite(newFileName, projImg);
	imshow("Projection Image", projImg);

	ui->statusBar->showMessage(QString::fromStdString(fileName));
}

void PaintingUI::penColor()
{
	QColor newColor = QColorDialog::getColor(paintingArea->penColor());
	if (newColor.isValid()) paintingArea->setPenColor(newColor);
}

void PaintingUI::penWidth()
{
	bool ok;
	int newWidth = QInputDialog::getInt(this, 
										tr("Painting"), 
										tr("Select pen width:"), 
										paintingArea->penWidth(),
										1, 
										50, 
										1, 
										&ok);
	if (ok)
		paintingArea->setPenWidth(newWidth);
}

void PaintingUI::clearScreen()
{
	maybeSave();

	string fileName;
	if (imageIndex == 0) fileName = "Data/Painting/view_1.jpg";
	else if (imageIndex == 1) fileName = "Data/Painting/view_2.jpg";
	else if (imageIndex == 2) fileName = "Data/Painting/view_3.jpg";
	else if (imageIndex == 3) fileName = "Data/Painting/view_4.jpg";
	paintingArea->openImage(QString::fromStdString(fileName));
}

void PaintingUI::createActions()
{
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(open()));
	foreach(QByteArray format, QImageWriter::supportedImageFormats()) 
	{
		QString text = tr("%1...").arg(QString(format).toUpper());

		QAction *action = new QAction(text, this);
		action->setData(format);
		connect(action, SIGNAL(triggered()), this, SLOT(save()));
	}

	connect(ui->actionNextView, SIGNAL(triggered()), this, SLOT(next()));
	connect(ui->actionPreviousView, SIGNAL(triggered()), this, SLOT(previous()));
	connect(ui->actionExtractColorVoxels, SIGNAL(triggered()), this, SLOT(extractColors()));
	connect(ui->actionCreateImg4Projection, SIGNAL(triggered()), this, SLOT(createImg4Projection()));
	connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui->actionPenColor, SIGNAL(triggered()), this, SLOT(penColor()));
	connect(ui->actionPenWidth, SIGNAL(triggered()), this, SLOT(penWidth()));
	connect(ui->actionClearScreen, SIGNAL(triggered()), this, SLOT(clearScreen()));
}

bool PaintingUI::maybeSave()
{
	if (paintingArea->isModified()) 
	{
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, 
								   tr("Painting"),
								   tr("The image has been modified.\n"
								   "Do you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save) 
		{
			return saveFile("png");
		}
		else if (ret == QMessageBox::Cancel) 
		{
			return false;
		}
	}

	return true;
}

bool PaintingUI::saveFile(const QByteArray &fileFormat)
{
	QString initialPath = "Data/Painting/untitled." + fileFormat;

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
		initialPath,
		tr("%1 Files (*.%2);;All Files (*)")
		.arg(QString::fromLatin1(fileFormat.toUpper()))
		.arg(QString::fromLatin1(fileFormat)));
	if (fileName.isEmpty())
	{
		return false;
	}
	else 
	{
		return paintingArea->saveImage(fileName, fileFormat.constData());
	}
}