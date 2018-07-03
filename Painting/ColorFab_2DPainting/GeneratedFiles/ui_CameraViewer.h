/********************************************************************************
** Form generated from reading UI file 'CameraViewer.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CAMERAVIEWER_H
#define UI_CAMERAVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CameraViewer
{
public:
    QLabel *Display;

    void setupUi(QWidget *CameraViewer)
    {
        if (CameraViewer->objectName().isEmpty())
            CameraViewer->setObjectName(QStringLiteral("CameraViewer"));
        CameraViewer->resize(640, 480);
        QIcon icon;
        icon.addFile(QStringLiteral("Resources/camera.ico"), QSize(), QIcon::Normal, QIcon::Off);
        CameraViewer->setWindowIcon(icon);
        Display = new QLabel(CameraViewer);
        Display->setObjectName(QStringLiteral("Display"));
        Display->setGeometry(QRect(0, 0, 640, 480));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Display->sizePolicy().hasHeightForWidth());
        Display->setSizePolicy(sizePolicy);
        Display->setMinimumSize(QSize(0, 0));
        Display->setMaximumSize(QSize(640, 480));
        Display->setFrameShape(QFrame::NoFrame);
        Display->setAlignment(Qt::AlignCenter);

        retranslateUi(CameraViewer);

        QMetaObject::connectSlotsByName(CameraViewer);
    } // setupUi

    void retranslateUi(QWidget *CameraViewer)
    {
        CameraViewer->setWindowTitle(QApplication::translate("CameraViewer", "CameraViewer", Q_NULLPTR));
        Display->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CameraViewer: public Ui_CameraViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CAMERAVIEWER_H
