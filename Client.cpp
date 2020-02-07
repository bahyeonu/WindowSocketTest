#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

void ErrorMessage(const char* message);
void DrawLine();
void TcpClient();
void UdpClient();

char ip[16];
int port;

int main(int argc, char* argv[]) {
	WSADATA wsa;

	int sel;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		ErrorMessage("초기화 실패");

	DrawLine();
	printf("사용하고자 하는 서버유형\n");
	printf(" 1. TCP  2. UDP\n번호입력: ");
	scanf_s("%d", &sel);

	while (sel != 1 && sel != 2) {
		DrawLine();
		printf("올바른 번호를 입력해주세요.\n번호입력: ");
		scanf_s("%d", &sel);
	}

	DrawLine();
	printf("호스트의 IP주소를 입력해주세요.: ");
	scanf_s("%s", ip, sizeof(ip));
	printf("사용하고자 하는 포트번호: ");
	scanf_s("%d", &port);

	switch (sel) {
	case 1: TcpClient();
		break;
	case 2: UdpClient();
		break;
	}

	WSACleanup();
	system("puase");
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

void TcpClient() {
	SOCKET sock;
	SOCKADDR_IN server;

	char toServer[] = "연결 성공!";
	char fromServer[30];
	int len;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
		ErrorMessage("소켓 생성 실패");

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_port = htons(port);

	if (connect(sock, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR)
		ErrorMessage("연결 실패");

	while (1) {
		if(len = recv(sock, fromServer, sizeof(fromServer), 0) > 0)
			printf("Message from server : %s \n", fromServer);

		if (!strcmp(fromServer, "exit"))
			break;
	}

	DrawLine();
	printf("클라이언트를 종료합니다.\n\n");
	system("pause");
	closesocket(sock);
}

void UdpClient() {
	SOCKET sock;
	SOCKADDR_IN server;

	char fromServer[30];
	int message_len;
	int server_len;

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock == INVALID_SOCKET)
		ErrorMessage("소켓 생성 실패");

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip);
	server.sin_port = htons(port);

	server_len = sizeof(server);
	sendto(sock, NULL, NULL, 0, (SOCKADDR*)&server, server_len);

	if ((message_len = recvfrom(sock, fromServer, sizeof(fromServer), 0, (SOCKADDR*)&server, &server_len)) < 0)
		ErrorMessage("수신실패");

	fromServer[message_len] = '\0';
	printf("Message from server : %s \n", fromServer);

	while (1) {
		message_len = recvfrom(sock, fromServer, sizeof(fromServer), 0, (SOCKADDR*)&server, &server_len);

		if (!strcmp(fromServer, "exit"))
			break;

		fromServer[message_len] = '\0';
		printf("Message from server : %s \n", fromServer);
	}


	DrawLine();
	printf("클라이언트를 종료합니다.\n\n");
	system("pause");
	closesocket(sock);
}