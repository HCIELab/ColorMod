//#pragma once
//#ifndef SERIALCONTROLLERCLASS_H_INCLUDED
//#define SERIALCONTROLLERCLASS_H_INCLUDED
//
//#define ARDUINO_WAIT_TIME 2000
//
//#include <Windows.h>
//#include <stdio.h>
//#include <stdlib.h>
//
//class SerialController {
//	private: 
//		//Serial comm handler
//		HANDLE hSerial;
//		//Connection status
//		bool connected;
//		//Get various information about the connection
//		COMSTAT status;
//		//Keep track of last error
//		DWORD errors;
//
//public: 
//	//Init serial communication with the given COM port 
//	SerialController(const char *portName); 
//	//Close the connection 
//	~SerialController(); 
//	//Writes data from a buffer through the Serial connection
//	//return true on success.
//	bool WriteData(const char *buffer, unsigned int nbChar);
//	//Check if we are actually connected
//	bool IsConnected();
//};
//#endif // SERIALCONTROLLERCLASS_H_INCLUDED
