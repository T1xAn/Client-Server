#pragma once
#include <Windows.h>
#include <iostream>

#define DEFAULT_PORT 3020

namespace ServerSocket {

	class ServerSocket {

	public:

		ServerSocket(WSADATA winsockData) {
			_winsockData = winsockData;
			_port = DEFAULT_PORT;
			_socket = NULL;

		}

		bool InitializeSocket(int addresss_specification, int specification_type);

		void CleanWinsock();

		bool SocketStartListening();

		SOCKET SocketAcceptConnection();

		

		USHORT	_port;
		WSADATA  _winsockData;			// структура для WSAStartup

	private:

		
		
		SOCKET _socket;				// гнездо
		sockaddr_in _socketData;			// структура данных гнезда
	};
}