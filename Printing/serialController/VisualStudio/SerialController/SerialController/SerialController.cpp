//#include "SerialController.h"
//
//SerialController::SerialController(const char *portName) {
//	// Not yet connected 
//	this->connected = false; 
//
//	// Try to connect to the give port through CreateFile 
//	this->hSerial = CreateFile(portName,
//		GENERIC_READ | GENERIC_WRITE,
//		0,
//		NULL,
//		OPEN_EXISTING,
//		FILE_ATTRIBUTE_NORMAL,
//		NULL); 
//
//	// Check if the connection was successful 
//	if (this->hSerial == INVALID_HANDLE_VALUE) {
//		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
//			printf("ERROR: Handle was not attached. Reason: %s not available. \n", portName); 
//		}
//		else {
//			printf("ERROR!"); 
//		}
//	}
//	else {
//		//If connected we try to set the comm parameters
//		DCB dcbSerialParams = { 0 };
//
//		//Try to get the current
//		if (!GetCommState(this->hSerial, &dcbSerialParams))
//		{
//			//If impossible, show an error
//			printf("failed to get current serial parameters!");
//		}
//		else
//		{
//			//Define serial connection parameters for the arduino board
//			dcbSerialParams.BaudRate = CBR_9600;
//			dcbSerialParams.ByteSize = 8;
//			dcbSerialParams.StopBits = ONESTOPBIT;
//			dcbSerialParams.Parity = NOPARITY;
//			//Setting the DTR to Control_Enable ensures that the Arduino is properly
//			//reset upon establishing a connection
//			dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;
//
//			//Set the parameters and check for their proper application
//			if (!SetCommState(hSerial, &dcbSerialParams))
//			{
//				printf("ALERT: Could not set Serial Port parameters");
//			}
//			else
//			{
//				//If everything went fine we're connected
//				this->connected = true;
//				//Flush any remaining characters in the buffers 
//				PurgeComm(this->hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
//				//We wait 2s as the arduino board will be reseting
//				Sleep(ARDUINO_WAIT_TIME);
//			}
//		}
//	}
//}
//
//SerialController::~SerialController()
//{
//	//Check if we are connected before trying to disconnect
//	if (this->connected)
//	{
//		//We're no longer connected
//		this->connected = false;
//		//Close the serial handler
//		CloseHandle(this->hSerial);
//	}
//}
//
//bool SerialController::WriteData(const char *buffer, unsigned int nbChar)
//{
//	DWORD bytesSend;
//
//	//Try to write the buffer on the Serial port
//	if (!WriteFile(this->hSerial, (void *)buffer, nbChar, &bytesSend, 0))
//	{
//		//In case it don't work get comm error and return false
//		ClearCommError(this->hSerial, &this->errors, &this->status);
//
//		return false;
//	}
//	else
//		return true;
//}
//
//bool SerialController::IsConnected()
//{
//	//Simply return the connection status
//	return this->connected;
//}
