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
#define SERVER_IP "192.168.200.100" // 서버 아이피
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

typedef struct _EXAMPLE_RECV_PACKET {
	int32_t Counter;
	bool Value5;
	bool Value6;
	bool Value7;
	bool Value8;
}_EXAMPLE_RECV_PACKET;

struct _EXAMPLE_SEND_PACKET clientRecv;
struct _EXAMPLE_RECV_PACKET ServerRequest;

void TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
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
	ServerRequest.Value7 = TRUE;
	ServerRequest.Value8 = TRUE;

	connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));
	send(hSocket, (char*)&ServerRequest, sizeof(struct _EXAMPLE_RECV_PACKET), 0);
	recv(hSocket, (char*)&clientRecv, PACKET_SIZE, 0);

	printf("----------------------------------------------------------------\n");
	printf("-------------------------- Client Part -------------------------\n");
	printf("----------------------------------------------------------------\n");
	printf("Client_RecvData Counter : %d\n", clientRecv.Counter);
	printf("Client_RecvData Value1 : %s\n", clientRecv.Value1);
	printf("Client_RecvData Value2 : %d\n", clientRecv.Value2);
	printf("Client_RecvData Value3 : %d\n", clientRecv.Value3);
	printf("Client_RecvData Value4 : %d\n", clientRecv.Value4);

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

	SetTimer(NULL, TIMER_ID, 1000, (TIMERPROC)TimerProc);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	closesocket;
}