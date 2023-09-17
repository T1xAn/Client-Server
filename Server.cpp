#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <conio.h>
#include <process.h>
#include <stdio.h>
#include <iostream>
#include <locale>
#include <vector>

#include "ServerSocket.h"
#include "ClientServ.h"

#pragma once
#pragma comment (lib,"wsock32.lib")
#pragma comment (lib, "ws2_32.lib")

std::vector <std::pair<ClientServ::ClientServ*,HANDLE>> Clients;

DWORD WINAPI AcceptConnectionsThread(void* ServerSocket) {
	while (TRUE) {
		ServerSocket::ServerSocket* ServerRecSocket = (ServerSocket::ServerSocket*)ServerSocket;
		SOCKET newClientSocket;
		/// ConditionFunc, WSACONNECT, recivefrom, sendto
		newClientSocket = ServerRecSocket->SocketAcceptConnection();
		if (newClientSocket == INVALID_SOCKET) {
			std::cout << "Не удалось принять соединение от клиента " << std::endl;
			continue;
		}
		
		ClientServ::ClientServ* newClient = new ClientServ::ClientServ(ServerRecSocket->_winsockData, newClientSocket);
		newClient->GetClientIpAddress();
		HANDLE newClientServThread = CreateThread(NULL, 0, ClientServ::ServingClient, (void*)newClient, 0, 0);
		Clients.push_back({ newClient, newClientServThread });

	}
}


int main() 
{
	setlocale(LC_ALL, "Russian");

	WSADATA WinSocketData;
	if (WSAStartup(MAKEWORD(2, 0), &WinSocketData) != 0) 
	{
		std::cout << "Ошибка: Запуска Winsock!\n";
		system("pause");
		return 1;
	}

	ServerSocket::ServerSocket* ServerRecSocket = new ServerSocket::ServerSocket(WinSocketData);

	if (!ServerRecSocket->InitializeSocket(AF_INET, SOCK_STREAM)) {

		std::cout << "Ошибка создания главного сокета, сервер будет отключён" << std::endl;
		WSACleanup();
		system("pause");
		return 1;
	}

	if (!ServerRecSocket->SocketStartListening()) {
		
		std::cout << "Ошибка сокета сервера, сервер будет отключён " << std::endl;
		ServerRecSocket->CleanWinsock();
		system("pause");
		return 1;
	}
	std::cout << "===========================================" << std::endl
		<< "Нажмите ESC что бы завершить работу сервера " << std::endl
		<< "===========================================" << std::endl;

	HANDLE AcceptConnections = CreateThread(NULL, 0, AcceptConnectionsThread,
		(void*)ServerRecSocket, 0, 0);
	while (TRUE) {
		int key = _getch();
		if (key == 27)
			break;
	}
	CloseHandle(AcceptConnections);

	std::pair<ClientServ::ClientServ*, HANDLE> DelConnection;
	while (Clients.size())
	{
		DelConnection = Clients[Clients.size()-1];
		DelConnection.first->CloseClientSocket();
		CloseHandle(DelConnection.second);
		Clients.pop_back();
	}

	WSACleanup();
	delete ServerRecSocket;
	return 0;
}



