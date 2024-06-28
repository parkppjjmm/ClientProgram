/* Code Explain
This code is a C++ implementation of a simple TCP client using the Winsock API. 
The client connects to a server, exchanges data using predefined packet structures, and updates shared memory with the received data.
*/

#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<windows.h>

#pragma comment (lib, "Ws2_32.lib")
#define PORT 8000
#define PACKET_SIZE 1000
#define SERVER_IP "xxx.xxx.xxx.xxx" // 서버 아이피
#define CAMERA_BUFFER_Value2 100
#define TIMER_ID 1

using namespace std;

typedef struct _EXAMPLE_SEND_PACKET
{
	int32_t Counter;
	char Value1[16];
	int32_t Value2;
	int32_t Value3;
	int32_t Value4;
}_EXAMPLE_SEND_PACKET;

typedef struct _EXAMPLE_RECV_PACKET 
{
	int32_t Counter;
	bool Value5;
	bool Value6;
	bool Value7;
	bool Value8;
}_EXAMPLE_RECV_PACKET;

struct _EXAMPLE_SEND_PACKET clientRecv;
struct _EXAMPLE_RECV_PACKET ServerRequest;

const std::string currentDateTime() {
	time_t     now = time(0); //현재 시간을 time_t 타입으로 저장
	struct tm  tstruct;
	char       Buffer[80];
	tstruct = *localtime(&now);
	strftime(Buffer, sizeof(Buffer), "%Y-%m-%d_%H:%M:%S", &tstruct); // YYYY-MM-DD.HH:mm:ss 형태의 스트링

	return Buffer;
}

void TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	////    0. 공유메모리 선언 및 local variable 선언	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	HANDLE HanValue1;
	HanValue1 = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "IPCValue1");
	if (HanValue1 == NULL) { std::cout << currentDateTime() << " -- " << "Could not read HanValue1 : " << GetLastError() << endl;	exit(0); }

	HANDLE HanValue2;
	HanValue2 = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "IPCValue2");
	if (HanValue2 == NULL) { std::cout << currentDateTime() << " -- " << "Could not read HanValue2 : " << GetLastError() << endl;	exit(0); }

	HANDLE HanValue3;
	HanValue3 = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "IPCValue3");
	if (HanValue3 == NULL) { std::cout << currentDateTime() << " -- " << "Could not read HanCoilValue3 : " << GetLastError() << endl;	exit(0); }
	
	HANDLE HanValue4;
	HanValue4 = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, "IPCValue4");
	if (HanValue4 == NULL) { std::cout << currentDateTime() << " -- " << "Could not read HanCoilValue4 : " << GetLastError() << endl;	exit(0); }

	char* chkValue1 = (char*)MapViewOfFile(HanValue1, FILE_MAP_WRITE, 0, 0, 0);
	int32_t* chkValue2 = (int32_t*)MapViewOfFile(HanValue2, FILE_MAP_WRITE, 0, 0, 0);
	int32_t* chkValue3 = (int32_t*)MapViewOfFile(HanValue3, FILE_MAP_WRITE, 0, 0, 0);
	int32_t* chkValue4 = (int32_t*)MapViewOfFile(HanValue4, FILE_MAP_WRITE, 0, 0, 0);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	////    1. TCP/IP Socket 통신 선언	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(PORT);
	tAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	ServerRequest.Counter = ServerRequest.Counter + 1;
	ServerRequest.Value5 = TRUE;
	ServerRequest.Value6 = TRUE;

	connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));
	send(hSocket, (char*)&ServerRequest, sizeof(struct _EXAMPLE_RECV_PACKET), 0);
	recv(hSocket, (char*)&clientRecv, PACKET_SIZE, 0);

	printf("----------------------------------------------------------------\n");
	printf("--------------------- Server to Client -------------------------\n");
	printf("----------------------------------------------------------------\n");
	printf("Client_RecvData Counter : %d\n", clientRecv.Counter);
	printf("Client_RecvData Value1 : %s\n", clientRecv.Value1);
	printf("Client_RecvData Value2 : %d\n", clientRecv.Value2);
	printf("Client_RecvData Value3 : %d\n", clientRecv.Value3);
	printf("Client_RecvData Value4 : %d\n", clientRecv.Value4);

	/////////////////// 공유 메모리에 저장 ///////////////////
	memcpy(chkValue1, clientRecv.Value1, sizeof(char) * 16);
	memcpy(chkValue3, &clientRecv.Value3, sizeof(int32_t));
	memcpy(chkValue2, &clientRecv.Value2, sizeof(int32_t));
	memcpy(chkValue4, &clientRecv.Value4, sizeof(int32_t));

	printf("----------------------------------------------------------------\n");
	printf("---------------------- Client to Shared Mem --------------------\n");
	printf("----------------------------------------------------------------\n");
	printf("chkValue1 : %s\n", chkValue1);
	printf("chkValue2 : %d\n", *chkValue2);
	printf("chkValue3 : %d\n", *chkValue3);
	printf("chkValue4 : %d\n", *chkValue4);

}

int main(int argc, char* argv[], char* envp[])
{
	ServerRequest.Counter = 0;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(PORT);
	tAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	SetTimer(NULL, TIMER_ID, 1000, (TIMERPROC)TimerProc); //SetTimer 지정 : 초당 1번

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	closesocket;
}
