#include "winsock2.h"
#include "iostream"
#include "Ws2tcpip.h"
using namespace std;

#pragma comment(lib, "ws2_32.lib")
const int MAX_REQUEST = 1024;
const int BUF_SIZE = 4096;
const char* DEFAULT_PORT = "1000";
//服务端
int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET SocketForListen = INVALID_SOCKET;
	SOCKET SocketForClient = INVALID_SOCKET;
	struct addrinfo *result = NULL, hints;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != iResult)
	{
		cout << "WSAStartup() Failed: " << iResult << endl;
		return 1;
	}

	SOCKADDR_IN AddrServerService; //地址
	AddrServerService.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	AddrServerService.sin_family = AF_INET;
	AddrServerService.sin_port = htons(1000);

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (0 != iResult)
	{
		cout << "getaddrinfo failed: " << iResult << endl;
		WSACleanup();
		return 1;
	}
	SocketForListen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (INVALID_SOCKET == SocketForListen)
	{
		cout << "socket failed: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	iResult = bind(SocketForListen, result->ai_addr, (int)result->ai_addrlen);
	if (SOCKET_ERROR == iResult)
	{
		cout << "bind failed: " <<  WSAGetLastError() << endl;
		freeaddrinfo(result);
		closesocket(SocketForListen);
		WSACleanup();
		return 1;
	}
	cout << "Bind" << endl;
	freeaddrinfo(result);
	iResult = listen(SocketForListen, SOMAXCONN);
	cout << "Start Listen..." << endl;
	if (iResult == SOCKET_ERROR)
	{
		cout << "Listen failed: " <<  WSAGetLastError() << endl;
		closesocket(SocketForListen);
		WSACleanup();
		return 1;
	}

	while(1)
	{
		cout << "Ready to accept: " << endl;
		SocketForClient = accept(SocketForListen, NULL, NULL);
		cout << "Accept a connection" << endl;
		if (INVALID_SOCKET == SocketForClient)
		{
			cout << "accept failed: " << WSAGetLastError() << endl;
			closesocket(SocketForListen);
			break;// 等待连接错误，退出循环
		}
		//可以在这里为每一个连接创建一个数据发送的接收线程，使服务端可以立即接收其它客户
		//端的连接
		char *szRecvBuf = NULL;
		szRecvBuf = new char[MAX_REQUEST];
		iResult = recv(SocketForClient, szRecvBuf, MAX_REQUEST, 0);
		if (0 == iResult) // connection has been closed
		{
			cout << "Connection closing..." << endl;
			delete[] szRecvBuf;
			closesocket(SocketForClient);
			return 1;
		}
		else if (SOCKET_ERROR == iResult) //recv error,socket error
		{
			cout << "recv Failed" << endl;
			delete[] szRecvBuf;
			closesocket(SocketForClient);
			return 1;
		}
		else if (iResult > 0) //接收成功
		{
			cout << iResult << " Has been reveived: " << szRecvBuf;
		}   
		closesocket(SocketForClient);
	}
	return 0;
}