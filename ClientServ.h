#pragma once
#include <Windows.h>
#include <iostream>

#define DEFAULT_PORT 3020

namespace ClientServ {

	DWORD WINAPI ServingClient(void* ClientData);

	class ClientServ {
		
	public:

		ClientServ(WSADATA wsdDada, SOCKET socket) {
			_winsockData = wsdDada;
			_socket = socket;
			strcpy_s(_cryptKey, "ClientServerKey");
		}

		void CloseClientSocket();
		void GetClientIpAddress();

		SOCKET ReturnSocket() { return _socket; }

		bool ReciveData(char* Data);

		bool SendData(char* Data);

		char _ipAddress[20];

		void CryptData(char* source);

		char _cryptKey[20];
		
	private:

		WSADATA  _winsockData;			// структура для WSAStartup
		SOCKET _socket;				// гнездо
		sockaddr_in _socketData;			// структура данных гнезда

	};
}