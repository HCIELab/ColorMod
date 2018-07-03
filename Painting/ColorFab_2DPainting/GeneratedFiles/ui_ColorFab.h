/********************************************************************************
** Form generated from reading UI file 'ColorFab.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORFAB_H
#define UI_COLORFAB_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ColorFabClass
{
public:
    QAction *actionQuit;
    QAction *actionLicense;
    QAction *actionLoadPaintingUI;
    QWidget *centralWidget;
    QTabWidget *tabWidgetSetting;
    QWidget *tabSetting;
    QGroupBox *groupCameraSetup;
    QLabel *labelCameraModel;
    QComboBox *comboBoxCameraModel;
    QCheckBox *checkBoxCameraFlipMode;
    QGroupBox *groupViewSetting;
    QLabel *labelViewSizeW;
    QLabel *labelViewSizeH;
    QLineEdit *lineEditViewW;
    QLineEdit *lineEditViewH;
    QGroupBox *groupScreenSetting;
    QLabel *labelScreenSizeW;
    QLabel *labelScreenSizeH;
    QLineEdit *lineEditScreenW;
    QLineEdit *lineEditScreenH;
    QComboBox *comboBoxScreenNumber;
    QLabel *labelScreenNumber;
    QWidget *tabCalibration;
    QGroupBox *groupCamToProCalibration;
    QPushButton *buttonCalibCamToPro_exec;
    QLineEdit *pCalibCam_x1;
    QLineEdit *pCalibCam_y1;
    QLineEdit *pCalibPro_y1;
    QLineEdit *pCalibPro_x1;
    QLineEdit *pCalibPro_y2;
    QLineEdit *pCalibCam_y2;
    QLineEdit *pCalibPro_x2;
    QLineEdit *pCalibCam_x2;
    QLineEdit *pCalibPro_y3;
    QLineEdit *pCalibCam_y3;
    QLineEdit *pCalibPro_x3;
    QLineEdit *pCalibCam_x3;
    QLineEdit *pCalibPro_y4;
    QLineEdit *pCalibCam_y4;
    QLineEdit *pCalibPro_x4;
    QLineEdit *pCalibCam_x4;
    QLabel *textMarkCalib1;
    QLabel *textMarkCalib2;
    QLabel *textMarkCalib3;
    QLabel *textMarkCalib4;
    QPushButton *buttonCalibCamToPro_input;
    QPushButton *buttonQuit;
    QPushButton *buttonConnect;
    QPushButton *buttonProjection;
    QTabWidget *tabWidgetProjection;
    QWidget *tabManualCapture;
    QPushButton *buttonManualCapture;
    QWidget *tabManualProjection;
    QPushButton *buttonSelectProjectionFile;
    QLabel *labelProjectionFileText;
    QLabel *labelProjectionFileParameter;
    QGroupBox *groupUVChamberController;
    QLineEdit *inPortName;
    QLabel *labelPortname;
    QGroupBox *groupFanController;
    QPushButton *buttonFanOn;
    QPushButton *buttonFanOff;
    QGroupBox *groupTableController;
    QPushButton *buttonTableOff;
    QPushButton *buttonTableAutoOn;
    QPushButton *buttonTableManualOn;
    QLabel *labelPortStatus;
    QLabel *outPortStatus;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuPainting;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ColorFabClass)
    {
        if (ColorFabClass->objectName().isEmpty())
            ColorFabClass->setObjectName(QStringLiteral("ColorFabClass"));
        ColorFabClass->resize(411, 429);
        actionQuit = new QAction(ColorFabClass);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionLicense = new QAction(ColorFabClass);
        actionLicense->setObjectName(QStringLiteral("actionLicense"));
        actionLoadPaintingUI = new QAction(ColorFabClass);
        actionLoadPaintingUI->setObjectName(QStringLiteral("actionLoadPaintingUI"));
        centralWidget = new QWidget(ColorFabClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidgetSetting = new QTabWidget(centralWidget);
        tabWidgetSetting->setObjectName(QStringLiteral("tabWidgetSetting"));
        tabWidgetSetting->setGeometry(QRect(10, 10, 391, 161));
        tabSetting = new QWidget();
        tabSetting->setObjectName(QStringLiteral("tabSetting"));
        groupCameraSetup = new QGroupBox(tabSetting);
        groupCameraSetup->setObjectName(QStringLiteral("groupCameraSetup"));
        groupCameraSetup->setGeometry(QRect(10, 10, 371, 51));
        labelCameraModel = new QLabel(groupCameraSetup);
        labelCameraModel->setObjectName(QStringLiteral("labelCameraModel"));
        labelCameraModel->setGeometry(QRect(10, 20, 41, 16));
        comboBoxCameraModel = new QComboBox(groupCameraSetup);
        comboBoxCameraModel->setObjectName(QStringLiteral("comboBoxCameraModel"));
        comboBoxCameraModel->setGeometry(QRect(50, 20, 191, 21));
        checkBoxCameraFlipMode = new QCheckBox(groupCameraSetup);
        checkBoxCameraFlipMode->setObjectName(QStringLiteral("checkBoxCameraFlipMode"));
        checkBoxCameraFlipMode->setEnabled(true);
        checkBoxCameraFlipMode->setGeometry(QRect(250, 20, 70, 17));
        checkBoxCameraFlipMode->setChecked(false);
        groupViewSetting = new QGroupBox(tabSetting);
        groupViewSetting->setObjectName(QStringLiteral("groupViewSetting"));
        groupViewSetting->setGeometry(QRect(230, 70, 151, 41));
        groupViewSetting->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelViewSizeW = new QLabel(groupViewSetting);
        labelViewSizeW->setObjectName(QStringLiteral("labelViewSizeW"));
        labelViewSizeW->setGeometry(QRect(10, 20, 16, 16));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        labelViewSizeW->setFont(font);
        labelViewSizeH = new QLabel(groupViewSetting);
        labelViewSizeH->setObjectName(QStringLiteral("labelViewSizeH"));
        labelViewSizeH->setGeometry(QRect(80, 20, 16, 16));
        labelViewSizeH->setFont(font);
        lineEditViewW = new QLineEdit(groupViewSetting);
        lineEditViewW->setObjectName(QStringLiteral("lineEditViewW"));
        lineEditViewW->setGeometry(QRect(20, 20, 51, 16));
        QFont font1;
        font1.setBold(true);
        font1.setWeight(75);
        lineEditViewW->setFont(font1);
        lineEditViewW->setAlignment(Qt::AlignCenter);
        lineEditViewW->setReadOnly(true);
        lineEditViewH = new QLineEdit(groupViewSetting);
        lineEditViewH->setObjectName(QStringLiteral("lineEditViewH"));
        lineEditViewH->setGeometry(QRect(90, 20, 51, 16));
        lineEditViewH->setFont(font1);
        lineEditViewH->setAlignment(Qt::AlignCenter);
        lineEditViewH->setReadOnly(true);
        groupScreenSetting = new QGroupBox(tabSetting);
        groupScreenSetting->setObjectName(QStringLiteral("groupScreenSetting"));
        groupScreenSetting->setGeometry(QRect(10, 70, 211, 41));
        groupScreenSetting->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        labelScreenSizeW = new QLabel(groupScreenSetting);
        labelScreenSizeW->setObjectName(QStringLiteral("labelScreenSizeW"));
        labelScreenSizeW->setGeometry(QRect(10, 20, 16, 16));
        labelScreenSizeW->setFont(font);
        labelScreenSizeH = new QLabel(groupScreenSetting);
        labelScreenSizeH->setObjectName(QStringLiteral("labelScreenSizeH"));
        labelScreenSizeH->setGeometry(QRect(70, 20, 16, 16));
        labelScreenSizeH->setFont(font);
        lineEditScreenW = new QLineEdit(groupScreenSetting);
        lineEditScreenW->setObjectName(QStringLiteral("lineEditScreenW"));
        lineEditScreenW->setGeometry(QRect(20, 20, 41, 16));
        lineEditScreenW->setFont(font1);
        lineEditScreenW->setAlignment(Qt::AlignCenter);
        lineEditScreenW->setReadOnly(false);
        lineEditScreenH = new QLineEdit(groupScreenSetting);
        lineEditScreenH->setObjectName(QStringLiteral("lineEditScreenH"));
        lineEditScreenH->setGeometry(QRect(80, 20, 41, 16));
        lineEditScreenH->setFont(font1);
        lineEditScreenH->setAlignment(Qt::AlignCenter);
        lineEditScreenH->setReadOnly(false);
        comboBoxScreenNumber = new QComboBox(groupScreenSetting);
        comboBoxScreenNumber->setObjectName(QStringLiteral("comboBoxScreenNumber"));
        comboBoxScreenNumber->setGeometry(QRect(160, 15, 41, 21));
        QFont font2;
        font2.setBold(true);
        font2.setItalic(false);
        font2.setWeight(75);
        comboBoxScreenNumber->setFont(font2);
        labelScreenNumber = new QLabel(groupScreenSetting);
        labelScreenNumber->setObjectName(QStringLiteral("labelScreenNumber"));
        labelScreenNumber->setGeometry(QRect(130, 20, 31, 16));
        QFont font3;
        font3.setBold(false);
        font3.setItalic(false);
        font3.setWeight(50);
        labelScreenNumber->setFont(font3);
        tabWidgetSetting->addTab(tabSetting, QString());
        tabCalibration = new QWidget();
        tabCalibration->setObjectName(QStringLiteral("tabCalibration"));
        groupCamToProCalibration = new QGroupBox(tabCalibration);
        groupCamToProCalibration->setObjectName(QStringLiteral("groupCamToProCalibration"));
        groupCamToProCalibration->setGeometry(QRect(10, 10, 361, 131));
        buttonCalibCamToPro_exec = new QPushButton(groupCamToProCalibration);
        buttonCalibCamToPro_exec->setObjectName(QStringLiteral("buttonCalibCamToPro_exec"));
        buttonCalibCamToPro_exec->setEnabled(false);
        buttonCalibCamToPro_exec->setGeometry(QRect(120, 100, 61, 21));
        buttonCalibCamToPro_exec->setFont(font1);
        pCalibCam_x1 = new QLineEdit(groupCamToProCalibration);
        pCalibCam_x1->setObjectName(QStringLiteral("pCalibCam_x1"));
        pCalibCam_x1->setGeometry(QRect(10, 20, 31, 20));
        QFont font4;
        font4.setPointSize(8);
        pCalibCam_x1->setFont(font4);
        pCalibCam_x1->setAlignment(Qt::AlignCenter);
        pCalibCam_x1->setReadOnly(true);
        pCalibCam_y1 = new QLineEdit(groupCamToProCalibration);
        pCalibCam_y1->setObjectName(QStringLiteral("pCalibCam_y1"));
        pCalibCam_y1->setGeometry(QRect(40, 20, 31, 20));
        pCalibCam_y1->setFont(font4);
        pCalibCam_y1->setAlignment(Qt::AlignCenter);
        pCalibCam_y1->setReadOnly(true);
        pCalibPro_y1 = new QLineEdit(groupCamToProCalibration);
        pCalibPro_y1->setObjectName(QStringLiteral("pCalibPro_y1"));
        pCalibPro_y1->setGeometry(QRect(150, 20, 31, 20));
        pCalibPro_y1->setFont(font4);
        pCalibPro_y1->setAlignment(Qt::AlignCenter);
        pCalibPro_y1->setReadOnly(true);
        pCalibPro_x1 = new QLineEdit(groupCamToProCalibration);
        pCalibPro_x1->setObjectName(QStringLiteral("pCalibPro_x1"));
        pCalibPro_x1->setGeometry(QRect(120, 20, 31, 20));
        pCalibPro_x1->setFont(font4);
        pCalibPro_x1->setAlignment(Qt::AlignCenter);
        pCalibPro_x1->setReadOnly(true);
        pCalibPro_y2 = new QLineEdit(groupCamToProCalibration);
        pCalibPro_y2->setObjectName(QStringLiteral("pCalibPro_y2"));
        pCalibPro_y2->setGeometry(QRect(150, 40, 31, 20));
        pCalibPro_y2->setFont(font4);
        pCalibPro_y2->setAlignment(Qt::AlignCenter);
        pCalibPro_y2->setReadOnly(true);
        pCalibCam_y2 = new QLineEdit(groupCamToProCalibration);
        pCalibCam_y2->setObjectName(QStringLiteral("pCalibCam_y2"));
        pCalibCam_y2->setGeometry(QRect(40, 40, 31, 20));
        pCalibCam_y2->setFont(font4);
        pCalibCam_y2->setAlignment(Qt::AlignCenter);
        pCalibCam_y2->setReadOnly(true);
        pCalibPro_x2 = new QLineEdit(groupCamToProCalibration);
        pCalibPro_x2->setObjectName(QStringLiteral("pCalibPro_x2"));
        pCalibPro_x2->setGeometry(QRect(120, 40, 31, 20));
        pCalibPro_x2->setFont(font4);
        pCalibPro_x2->setAlignment(Qt::AlignCenter);
        pCalibPro_x2->setReadOnly(true);
        pCalibCam_x2 = new QLineEdit(groupCamToProCalibration);
        pCalibCam_x2->setObjectName(QStringLiteral("pCalibCam_x2"));
        pCalibCam_x2->setGeometry(QRect(10, 40, 31, 20));
        pCalibCam_x2->setFont(font4);
        pCalibCam_x2->setAlignment(Qt::AlignCenter);
        pCalibCam_x2->setReadOnly(true);
        pCalibPro_y3 = new QLineEdit(groupCamToProCalibration);
        pCalibPro_y3->setObjectName(QStringLiteral("pCalibPro_y3"));
        pCalibPro_y3->setGeometry(QRect(150, 60, 31, 20));
        pCalibPro_y3->setFont(font4);
        pCalibPro_y3->setAlignment(Qt::AlignCenter);
        pCalibCam_y3 = new QLineEdit(groupCamToProCalibration);
        pCalibCam_y3->setObjectName(QStringLiteral("pCalibCam_y3"));
        pCalibCam_y3->setGeometry(QRect(40, 60, 31, 20));
        pCalibCam_y3->setFont(font4);
        pCalibCam_y3->setAlignment(Qt::AlignCenter);
        pCalibCam_y3->setReadOnly(true);
        pCalibPro_x3 = new QLineEdit(groupCamToProCalibration);
        pCalibPro_x3->setObjectName(QStringLiteral("pCalibPro_x3"));
        pCalibPro_x3->setGeometry(QRect(120, 60, 31, 20));
        pCalibPro_x3->setFont(font4);
        pCalibPro_x3->setAlignment(Qt::AlignCenter);
        pCalibCam_x3 = new QLineEdit(groupCamToProCalibration);
        pCalibCam_x3->setObjectName(QStringLiteral("pCalibCam_x3"));
        pCalibCam_x3->setGeometry(QRect(10, 60, 31, 20));
        pCalibCam_x3->setFont(font4);
        pCalibCam_x3->setAlignment(Qt::AlignCenter);
        pCalibCam_x3->setReadOnly(true);
        pCalibPro_y4 = new QLineEdit(groupCamToProCalibration);
        pCalibPro_y4->setObjectName(QStringLiteral("pCalibPro_y4"));
        pCalibPro_y4->setGeometry(QRect(150, 80, 31, 20));
        pCalibPro_y4->setFont(font4);
        pCalibPro_y4->setAlignment(Qt::AlignCenter);
        pCalibPro_y4->setReadOnly(true);
        pCalibCam_y4 = new QLineEdit(groupCamToProCalibration);
        pCalibCam_y4->setObjectName(QStringLiteral("pCalibCam_y4"));
        pCalibCam_y4->setGeometry(QRect(40, 80, 31, 20));
        pCalibCam_y4->setFont(font4);
        pCalibCam_y4->setAlignment(Qt::AlignCenter);
        pCalibCam_y4->setReadOnly(true);
        pCalibPro_x4 = new QLineEdit(groupCamToProCalibration);
        pCalibPro_x4->setObjectName(QStringLiteral("pCalibPro_x4"));
        pCalibPro_x4->setGeometry(QRect(120, 80, 31, 20));
        pCalibPro_x4->setFont(font4);
        pCalibPro_x4->setAlignment(Qt::AlignCenter);
        pCalibPro_x4->setReadOnly(true);
        pCalibCam_x4 = new QLineEdit(groupCamToProCalibration);
        pCalibCam_x4->setObjectName(QStringLiteral("pCalibCam_x4"));
        pCalibCam_x4->setGeometry(QRect(10, 80, 31, 20));
        pCalibCam_x4->setFont(font4);
        pCalibCam_x4->setAlignment(Qt::AlignCenter);
        pCalibCam_x4->setReadOnly(true);
        textMarkCalib1 = new QLabel(groupCamToProCalibration);
        textMarkCalib1->setObjectName(QStringLiteral("textMarkCalib1"));
        textMarkCalib1->setGeometry(QRect(70, 20, 51, 16));
        textMarkCalib1->setAlignment(Qt::AlignCenter);
        textMarkCalib2 = new QLabel(groupCamToProCalibration);
        textMarkCalib2->setObjectName(QStringLiteral("textMarkCalib2"));
        textMarkCalib2->setGeometry(QRect(70, 40, 51, 16));
        textMarkCalib2->setAlignment(Qt::AlignCenter);
        textMarkCalib3 = new QLabel(groupCamToProCalibration);
        textMarkCalib3->setObjectName(QStringLiteral("textMarkCalib3"));
        textMarkCalib3->setGeometry(QRect(70, 60, 51, 16));
        textMarkCalib3->setAlignment(Qt::AlignCenter);
        textMarkCalib4 = new QLabel(groupCamToProCalibration);
        textMarkCalib4->setObjectName(QStringLiteral("textMarkCalib4"));
        textMarkCalib4->setGeometry(QRect(70, 80, 51, 16));
        textMarkCalib4->setAlignment(Qt::AlignCenter);
        buttonCalibCamToPro_input = new QPushButton(groupCamToProCalibration);
        buttonCalibCamToPro_input->setObjectName(QStringLiteral("buttonCalibCamToPro_input"));
        buttonCalibCamToPro_input->setEnabled(false);
        buttonCalibCamToPro_input->setGeometry(QRect(10, 100, 61, 21));
        tabWidgetSetting->addTab(tabCalibration, QString());
        buttonQuit = new QPushButton(centralWidget);
        buttonQuit->setObjectName(QStringLiteral("buttonQuit"));
        buttonQuit->setGeometry(QRect(320, 340, 81, 31));
        buttonConnect = new QPushButton(centralWidget);
        buttonConnect->setObjectName(QStringLiteral("buttonConnect"));
        buttonConnect->setGeometry(QRect(10, 340, 91, 31));
        buttonProjection = new QPushButton(centralWidget);
        buttonProjection->setObjectName(QStringLiteral("buttonProjection"));
        buttonProjection->setEnabled(false);
        buttonProjection->setGeometry(QRect(110, 340, 91, 31));
        tabWidgetProjection = new QTabWidget(centralWidget);
        tabWidgetProjection->setObjectName(QStringLiteral("tabWidgetProjection"));
        tabWidgetProjection->setGeometry(QRect(10, 180, 391, 81));
        tabManualCapture = new QWidget();
        tabManualCapture->setObjectName(QStringLiteral("tabManualCapture"));
        buttonManualCapture = new QPushButton(tabManualCapture);
        buttonManualCapture->setObjectName(QStringLiteral("buttonManualCapture"));
        buttonManualCapture->setGeometry(QRect(4, 5, 371, 41));
        tabWidgetProjection->addTab(tabManualCapture, QString());
        tabManualProjection = new QWidget();
        tabManualProjection->setObjectName(QStringLiteral("tabManualProjection"));
        buttonSelectProjectionFile = new QPushButton(tabManualProjection);
        buttonSelectProjectionFile->setObjectName(QStringLiteral("buttonSelectProjectionFile"));
        buttonSelectProjectionFile->setGeometry(QRect(320, 10, 61, 41));
        labelProjectionFileText = new QLabel(tabManualProjection);
        labelProjectionFileText->setObjectName(QStringLiteral("labelProjectionFileText"));
        labelProjectionFileText->setGeometry(QRect(10, 10, 91, 16));
        labelProjectionFileParameter = new QLabel(tabManualProjection);
        labelProjectionFileParameter->setObjectName(QStringLiteral("labelProjectionFileParameter"));
        labelProjectionFileParameter->setGeometry(QRect(10, 30, 301, 16));
        labelProjectionFileParameter->setFont(font1);
        tabWidgetProjection->addTab(tabManualProjection, QString());
        groupUVChamberController = new QGroupBox(centralWidget);
        groupUVChamberController->setObjectName(QStringLiteral("groupUVChamberController"));
        groupUVChamberController->setGeometry(QRect(10, 270, 386, 66));
        inPortName = new QLineEdit(groupUVChamberController);
        inPortName->setObjectName(QStringLiteral("inPortName"));
        inPortName->setGeometry(QRect(35, 20, 71, 16));
        inPortName->setFont(font1);
        inPortName->setAlignment(Qt::AlignCenter);
        labelPortname = new QLabel(groupUVChamberController);
        labelPortname->setObjectName(QStringLiteral("labelPortname"));
        labelPortname->setGeometry(QRect(10, 20, 31, 16));
        groupFanController = new QGroupBox(groupUVChamberController);
        groupFanController->setObjectName(QStringLiteral("groupFanController"));
        groupFanController->setGeometry(QRect(140, 15, 96, 46));
        buttonFanOn = new QPushButton(groupFanController);
        buttonFanOn->setObjectName(QStringLiteral("buttonFanOn"));
        buttonFanOn->setGeometry(QRect(5, 15, 41, 23));
        buttonFanOff = new QPushButton(groupFanController);
        buttonFanOff->setObjectName(QStringLiteral("buttonFanOff"));
        buttonFanOff->setGeometry(QRect(50, 15, 41, 23));
        groupTableController = new QGroupBox(groupUVChamberController);
        groupTableController->setObjectName(QStringLiteral("groupTableController"));
        groupTableController->setGeometry(QRect(240, 15, 141, 46));
        buttonTableOff = new QPushButton(groupTableController);
        buttonTableOff->setObjectName(QStringLiteral("buttonTableOff"));
        buttonTableOff->setGeometry(QRect(95, 15, 41, 23));
        buttonTableAutoOn = new QPushButton(groupTableController);
        buttonTableAutoOn->setObjectName(QStringLiteral("buttonTableAutoOn"));
        buttonTableAutoOn->setGeometry(QRect(5, 15, 41, 23));
        buttonTableManualOn = new QPushButton(groupTableController);
        buttonTableManualOn->setObjectName(QStringLiteral("buttonTableManualOn"));
        buttonTableManualOn->setGeometry(QRect(50, 15, 41, 23));
        labelPortStatus = new QLabel(groupUVChamberController);
        labelPortStatus->setObjectName(QStringLiteral("labelPortStatus"));
        labelPortStatus->setGeometry(QRect(10, 40, 41, 16));
        outPortStatus = new QLabel(groupUVChamberController);
        outPortStatus->setObjectName(QStringLiteral("outPortStatus"));
        outPortStatus->setGeometry(QRect(45, 40, 131, 16));
        QFont font5;
        font5.setFamily(QStringLiteral("MS Gothic"));
        font5.setPointSize(8);
        font5.setBold(false);
        font5.setWeight(50);
        outPortStatus->setFont(font5);
        ColorFabClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ColorFabClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 411, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuPainting = new QMenu(menuBar);
        menuPainting->setObjectName(QStringLiteral("menuPainting"));
        ColorFabClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ColorFabClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ColorFabClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ColorFabClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ColorFabClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuPainting->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionLicense);
        menuPainting->addAction(actionLoadPaintingUI);

        retranslateUi(ColorFabClass);

        tabWidgetSetting->setCurrentIndex(0);
        comboBoxScreenNumber->setCurrentIndex(-1);
        tabWidgetProjection->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ColorFabClass);
    } // setupUi

    void retranslateUi(QMainWindow *ColorFabClass)
    {
        ColorFabClass->setWindowTitle(QApplication::translate("ColorFabClass", "ColorFab", Q_NULLPTR));
        actionQuit->setText(QApplication::translate("ColorFabClass", "Quit", Q_NULLPTR));
        actionLicense->setText(QApplication::translate("ColorFabClass", "License", Q_NULLPTR));
        actionLoadPaintingUI->setText(QApplication::translate("ColorFabClass", "Load Painting UI", Q_NULLPTR));
        groupCameraSetup->setTitle(QApplication::translate("ColorFabClass", "Camera Setup", Q_NULLPTR));
        labelCameraModel->setText(QApplication::translate("ColorFabClass", "Model:", Q_NULLPTR));
        checkBoxCameraFlipMode->setText(QApplication::translate("ColorFabClass", "Flip image", Q_NULLPTR));
        groupViewSetting->setTitle(QApplication::translate("ColorFabClass", "View Setting", Q_NULLPTR));
        labelViewSizeW->setText(QApplication::translate("ColorFabClass", "W", Q_NULLPTR));
        labelViewSizeH->setText(QApplication::translate("ColorFabClass", "H", Q_NULLPTR));
        lineEditViewW->setText(QApplication::translate("ColorFabClass", "640", Q_NULLPTR));
        lineEditViewH->setText(QApplication::translate("ColorFabClass", "480", Q_NULLPTR));
        groupScreenSetting->setTitle(QApplication::translate("ColorFabClass", "Screen Setting", Q_NULLPTR));
        labelScreenSizeW->setText(QApplication::translate("ColorFabClass", "W", Q_NULLPTR));
        labelScreenSizeH->setText(QApplication::translate("ColorFabClass", "H", Q_NULLPTR));
        lineEditScreenW->setText(QApplication::translate("ColorFabClass", "1024", Q_NULLPTR));
        lineEditScreenH->setText(QApplication::translate("ColorFabClass", "768", Q_NULLPTR));
        comboBoxScreenNumber->setCurrentText(QString());
        labelScreenNumber->setText(QApplication::translate("ColorFabClass", "DISP.", Q_NULLPTR));
        tabWidgetSetting->setTabText(tabWidgetSetting->indexOf(tabSetting), QApplication::translate("ColorFabClass", "Setting", Q_NULLPTR));
        groupCamToProCalibration->setTitle(QApplication::translate("ColorFabClass", "ROI (camera <-> projector)", Q_NULLPTR));
        buttonCalibCamToPro_exec->setText(QApplication::translate("ColorFabClass", "Calibrate", Q_NULLPTR));
        pCalibCam_x1->setText(QString());
        pCalibCam_y1->setText(QString());
        pCalibPro_y1->setText(QString());
        pCalibPro_x1->setText(QString());
        pCalibPro_y2->setText(QString());
        pCalibCam_y2->setText(QString());
        pCalibPro_x2->setText(QString());
        pCalibCam_x2->setText(QString());
        pCalibPro_y3->setText(QString());
        pCalibCam_y3->setText(QString());
        pCalibPro_x3->setText(QString());
        pCalibCam_x3->setText(QString());
        pCalibPro_y4->setText(QString());
        pCalibCam_y4->setText(QString());
        pCalibPro_x4->setText(QString());
        pCalibCam_x4->setText(QString());
        textMarkCalib1->setText(QApplication::translate("ColorFabClass", "------>", Q_NULLPTR));
        textMarkCalib2->setText(QApplication::translate("ColorFabClass", "------>", Q_NULLPTR));
        textMarkCalib3->setText(QApplication::translate("ColorFabClass", "------>", Q_NULLPTR));
        textMarkCalib4->setText(QApplication::translate("ColorFabClass", "------>", Q_NULLPTR));
        buttonCalibCamToPro_input->setText(QApplication::translate("ColorFabClass", "Manual", Q_NULLPTR));
        tabWidgetSetting->setTabText(tabWidgetSetting->indexOf(tabCalibration), QApplication::translate("ColorFabClass", "Calibration", Q_NULLPTR));
        buttonQuit->setText(QApplication::translate("ColorFabClass", "Quit", Q_NULLPTR));
        buttonConnect->setText(QApplication::translate("ColorFabClass", "Connect", Q_NULLPTR));
        buttonProjection->setText(QApplication::translate("ColorFabClass", "Projection", Q_NULLPTR));
        buttonManualCapture->setText(QApplication::translate("ColorFabClass", "Capture", Q_NULLPTR));
        tabWidgetProjection->setTabText(tabWidgetProjection->indexOf(tabManualCapture), QApplication::translate("ColorFabClass", "Capture-Manual", Q_NULLPTR));
        buttonSelectProjectionFile->setText(QApplication::translate("ColorFabClass", "Select", Q_NULLPTR));
        labelProjectionFileText->setText(QApplication::translate("ColorFabClass", "Projection file", Q_NULLPTR));
        labelProjectionFileParameter->setText(QApplication::translate("ColorFabClass", "(Not Select)", Q_NULLPTR));
        tabWidgetProjection->setTabText(tabWidgetProjection->indexOf(tabManualProjection), QApplication::translate("ColorFabClass", "Projection-Manual", Q_NULLPTR));
        groupUVChamberController->setTitle(QApplication::translate("ColorFabClass", "UV Chamber (Arduino Controller)", Q_NULLPTR));
        inPortName->setText(QApplication::translate("ColorFabClass", "COM3", Q_NULLPTR));
        labelPortname->setText(QApplication::translate("ColorFabClass", "Port", Q_NULLPTR));
        groupFanController->setTitle(QApplication::translate("ColorFabClass", "Fan", Q_NULLPTR));
        buttonFanOn->setText(QApplication::translate("ColorFabClass", "ON", Q_NULLPTR));
        buttonFanOff->setText(QApplication::translate("ColorFabClass", "OFF", Q_NULLPTR));
        groupTableController->setTitle(QApplication::translate("ColorFabClass", "Rotation Table", Q_NULLPTR));
        buttonTableOff->setText(QApplication::translate("ColorFabClass", "OFF", Q_NULLPTR));
        buttonTableAutoOn->setText(QApplication::translate("ColorFabClass", "AON", Q_NULLPTR));
        buttonTableManualOn->setText(QApplication::translate("ColorFabClass", "MON", Q_NULLPTR));
        labelPortStatus->setText(QApplication::translate("ColorFabClass", "Status", Q_NULLPTR));
        outPortStatus->setText(QApplication::translate("ColorFabClass", "Not Connected", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("ColorFabClass", "File", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("ColorFabClass", "Help", Q_NULLPTR));
        menuPainting->setTitle(QApplication::translate("ColorFabClass", "Painting", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ColorFabClass: public Ui_ColorFabClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORFAB_H
