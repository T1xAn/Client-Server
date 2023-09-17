#pragma once
#include <Windows.h>
#include <iostream>

#define DEFAULT_PORT 3020
#define LOCALSERVER_IP 16777343
namespace Socket {

	class ConnectSocket {

	public:

		ConnectSocket(WSADATA winsockData) {
			_winsockData = winsockData;
			_port = DEFAULT_PORT;
			_serverIp = LOCALSERVER_IP;
			_socket = NULL;
			strcpy_s(_cryptKey, "ClientServerKey");
		}

		~ConnectSocket() {
		}

		bool InitializeSocket(int addresss_specification, int specification_type);

		void SetServer_IPadress(char ip[]);

		bool ConnetcToServer();

		void CleanWinsock();

		bool SocketSend(char data[]);

		bool SocketRecive(char* resData);

		bool CheckConnectionStatus(); //// определение

		void CryptData(char* source);


		USHORT _port;
		ULONG _serverIp;
		char _cryptKey[20];

	private:

		WSADATA _winsockData;			// структура для WSAStartup
		SOCKET _socket;				// гнездо
		sockaddr_in _socketData;			// структура данных гнезда // Заменить на ту что в getaddrinfo

	};
}