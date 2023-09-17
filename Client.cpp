#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <conio.h>
#include <stdio.h>
#include "ConnectSocket.h"
#include <locale>

#pragma once
#pragma comment (lib,"wsock32.lib")
#pragma comment (lib, "ws2_32.lib")

#define PORT_NUMBER 3030

int main()
{
	WSADATA  WinSocketData;	

	setlocale(LC_ALL, "Russian");
	if (WSAStartup(MAKEWORD(2, 0), &WinSocketData) != 0) // инициализировать библиотеку WinSock
	{
		std::cout << "Ошибка: Запуска Winsock!\n";
		system("pause");
		return 1;
	}
	Socket::ConnectSocket ClientServerSocket(WinSocketData);
	ClientServerSocket.InitializeSocket(AF_INET, SOCK_STREAM);

	char DataBuffer[1000];
	std::cout << "Введите IP адресс сервера (по умолчанию 127.0.0.1):";
	std::cin.getline(DataBuffer,1000);
	ClientServerSocket.SetServer_IPadress(DataBuffer);

	if (!ClientServerSocket.ConnetcToServer()) {
		std::cout << "Ошибка сокета при подключении: приложение будет закрыто" << std::endl;
		ClientServerSocket.CleanWinsock();
		system("pause");
		return 1;
	}
	std::cout << "===========================================" << std::endl
		<< "Нажмите ESC что бы завершить работу клиента " << std::endl
		<< "===========================================" << std::endl;
	while (TRUE) {
		std::cout << "Нажмите Y что бы отправить зашифрованное сообщение или N для обычного " << std::endl;
		int Key = _getch();
		ZeroMemory(DataBuffer, 1000);
		switch (Key) {
			case 27: {
				ClientServerSocket.CleanWinsock();
				std::cout << "Обработка закончена, заврешение работы..." << std::endl;
				system("pause");
				return 0;
			}
			case 121: { // Y
				std::cout << "Введите текст сообщения: " << std::endl;
				std::cin.getline(DataBuffer, 1000);
				ClientServerSocket.CryptData(DataBuffer);
				char* ptr = &DataBuffer[1];
				int DataOffset = strlen(DataBuffer);
				MoveMemory(ptr, DataBuffer, DataOffset);
				DataBuffer[DataOffset+1] = '\0';
				DataBuffer[0] = 1;
				break;
			}
			case 110: { // N
				std::cout << "Введите текст сообщения: " << std::endl;
				std::cin.getline(DataBuffer, 1000);
				char* ptr = &DataBuffer[1];
				int DataOffset = strlen(DataBuffer);
				MoveMemory(ptr, DataBuffer, DataOffset);
				DataBuffer[DataOffset+1] = '\0';
				DataBuffer[0] = 2;
				break;
			}
			default:
				continue;
		}

		bool result = ClientServerSocket.SocketSend(DataBuffer);

		if (result) {
			ZeroMemory(DataBuffer, sizeof(DataBuffer));
			result = ClientServerSocket.SocketRecive(DataBuffer);

			if (result) {
				std::cout << "Полученное сообщение: " << DataBuffer << std::endl;
			}
		}
	}
	
}

