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
#define SERVER_IP "192.168.200.100" // ���� ������
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


////////////// Socket ����� ���� struct //////////////////
struct _EXAMPLE_SEND_PACKET clientRecv;
struct _EXAMPLE_RECV_PACKET ServerRequest;

int main(int argc, char* argv[], char* envp[])
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	////    1. TCP/IP ���� ��� ����
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	ServerRequest.Counter = 0; //����� ���� Counter �� 0���� �ʱ�ȭ

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET hSocket; //���� ����
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(PORT);
	tAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	///////////////////////// ���� ���� ////////////////////////////
	while (true)
	{
		/////////////// ���������� Counter ���� ��ȣ ���� /////////////////
		ServerRequest.Counter = ServerRequest.Counter + 1; // ���� �� �̻��� �� �ʱ�ȭ �ʿ� ����.
		ServerRequest.Value5 = TRUE;
		ServerRequest.Value6 = TRUE;
		ServerRequest.Value7 = TRUE;
		ServerRequest.Value8 = TRUE;

		/////////////////// ���������� ���� ��û /////////////////////
		connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));

		/////////////////// ���������� ���� //////////////////////////
		send(hSocket, (char*)&ServerRequest, sizeof(struct _EXAMPLE_RECV_PACKET), 0); //������ �޼����� ����

		/////////////////// �����κ��� ������ ���� ///////////////////
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

		Sleep(1000);	//1000ms �Ͻ� ���� �� �ʴ� 1��
	}
	closesocket(hSocket);
	WSACleanup(); // WSAStartup�� �ϸ鼭 ������ ������ ����
	return 0;
}