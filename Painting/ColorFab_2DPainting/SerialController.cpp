#include "SerialController.h"
#include "Config.h"

#include <QtSerialPort/QSerialPort>
#include <QTime>
#include <iostream>

SerialController::SerialController(QObject *parent):
	QThread(parent), waitTimeout(0), quit(false)
{
}

SerialController::~SerialController()
{
	mutex.lock();
	quit = true;
	cond.wakeOne();
	mutex.unlock();
	wait();
}

void SerialController::transaction(const QString &portName, int waitTimeout, const QString &request)
{
	QMutexLocker locker(&mutex);
	this->portName = portName;
	this->waitTimeout = waitTimeout;
	this->request = request;
	if (!isRunning())
		start();
	else
		cond.wakeOne();
}

void SerialController::stop()
{
	mutex.lock();
	quit = true;
	mutex.unlock();
	cond.wakeOne();
	wait();
}

void SerialController::run()
{
	bool currentPortNameChanged = false;

	mutex.lock();
	QString currentPortName;
	if (currentPortName != portName) 
	{
		currentPortName = portName;
		currentPortNameChanged = true;
	}

	int currentWaitTimeout = waitTimeout;
	QString currentRequest = request;
	mutex.unlock();
	QSerialPort serial;

	while (!quit) 
	{
		if (currentPortNameChanged) 
		{
			serial.close();
			serial.setPortName(currentPortName);

			if (!serial.open(QIODevice::ReadWrite)) 
			{
				emit Error(tr("Can't open %1, error code %2").arg(portName).arg(serial.error()));
				return;
			}
		}
		// write request
		QByteArray requestData = currentRequest.toLocal8Bit();
		serial.write(requestData);
		if (serial.waitForBytesWritten(waitTimeout)) 
		{
			// read response
			if (serial.waitForReadyRead(currentWaitTimeout)) 
			{
				QByteArray responseData = serial.readAll();
				while (serial.waitForReadyRead(10))
					responseData += serial.readAll();

				QString response(responseData);
				emit this->Response(response);
			}
			else 
			{
				//emit Timeout(tr("Wait read response timeout %1").arg(QTime::currentTime().toString()));
			}
		}
		else 
		{
			//emit Timeout(tr("Wait write request timeout %1").arg(QTime::currentTime().toString()));
		}
		mutex.lock();
		cond.wait(&mutex);
		if (currentPortName != portName) 
		{
			currentPortName = portName;
			currentPortNameChanged = true;
		}
		else 
		{
			currentPortNameChanged = false;
		}
		currentWaitTimeout = waitTimeout;
		currentRequest = request;
		mutex.unlock();
	}
}

void SerialController::Request(const QString &request)
{
	QMutexLocker locker(&mutex);
	this->request = request;
	if (!isRunning())
		start();
	else
		cond.wakeOne();

	return;
}