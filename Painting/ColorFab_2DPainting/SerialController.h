#pragma once

// Original source code from 
// - http://playground.arduino.cc/Interfacing/CPPWindows
// - http://webtutsdepot.com/2010/05/01/arduino-visual-c-plus-plus-tutorial/

#ifndef SERIALCLASS_H_INCLUDED
#define SERIALCLASS_H_INCLUDED

#include <QtCore/QThread>
#include <QMutex>
#include <QWaitCondition>

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class SerialController : public QThread
{
	Q_OBJECT

	private:
		QString portName;
		QString request;
		int waitTimeout;
		QMutex mutex;
		QWaitCondition cond;
		bool quit;

	public:
		explicit SerialController(QObject *parent = nullptr);
		~SerialController();

		void transaction(const QString &portName, int waitTimeout, const QString &request);
		void run() Q_DECL_OVERRIDE;
		void stop();

	signals:
		void Response(const QString &s);
		void Error(const QString &s);
		//void Timeout(const QString &s);

	public slots:
		void Request(const QString &request);
};

#endif // SERIALCLASS_H_INCLUDED