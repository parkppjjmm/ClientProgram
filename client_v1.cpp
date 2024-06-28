#include<stdio.h>
#include<stdlib.h>
#include <thread>
#include<winsock2.h>
#include<iostream>
#include<string>
#include<sstream>
#include<fstream>

#pragma comment (lib, "Ws2_32.lib")

#define PORT 8000
#define PACKET_SIZE 1000
#define SERVER_IP "xxx.xxx.xxx.xxx" // 서버 아이피
#define CAMERA_BUFFER_Value2 100

using namespace std;

using std::thread;

typedef struct _EXAMPLE_SEND_PACKET
{
	int32_t Counter;
	char Value1[16];
	int32_t Value2;
	int32_t Value3;
	int32_t Value4;
}_EXAMPLE_SEND_PACKET;
// 32bytes

typedef struct _EXAMPLE_RECV_PACKET {
	int32_t Counter;
	bool Value5;
	bool Value6;
	bool Value7;
	bool Value8;
}_EXAMPLE_RECV_PACKET;
// 8bytes


////////////// Socket 통신을 위한 struct //////////////////
struct _EXAMPLE_SEND_PACKET clientRecv;
struct _EXAMPLE_RECV_PACKET ServerRequest;

int main(int argc, char* argv[], char* envp[])
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	////    1. TCP/IP 소켓 통신 시작
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	ServerRequest.Counter = 0; //통신을 위한 Counter 값 0으로 초기화

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hSocket; //소켓 생성
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(PORT);
	tAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	///////////////////////// 무한 루프 ////////////////////////////
	while (true)
	{
		/////////////// 서버쪽으로 Counter 증가 신호 전송 /////////////////
		ServerRequest.Counter = ServerRequest.Counter + 1; // 일정 값 이상일 때 초기화 필요 없음.
		ServerRequest.Value5 = TRUE;
		ServerRequest.Value6 = TRUE;
		ServerRequest.Value7 = TRUE;
		ServerRequest.Value8 = TRUE;

		/////////////////// 서버쪽으로 연결 요청 /////////////////////
		connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));

		/////////////////// 서버쪽으로 전송 //////////////////////////
		send(hSocket, (char*)&ServerRequest, sizeof(struct _EXAMPLE_RECV_PACKET), 0); //서버에 메세지를 전달

		/////////////////// 서버로부터 데이터 수신 ///////////////////
		recv(hSocket, (char*)&clientRecv, PACKET_SIZE, 0);

		printf("----------------------------------------------------------------\n");
		printf("-------------------------- Client Part -------------------------\n");
		printf("----------------------------------------------------------------\n");
		printf("Client_RecvData Counter : %d\n", clientRecv.Counter);
		printf("Client_RecvData Value1 : %s\n", clientRecv.Value1);
		printf("Client_RecvData Value2 : %d\n", clientRecv.Value2);
		printf("Client_RecvData Value3 : %d\n", clientRecv.Value3);
		printf("Client_RecvData Value4 : %d\n", clientRecv.Value4);
		printf("----------------------------------------------------------------\n");

		Sleep(1000);	//1000ms 일시 정지 → 초당 1번
	}
	closesocket(hSocket);
	WSACleanup(); // WSAStartup을 하면서 지정한 내용을 제거
	return 0;
}
