/********************************************************************************
** Form generated from reading UI file 'PaintingUI.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAINTINGUI_H
#define UI_PAINTINGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PaintingUI
{
public:
    QAction *actionmm;
    QAction *actionmj;
    QAction *actionNextView;
    QAction *actionprevious;
    QAction *actionPreviousView;
    QAction *actionExtractColorVoxels;
    QAction *actionCreateImage4Projection;
    QAction *actionCreateImg4Projection;
    QAction *actionOpen;
    QAction *actionSaveAs;
    QAction *actionPenColor;
    QAction *actionPenWidth;
    QAction *actionQuit;
    QAction *actionClearScreen;
    QWidget *centralWidget;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuAbout;
    QMenu *menuFile;
    QMenu *menuPaintingOption;

    void setupUi(QMainWindow *PaintingUI)
    {
        if (PaintingUI->objectName().isEmpty())
            PaintingUI->setObjectName(QStringLiteral("PaintingUI"));
        PaintingUI->resize(660, 505);
        actionmm = new QAction(PaintingUI);
        actionmm->setObjectName(QStringLiteral("actionmm"));
        actionmj = new QAction(PaintingUI);
        actionmj->setObjectName(QStringLiteral("actionmj"));
        actionNextView = new QAction(PaintingUI);
        actionNextView->setObjectName(QStringLiteral("actionNextView"));
        actionprevious = new QAction(PaintingUI);
        actionprevious->setObjectName(QStringLiteral("actionprevious"));
        actionPreviousView = new QAction(PaintingUI);
        actionPreviousView->setObjectName(QStringLiteral("actionPreviousView"));
        actionExtractColorVoxels = new QAction(PaintingUI);
        actionExtractColorVoxels->setObjectName(QStringLiteral("actionExtractColorVoxels"));
        actionCreateImage4Projection = new QAction(PaintingUI);
        actionCreateImage4Projection->setObjectName(QStringLiteral("actionCreateImage4Projection"));
        actionCreateImg4Projection = new QAction(PaintingUI);
        actionCreateImg4Projection->setObjectName(QStringLiteral("actionCreateImg4Projection"));
        actionOpen = new QAction(PaintingUI);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSaveAs = new QAction(PaintingUI);
        actionSaveAs->setObjectName(QStringLiteral("actionSaveAs"));
        actionPenColor = new QAction(PaintingUI);
        actionPenColor->setObjectName(QStringLiteral("actionPenColor"));
        actionPenWidth = new QAction(PaintingUI);
        actionPenWidth->setObjectName(QStringLiteral("actionPenWidth"));
        actionQuit = new QAction(PaintingUI);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionClearScreen = new QAction(PaintingUI);
        actionClearScreen->setObjectName(QStringLiteral("actionClearScreen"));
        centralWidget = new QWidget(PaintingUI);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        PaintingUI->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(PaintingUI);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        PaintingUI->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(PaintingUI);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        PaintingUI->setStatusBar(statusBar);
        menuBar = new QMenuBar(PaintingUI);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 660, 21));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuPaintingOption = new QMenu(menuBar);
        menuPaintingOption->setObjectName(QStringLiteral("menuPaintingOption"));
        PaintingUI->setMenuBar(menuBar);

        mainToolBar->addAction(actionPreviousView);
        mainToolBar->addAction(actionNextView);
        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuBar->addAction(menuPaintingOption->menuAction());
        menuAbout->addAction(actionExtractColorVoxels);
        menuAbout->addAction(actionCreateImg4Projection);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSaveAs);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);
        menuPaintingOption->addAction(actionPenColor);
        menuPaintingOption->addAction(actionPenWidth);
        menuPaintingOption->addSeparator();
        menuPaintingOption->addAction(actionClearScreen);

        retranslateUi(PaintingUI);

        QMetaObject::connectSlotsByName(PaintingUI);
    } // setupUi

    void retranslateUi(QMainWindow *PaintingUI)
    {
        PaintingUI->setWindowTitle(QApplication::translate("PaintingUI", "PaintingUI", Q_NULLPTR));
        actionmm->setText(QApplication::translate("PaintingUI", "mm", Q_NULLPTR));
        actionmj->setText(QApplication::translate("PaintingUI", "mj", Q_NULLPTR));
        actionNextView->setText(QApplication::translate("PaintingUI", "Next View", Q_NULLPTR));
        actionprevious->setText(QApplication::translate("PaintingUI", "previous", Q_NULLPTR));
        actionPreviousView->setText(QApplication::translate("PaintingUI", "Previous View", Q_NULLPTR));
        actionExtractColorVoxels->setText(QApplication::translate("PaintingUI", "Extract Color Voxels", Q_NULLPTR));
        actionCreateImage4Projection->setText(QApplication::translate("PaintingUI", "Create Image for Projection", Q_NULLPTR));
        actionCreateImg4Projection->setText(QApplication::translate("PaintingUI", "Create Image for Projection", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("PaintingUI", "Open", Q_NULLPTR));
        actionSaveAs->setText(QApplication::translate("PaintingUI", "Save As", Q_NULLPTR));
        actionPenColor->setText(QApplication::translate("PaintingUI", "Pen Color", Q_NULLPTR));
        actionPenWidth->setText(QApplication::translate("PaintingUI", "Pen Width", Q_NULLPTR));
        actionQuit->setText(QApplication::translate("PaintingUI", "Quit", Q_NULLPTR));
        actionClearScreen->setText(QApplication::translate("PaintingUI", "Clear Screen", Q_NULLPTR));
        menuAbout->setTitle(QApplication::translate("PaintingUI", "Image Processing", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("PaintingUI", "File", Q_NULLPTR));
        menuPaintingOption->setTitle(QApplication::translate("PaintingUI", "Painting Option", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PaintingUI: public Ui_PaintingUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAINTINGUI_H
