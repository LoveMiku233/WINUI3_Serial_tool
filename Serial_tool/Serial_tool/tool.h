#ifndef _MIKU_TOOL_
#define _MIKU_TOOL_

using namespace std;
struct TempHum
{
	float temp = 0;
	float hum = 0;
};

bool  getTempHum(char* cstr, TempHum* dht);

class  Serial_Miku {
private:
	HANDLE hCom;
	string last_err;
public:
	bool openSpy(string name, unsigned char baud_rate, unsigned char parity, unsigned char byte_size, unsigned char stop_bits);
	bool openSpy(string name);

	DWORD writeData(char* buffer, int length);
	DWORD readData(char* buffer, int length);
	void closeCom();
	bool isOpened();
	bool flushBuff();
};

#endif