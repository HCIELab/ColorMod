/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <iostream>
#include "PaintingArea.h"
#include "ColorFabPainting.h"
#include "ImageAnalysis.h"
#include <string>
#include "config.h"

ColorFabPainting::ColorFabPainting(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::ColorFabPaintingClass)
{
	ui->setupUi(this);
	paintingArea = new PaintingArea();
	imageAnalysis = new ImageAnalysis(); 
	setCentralWidget(paintingArea);

	createActions();
	createMenus();
	int image = NUM_OF_IMAGE;
	setWindowTitle(tr("Painting"));
	resize(500, 500);
	ui->statusBar->showMessage("ColorFab Painting Interface");
}

void ColorFabPainting::closeEvent(QCloseEvent *event)
{
	if (maybeSave()) {
		imageAnalysis->~ImageAnalysis(); 
		event->accept();
	}
	else {
		imageAnalysis->~ImageAnalysis();
		event->ignore();
	}
}

void ColorFabPainting::open(){
	string one = "1.jpg";
	string two = "2.jpg"; 
	string three = "3.jpg"; 
	string four = "4.jpg"; 

	string fileName;
	if (imageIndex == 0)
		fileName = "1.jpg";
	else if (imageIndex == 1)
		fileName = "2.jpg";
	else if (imageIndex == 2)
		fileName = "3.jpg";
	else if (imageIndex == 3)
		fileName = "4.jpg";

	paintingArea->openImage(QString::fromStdString(fileName));
	ui->statusBar->showMessage(QString::fromStdString(fileName));

	/*	fileName = QFileDialog::getOpenFileName(this,
		tr("Open File"), QDir::currentPath());
	if (!fileName.isEmpty()) {
		int i = fileName.lastIndexOf("/");
		std::string name = fileName.mid(i + 1, 1).toLocal8Bit().constData();
		imageIndex = stoi(name.c_str());
		paintingArea->openImage(fileName);
	}*/
		
}

void ColorFabPainting::save()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QByteArray fileFormat = action->data().toByteArray();
	saveFile(fileFormat);

}

void ColorFabPainting::next() {
	maybeSave(); 
	imageIndex = (imageIndex + 1) % 4; 
	open(); 
}

void ColorFabPainting::previous() {
	maybeSave(); 
	imageIndex = (4 + imageIndex - 1) % 4; 
	open(); 
}

void ColorFabPainting::extractColors() 
{
	string fileName; 
	if (imageIndex == 0)
		fileName = "1.jpg";
	else if (imageIndex == 1)
		fileName = "2.jpg";
	else if (imageIndex == 2)
		fileName = "3.jpg";
	else if (imageIndex == 3)
		fileName = "4.jpg";

	Mat img = imread(fileName, CV_LOAD_IMAGE_COLOR); 
	Mat blur; 
	for (int i = 1; i < 9; i = i + 2) {
		GaussianBlur(img, blur, Size(i, i), 0, 0);
	}
	Mat greyscaleimage;
	cv::cvtColor(blur, greyscaleimage, CV_BGR2GRAY);
	colorPos = imageAnalysis->extractColors(greyscaleimage);
	std::string newFileName = "view_" + fileName;
	rowNum = greyscaleimage.rows; 
	colNum = greyscaleimage.cols; 
	
	imwrite(newFileName, greyscaleimage);
	paintingArea->openImage(QString::fromStdString(newFileName));
	ui->statusBar->showMessage(QString::fromStdString(newFileName));
}

void ColorFabPainting::createImg4Projection() 
{
	//maybeSave();
	string fileName;
	if (imageIndex == 0)
		fileName = "view_1.jpg";
	else if (imageIndex == 1)
		fileName = "view_2.jpg";
	else if (imageIndex == 2)
		fileName = "view_3.jpg";
	else if (imageIndex == 3)
		fileName = "view_4.jpg";
	paintingArea->saveImage(QString::fromStdString(fileName), "jpg"); 
	//paintingArea->openImage(QString::fromStdString(fileName));
	
	Mat userPainting = imread(fileName, CV_LOAD_IMAGE_COLOR); 
	map<string, vector<pair<int, int>>> match = imageAnalysis->matchUserPainting(userPainting, colorPos); 
	Mat projImg = imageAnalysis->createImg4Proj(match, rowNum, colNum); 
	std::string newFileName = "projection_" + fileName; 
	imwrite(newFileName, projImg); 
	imshow("Projection Image", projImg); 

	ui->statusBar->showMessage(QString::fromStdString(fileName));
}

void ColorFabPainting::penColor()
{
	QColor newColor = QColorDialog::getColor(paintingArea->penColor());
	if (newColor.isValid())
		paintingArea->setPenColor(newColor);
}

void ColorFabPainting::penWidth()
{
	bool ok;
	int newWidth = QInputDialog::getInt(this, tr("Painting"),
		tr("Select pen width:"),
		paintingArea->penWidth(),
		1, 50, 1, &ok);
	if (ok)
		paintingArea->setPenWidth(newWidth);
}

void ColorFabPainting::clearScreen()
{
	maybeSave(); 
	string fileName;
	if (imageIndex == 0)
		fileName = "view_1.jpg";
	else if (imageIndex == 1)
		fileName = "view_2.jpg";
	else if (imageIndex == 2)
		fileName = "view_3.jpg";
	else if (imageIndex == 3)
		fileName = "view_4.jpg";
	paintingArea->openImage(QString::fromStdString(fileName));
}

//void ColorFabPainting::eraserSize()
//{
//	bool ok; 
//	int newSize = QInputDialog::getInt(this, tr("Painting"),
//		tr("Select eraser size:"),
//		paintingArea->penWidth(),
//		1, 50, 1, &ok);
//	if (ok)
//		paintingArea->setEraserSize(newSize); 
//}

void ColorFabPainting::createActions()
{
	openAct = new QAction(tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	foreach(QByteArray format, QImageWriter::supportedImageFormats()) {
		QString text = tr("%1...").arg(QString(format).toUpper());

		QAction *action = new QAction(text, this);
		action->setData(format);
		connect(action, SIGNAL(triggered()), this, SLOT(save()));
		saveAsActs.append(action);
	}

	connect(ui->actionNextView, SIGNAL(triggered()), this, SLOT(next()));
	connect(ui->actionPreviousView, SIGNAL(triggered()), this, SLOT(previous()));

	connect(ui->actionExtractColorVoxels, SIGNAL(triggered()), this, SLOT(extractColors()));

	connect(ui->actionCreateImg4Projection, SIGNAL(triggered()), this, SLOT(createImg4Projection()));

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	penColorAct = new QAction(tr("&Pen Color..."), this);
	connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));
	
	penWidthAct = new QAction(tr("Pen &Width..."), this);
	connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

	//eraserSizeAct = new QAction(tr("E&raser Size..."), this); 
	//connect(eraserSizeAct, SIGNAL(triggered()), this, SLOT(eraserSize())); 

	clearScreenAct = new QAction(tr("&Clear Screen"), this);
	clearScreenAct->setShortcut(tr("Ctrl+L"));
	connect(clearScreenAct, SIGNAL(triggered()),
		this, SLOT(clearScreen()));
}

void ColorFabPainting::createMenus()
{
	saveAsMenu = new QMenu(tr("&Save As"), this);
	foreach(QAction *action, saveAsActs)
		saveAsMenu->addAction(action);

	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addMenu(saveAsMenu);
	//fileMenu->addAction(ui->actionNextView);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);


	optionMenu = new QMenu(tr("Painting &Options"), this);
	optionMenu->addAction(penColorAct);
	optionMenu->addAction(penWidthAct);
	//optionMenu->addAction(eraserSizeAct); 
	optionMenu->addSeparator();
	optionMenu->addAction(clearScreenAct);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(optionMenu);
}

bool ColorFabPainting::maybeSave()
{
	if (paintingArea->isModified()) {
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("Painting"),
			tr("The image has been modified.\n"
				"Do you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard
			| QMessageBox::Cancel);
		if (ret == QMessageBox::Save) {
			return saveFile("png");
		}
		else if (ret == QMessageBox::Cancel) {
			return false;
		}
	}
	return true;
}

bool ColorFabPainting::saveFile(const QByteArray &fileFormat)
{
	QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
		initialPath,
		tr("%1 Files (*.%2);;All Files (*)")
		.arg(QString::fromLatin1(fileFormat.toUpper()))
		.arg(QString::fromLatin1(fileFormat)));
	if (fileName.isEmpty()) {
		return false;
	}
	else {
		return paintingArea->saveImage(fileName, fileFormat.constData());
	}
}