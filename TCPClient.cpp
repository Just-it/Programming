// TCPClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <winsock2.h>
#include <iostream>
using namespace std;
#define RECV_BUFFER_SIZE 8192
#pragma  comment(lib, "ws2_32.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaDate; 
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaDate); //initiate socket libary.
	if (iResult != 0)
	{
		cout << "Error at WSAStartUP()\n";
	}

	SOCKET ConnectSocket; //socket
	ConnectSocket = socket(AF_INET, //IPV4
		SOCK_STREAM, //TCP
		0);
	if (ConnectSocket == INVALID_SOCKET)
	{
		cout << "Error at Socket(): " << WSAGetLastError();
		WSACleanup();
		return 1;
	}

	//set the client's protocol,address,port
	SOCKADDR_IN AddrClientService; //地址
	AddrClientService.sin_family = AF_INET;
	AddrClientService.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	AddrClientService.sin_port = htons(1000); 

	char* recvbuf = NULL;  //receive buffer
	int iBytesSent;
	int iBytesRecv = 0;
	char cSendBuf[32] = "Get Information "; //默认发送的数据

	if (connect(ConnectSocket,
		(SOCKADDR*)&AddrClientService,
		sizeof(AddrClientService)) == SOCKET_ERROR)
	{
		cout << "Failed To Connect" << endl;
		WSACleanup();
		return 1;
	}

	iBytesSent = send(ConnectSocket, cSendBuf, strlen(cSendBuf)+1, 0);
	if (iBytesSent == SOCKET_ERROR)
	{
		cout << "send error " << WSAGetLastError() << endl;
		closesocket(ConnectSocket);
		return 1;
	}
	cout << "Bytes Sent: " << iBytesSent << endl;

	recvbuf = new char[RECV_BUFFER_SIZE];
	while(iBytesRecv != SOCKET_ERROR)
	{
		iBytesRecv = recv(ConnectSocket, recvbuf, RECV_BUFFER_SIZE, 0);
		if (iBytesRecv == 0)
		{
			cout << "Connection Closed" << endl;
			break;
		}
		cout << "Bytes recv: " << iBytesRecv << endl;
	}
	delete[] recvbuf;
	WSACleanup();
	return 0;
}

