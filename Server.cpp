#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include<WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

void ErrorMessage(const char* message);
void DrawLine();
void TcpServer();
void UdpServer();
void GetAddress();

int port;

int main(int argc, char* argv[]) {
	WSADATA wsa;
	
	int sel;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		ErrorMessage("�ʱ�ȭ ����");

	DrawLine();
	printf("����ϰ��� �ϴ� ��������\n");
	printf(" 1. TCP  2. UDP\n��ȣ�Է�: ");
	scanf_s("%d", &sel);

	while (sel != 1 && sel != 2) {
		DrawLine();
		printf("�ùٸ� ��ȣ�� �Է����ּ���.\n��ȣ�Է�: ");
		scanf_s("%d", &sel);
	}

	DrawLine();
	printf("����ϰ��� �ϴ� ��Ʈ��ȣ: ");
	scanf_s("%d", &port);

	switch(sel) {
	case 1 : TcpServer();
		break;
	case 2 : UdpServer();
		break;
	}

	WSACleanup();
	DrawLine();
	printf("������ �����մϴ�.\n\n");
	system("pause");
	return 0;
}

void ErrorMessage(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void DrawLine() {
	for (int i = 0; i < 32; i++)
		printf("-");
	printf("\n");
}

void TcpServer() {
	SOCKET sock1, sock2;
	SOCKADDR_IN server, client;

	int len;
	char toClient[] = "���� ����!";

	sock1 = socket(AF_INET, SOCK_STREAM, 0);

	if (sock1 == INVALID_SOCKET)
		ErrorMessage("���� ���� ����");

	GetAddress();
	printf("TCP Ŭ���̾�Ʈ �����\n");

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	if (bind(sock1, (SOCKADDR*)&server, sizeof(server)))
		ErrorMessage("���ε� ����");

	if (listen(sock1, 5))
		ErrorMessage("listen ����");

	len = sizeof(client);
	sock2 = accept(sock1, (SOCKADDR*)&client, &len);

	if (sock2 == INVALID_SOCKET)
		ErrorMessage("���� ����");

	send(sock2, toClient, sizeof(toClient), 0);

	while (1) {
		printf("Message to Server: ");
		scanf_s("%s", toClient, sizeof(toClient));
		send(sock2, toClient, sizeof(toClient), 0);

		if (!strcmp(toClient,"exit"))
			break;
	}


	closesocket(sock2);
	closesocket(sock1);

}

void UdpServer() {
	SOCKET sock;
	SOCKADDR_IN server, client;

	char toClient[] = "���� ����!";
	char buf[1];
	int buf_len;
	int server_len;

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock == INVALID_SOCKET)
		ErrorMessage("���� ���� ����");

	GetAddress();

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(port);

	if (bind(sock, (SOCKADDR*)&server, sizeof(server)))
		ErrorMessage("���ε� ����");

	printf("UDP Ŭ���̾�Ʈ �����\n");

	server_len = sizeof(client);
	if (buf_len = recvfrom(sock, buf, 1, 0, (SOCKADDR*)&client, &server_len) < 0) {
		ErrorMessage("����\n");
	}

	sendto(sock, toClient, strlen(toClient), 0, (SOCKADDR*)&client, sizeof(client));

	while(1) {
		printf("Message to Client: ");
		scanf_s("%s", toClient, sizeof(toClient));
		sendto(sock, toClient, strlen(toClient), 0, (SOCKADDR*)&client, sizeof(client));

		if (!strcmp(toClient, "exit"))
			break;
	}

}

void GetAddress() {
	char hostName[512];
	char ipAddr[32];
	struct addrinfo hint, *res;

	memset(&hint, 0, sizeof(hint));
	hint.ai_family = PF_INET;

	gethostname(hostName, sizeof(hostName));
	DrawLine();
	printf("ȣ��Ʈ�̸�: %s\n", hostName);

	getaddrinfo(hostName, NULL, &hint, &res);
	inet_ntop(AF_INET, &((struct sockaddr_in *) res->ai_addr)->sin_addr, ipAddr, sizeof(ipAddr));
	printf("ȣ��Ʈ IP �ּ�: %s\n", ipAddr);
	printf("��Ʈ��ȣ: %d\n", port);
	DrawLine();

	freeaddrinfo(res);

}