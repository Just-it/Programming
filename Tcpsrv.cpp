#include <WinSock2.h>
#include <stdio.h>

int main()
{
	//加载套接字
	WORD wVersionRequested;
	WSADATA wsadata;
	int err;
	wVersionRequested = MAKEWORD(1,1);

	err = WSAStartup(wVersionRequested,&wsadata);
	if (err != 0)
	{
		return;
	}
	if (LOBYTE(wsadata.wVersion)!=1||HIBYTE(wsadata.wHighVersion)!=1)
	{
		WSACleanup();
		return;
	}
	//创建用于监听的套接字
	SOCKET sockSvr = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addSvr;
	addSvr.sin_family = AF_INET;
	addSvr.sin_family = htons(6000);
	addSvr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//绑定套接字
	bind(sockSvr,(SOCKADDR*)&addSvr,sizeof(SOCKADDR));
	//将套接字设置为监听模式，准备接受客户请求
	listen(sockSvr,5);
	char buff[1024];
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while (true)
	{
		//等待客户请求
		SOCKET sockcnn = accept(sockSvr,(SOCKADDR*)&addrClient,&len);
		//发送数据
		char sendbuff[1024];
		sprintf_s(sendbuff,"Welcome %s to http://www.css.bolg.com",inet_ntoa(addrClient.sin_addr));
		send(sockcnn,sendbuff,sizeof(sendbuff)+1,0);
		//接受数据
		char recvbuff[1024];
		recv(sockcnn,recvbuff,1024,0);
		printf("%s\n",recvbuff);
		closesocket(sockcnn);
	}
	return 0;
}