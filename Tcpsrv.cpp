#include <WinSock2.h>
#include <stdio.h>

int main()
{
	//�����׽���
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
	//�������ڼ������׽���
	SOCKET sockSvr = socket(AF_INET,SOCK_STREAM,0);
	SOCKADDR_IN addSvr;
	addSvr.sin_family = AF_INET;
	addSvr.sin_family = htons(6000);
	addSvr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//���׽���
	bind(sockSvr,(SOCKADDR*)&addSvr,sizeof(SOCKADDR));
	//���׽�������Ϊ����ģʽ��׼�����ܿͻ�����
	listen(sockSvr,5);
	char buff[1024];
	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while (true)
	{
		//�ȴ��ͻ�����
		SOCKET sockcnn = accept(sockSvr,(SOCKADDR*)&addrClient,&len);
		//��������
		char sendbuff[1024];
		sprintf_s(sendbuff,"Welcome %s to http://www.css.bolg.com",inet_ntoa(addrClient.sin_addr));
		send(sockcnn,sendbuff,sizeof(sendbuff)+1,0);
		//��������
		char recvbuff[1024];
		recv(sockcnn,recvbuff,1024,0);
		printf("%s\n",recvbuff);
		closesocket(sockcnn);
	}
	return 0;
}