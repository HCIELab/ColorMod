/********************************************************************************
** Form generated from reading UI file 'ColorFabPainting.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORFABPAINTING_H
#define UI_COLORFABPAINTING_H

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

class Ui_ColorFabPaintingClass
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
    QWidget *centralWidget;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuAbout;

    void setupUi(QMainWindow *ColorFabPaintingClass)
    {
        if (ColorFabPaintingClass->objectName().isEmpty())
            ColorFabPaintingClass->setObjectName(QStringLiteral("ColorFabPaintingClass"));
        ColorFabPaintingClass->resize(660, 505);
        actionmm = new QAction(ColorFabPaintingClass);
        actionmm->setObjectName(QStringLiteral("actionmm"));
        actionmj = new QAction(ColorFabPaintingClass);
        actionmj->setObjectName(QStringLiteral("actionmj"));
        actionNextView = new QAction(ColorFabPaintingClass);
        actionNextView->setObjectName(QStringLiteral("actionNextView"));
        actionprevious = new QAction(ColorFabPaintingClass);
        actionprevious->setObjectName(QStringLiteral("actionprevious"));
        actionPreviousView = new QAction(ColorFabPaintingClass);
        actionPreviousView->setObjectName(QStringLiteral("actionPreviousView"));
        actionExtractColorVoxels = new QAction(ColorFabPaintingClass);
        actionExtractColorVoxels->setObjectName(QStringLiteral("actionExtractColorVoxels"));
        actionCreateImage4Projection = new QAction(ColorFabPaintingClass);
        actionCreateImage4Projection->setObjectName(QStringLiteral("actionCreateImage4Projection"));
        actionCreateImg4Projection = new QAction(ColorFabPaintingClass);
        actionCreateImg4Projection->setObjectName(QStringLiteral("actionCreateImg4Projection"));
        centralWidget = new QWidget(ColorFabPaintingClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        ColorFabPaintingClass->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(ColorFabPaintingClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ColorFabPaintingClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ColorFabPaintingClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ColorFabPaintingClass->setStatusBar(statusBar);
        menuBar = new QMenuBar(ColorFabPaintingClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 660, 21));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QStringLiteral("menuAbout"));
        ColorFabPaintingClass->setMenuBar(menuBar);

        mainToolBar->addAction(actionPreviousView);
        mainToolBar->addAction(actionNextView);
        menuBar->addAction(menuAbout->menuAction());
        menuAbout->addAction(actionExtractColorVoxels);
        menuAbout->addAction(actionCreateImg4Projection);

        retranslateUi(ColorFabPaintingClass);

        QMetaObject::connectSlotsByName(ColorFabPaintingClass);
    } // setupUi

    void retranslateUi(QMainWindow *ColorFabPaintingClass)
    {
        ColorFabPaintingClass->setWindowTitle(QApplication::translate("ColorFabPaintingClass", "ColorFabPainting", Q_NULLPTR));
        actionmm->setText(QApplication::translate("ColorFabPaintingClass", "mm", Q_NULLPTR));
        actionmj->setText(QApplication::translate("ColorFabPaintingClass", "mj", Q_NULLPTR));
        actionNextView->setText(QApplication::translate("ColorFabPaintingClass", "Next View", Q_NULLPTR));
        actionprevious->setText(QApplication::translate("ColorFabPaintingClass", "previous", Q_NULLPTR));
        actionPreviousView->setText(QApplication::translate("ColorFabPaintingClass", "Previous View", Q_NULLPTR));
        actionExtractColorVoxels->setText(QApplication::translate("ColorFabPaintingClass", "Extract Color Voxels", Q_NULLPTR));
        actionCreateImage4Projection->setText(QApplication::translate("ColorFabPaintingClass", "Create Image for Projection", Q_NULLPTR));
        actionCreateImg4Projection->setText(QApplication::translate("ColorFabPaintingClass", "Create Image for Projection", Q_NULLPTR));
        menuAbout->setTitle(QApplication::translate("ColorFabPaintingClass", "Image Processing", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ColorFabPaintingClass: public Ui_ColorFabPaintingClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORFABPAINTING_H
