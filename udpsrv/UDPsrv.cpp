// UDPsrv.cpp : 定义控制台应用程序的入口点。
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
	WSADATA wsadata;
	SOCKET SocketSeverve = INVALID_SOCKET;
	SOCKET SocketClient = INVALID_SOCKET;
	struct addrinfo *result = NULL,hits;
	int err;
	//加载套接字库
	err = WSAStartup(MAKEWORD(2,2),&wsadata);
	if (0 != err)
	{
		cout<<"WSAStartup() Failed: " << err << endl;
		return 0;
	}
	//创建套接字
	ZeroMemory(&hits,sizeof(struct addrinfo));
	hits.ai_family = AF_INET;
	hits.ai_socktype = SOCK_DGRAM;
	hits.ai_protocol = IPPROTO_UDP;
	hits.ai_flags = AI_PASSIVE;

	err = getaddrinfo(NULL,DEFAULT_PORT,&hits,&result);
	if (0 != err)
	{
		cout << "getaddrinfo failed: " << err << endl;
		WSACleanup();
		return 0;
	}
	SocketSeverve = socket(result->ai_family,result->ai_socktype,result->ai_protocol);
	if (INVALID_SOCKET == SocketSeverve)
	{
		cout << "socket failed:" << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 0;
	}
	//绑定套接字
	err = bind(SocketSeverve,result->ai_addr,(int)result->ai_addrlen);
	if (SOCKET_ERROR == err)
	{
		cout << "bind failed:" << WSAGetLastError() << endl;
		closesocket(SocketSeverve);
		WSACleanup();
		return 0;
	}
	//等待并接受数据
	int len = sizeof(SOCKADDR);
	char* recvbuff = new char[MAX_REQUEST];
	char* sendbuff = new char[MAX_REQUEST];
	SOCKADDR_IN addrClient;

	while(1)
	{
		//接受数据
		err = recvfrom(SocketSeverve,recvbuff,MAX_REQUEST,0,(SOCKADDR*)&addrClient,&len);
		if (SOCKET_ERROR == err)
		{
			cout << "recv failed!" << endl;
			delete[] recvbuff;
			delete[] sendbuff;
			return 0;
		}
		else if('q' == recvbuff[0])
		{
			sendto(SocketSeverve,"q",strlen("q")+1,0,(SOCKADDR*)&addrClient,len);
			cout << "chat over!" << endl;
			break;
		}
		else
			cout << "recieve a information from " << inet_ntoa(addrClient.sin_addr) <<" : " << recvbuff << endl;

		//发送数据
		cout << "input your reply: ";
		cin.getline(sendbuff,MAX_REQUEST);
		sendto(SocketSeverve,sendbuff,strlen(sendbuff)+1,0,(SOCKADDR*)&addrClient,len);
		if (SOCKET_ERROR == err)
		{
			cout << "send failed!" << endl;
		}
	}
	//关闭套接字
	closesocket(SocketSeverve);
	WSACleanup();
	delete[] recvbuff;
	delete[] sendbuff;
	return 0;
}

