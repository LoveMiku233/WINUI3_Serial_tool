#include "pch.h"
using namespace std;

//转换字符串为dht11字符串
bool getTempHum(char* cstr, TempHum* dht) {
	//{Temp:26.0,Hum:58.0}
	string str = cstr;
	if (-1 == str.find("{") || -1 == str.find("}")) {
		return false;
	}else if(str.find("{")>str.find("}")) {
		return false;
	}
	int cnt = str.find("Temp");
	dht->temp = (float)atof(str.substr(cnt + 5, str.find(",") - cnt - 5).c_str());
	cnt = str.find("Hum");
	dht->hum = (float)atof(str.substr(cnt + 4, str.find('}') - cnt - 4).c_str());
	return true;
}


/*************串口*********************/
bool Serial_Miku::openSpy(string name, unsigned char baud_rate, unsigned char parity, unsigned char byte_size, unsigned char stop_bits) {
	if (!openSpy(name)) {
		return false;
	}
	DCB dcb;
	if (false == GetCommState(hCom, &dcb)) {
		last_err = "GetCommState Read ERR!";
		return false;
	}
	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = baud_rate;
	dcb.Parity = parity;
	dcb.ByteSize = byte_size;
	dcb.StopBits = stop_bits;
	if (false == SetCommState(hCom, &dcb)) {
		last_err = "SetCommState Writer ERR!";
		return false;
	}

	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = MAXDWORD; 

	timeouts.ReadTotalTimeoutMultiplier = 0; 
	timeouts.ReadTotalTimeoutConstant = 0; 
	timeouts.WriteTotalTimeoutMultiplier = 50; 
	timeouts.WriteTotalTimeoutConstant = 2000; 

	if (false == SetCommTimeouts(hCom, &timeouts)) {
		last_err = "SetCommTimeouts Writer ERR!";
		return false;
	}
	if (false == PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_RXABORT))
	{
		last_err = "Buffer ERR!";
		return false;
	}
	return true;
}

bool Serial_Miku::openSpy(string name) {
	hCom = CreateFileA(
		name.data(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);
	if (INVALID_HANDLE_VALUE == hCom) {
		hCom = NULL;
		last_err = "Open " + name + " failed!";
		return false;
	}
	return true;
}

void Serial_Miku::closeCom() {
	if (NULL == hCom)
		return;
	CloseHandle(hCom);
	hCom = NULL;
}

bool Serial_Miku::isOpened() {
	return NULL == hCom ? false : true;
}

DWORD Serial_Miku::readData(char* buffer, int length)
{
	DWORD writeSize = 0;

	bool ret = false;

	ret = ReadFile(hCom, buffer, length, &writeSize, NULL);

	if (false == ret)
		return 0;

	return writeSize;
}


DWORD Serial_Miku::writeData(char* buffer, int length)
{
	DWORD writeSize = 0;

	bool ret = false;

	ret = WriteFile(hCom, buffer, length, &writeSize, NULL);

	if (false == ret)
		return 0;

	return writeSize;

}

bool Serial_Miku::flushBuff()
{
	if (FlushFileBuffers(hCom))
	{
		last_err = "flush buffer failed!";
		return false;
	}
	return true;
}