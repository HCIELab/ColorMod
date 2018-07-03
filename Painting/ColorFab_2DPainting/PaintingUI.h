#pragma once
#ifndef PAINTINGUI_H
#define PAINTINGUI_H

#include <QtWidgets/QMainWindow>
#include <QList>
#include <QMainWindow>
#include "ui_PaintingUI.h"

#include <iostream>
#include <string>
#include "PaintingImageAnalysis.h"

class PaintingArea;
class PaintingUI : public QMainWindow
{
	Q_OBJECT

	public:
		PaintingUI(QWidget *parent = Q_NULLPTR);

	protected:
		void closeEvent(QCloseEvent *event) override;

		private slots:
		void open();
		void save();
		void next();
		void previous();
		void extractColors();
		void createImg4Projection();
		void penColor();
		void penWidth();
		void clearScreen();

	private:
		Ui::PaintingUI *ui;
		void createActions();
		bool maybeSave();
		bool saveFile(const QByteArray &fileFormat);

		PaintingArea *paintingArea;

		PaintingImageAnalysis *paintingImageAnalysis;
		map<string, Mat> colorPos;
		int rowNum;
		int colNum;

		int imageIndex = 0;
};

#endif