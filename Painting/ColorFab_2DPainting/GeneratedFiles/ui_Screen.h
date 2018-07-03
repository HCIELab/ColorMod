/********************************************************************************
** Form generated from reading UI file 'Screen.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCREEN_H
#define UI_SCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Screen
{
public:
    QLabel *Display;

    void setupUi(QWidget *Screen)
    {
        if (Screen->objectName().isEmpty())
            Screen->setObjectName(QStringLiteral("Screen"));
        Screen->resize(1042, 768);
        Display = new QLabel(Screen);
        Display->setObjectName(QStringLiteral("Display"));
        Display->setGeometry(QRect(0, 0, 1024, 768));

        retranslateUi(Screen);

        QMetaObject::connectSlotsByName(Screen);
    } // setupUi

    void retranslateUi(QWidget *Screen)
    {
        Screen->setWindowTitle(QApplication::translate("Screen", "Projection", Q_NULLPTR));
        Display->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Screen: public Ui_Screen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCREEN_H
