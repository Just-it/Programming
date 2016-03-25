// UDPclient.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <WinSock2.h>
#include "Ws2tcpip.h"
using namespace std;
#pragma comment(lib, "ws2_32.lib")
const int MAX_REQUEST = 1024;
const int BUF_SIZE = 4096;
const char* DEFAULT_PORT = "1000";

int _tmain(int argc, _TCHAR* argv[])
{
	//�����׽��ֿ�
	WSADATA wsadata;
	SOCKET SocketSrv = INVALID_SOCKET;
	SOCKET SocketClt = INVALID_SOCKET;
	struct addrinfo *result = NULL,hits;
	int err;
	err = WSAStartup(MAKEWORD(2,2),&wsadata);
	if (0 != err)
	{
		cout << "WSAStartup() fail" <<endl;
		return 0;
	}
	//�����׽���
	ZeroMemory(&hits,sizeof(struct addrinfo));
	hits.ai_family = AF_INET;
	hits.ai_socktype = SOCK_DGRAM;
	hits.ai_protocol = IPPROTO_UDP;
	hits.ai_flags = AI_PASSIVE;
	err = getaddrinfo(NULL,DEFAULT_PORT,&hits,&result);
	if (0 != err)
	{
		cout << "getaddrinfo() failed!" << endl;
		WSACleanup();
		return 0;
	}
	SocketClt = socket(result->ai_family,result->ai_socktype,result->ai_protocol);
	if (INVALID_SOCKET == SocketClt)
	{
		cout << "create socket failed :" << WSAGetLastError() <<endl;
		WSACleanup();
		return 0;
	}
	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_port = htons(1000);
	addrSrv.sin_family = AF_INET;
	//��������
	int len = sizeof(SOCKADDR);
	char* sendbuff = new char[MAX_REQUEST];
	char* recvbuff = new char[MAX_REQUEST];
	while(1)
	{
		//��������
		cout << "input your ask: ";
		cin.getline(sendbuff,MAX_REQUEST);
		err = sendto(SocketClt,sendbuff,strlen(sendbuff)+1,0,(SOCKADDR*)&addrSrv,len);
		if (SOCKET_ERROR == err)
		{
			cout << "send failed!" << endl;
		}

		//��������
		err = recvfrom(SocketClt,recvbuff,MAX_REQUEST,0,(SOCKADDR*)&addrSrv,&len);
		if (SOCKET_ERROR == err)
		{
			cout << "recv failed!" << endl;
			delete[] recvbuff;
			delete[] sendbuff;
			return 0;
		}
		else if ('q' == recvbuff[0])
		{
			sendto(SocketClt,"q",strlen("q")+1,0,(SOCKADDR*)&addrSrv,len);
			cout << "chat over!" << endl;
			break;
		}
		else
			cout << "recieve a information from " <<inet_ntoa(addrSrv.sin_addr) << " : " << recvbuff << endl;
	}
	//�ر��׽���
	closesocket(SocketClt);
	WSACleanup();
	delete[] recvbuff;
	delete[] sendbuff;
	return 0;
}

