#include "ServerSocket.h"

namespace ServerSocket {

	void ServerSocket::CleanWinsock() {
		closesocket(_socket);
		WSACleanup();
		std::cout << " ���������� Winsock � ���������� ������ ������� " << std::endl;
	}

	bool ServerSocket::InitializeSocket(int addresss_specification, int specification_type) {

		_socket = socket(addresss_specification, specification_type, 0);

		if (_socket == INVALID_SOCKET) {
			std::cout << "������ ������������� ������" << std::endl;
			return FALSE;
		}
		std::cout << "������������� ������ ������ �������" << std::endl;
		return TRUE;
	}

	bool ServerSocket::SocketStartListening() {

		if (_socketData.sin_family != AF_INET)
			_socketData.sin_family = AF_INET;

		_socketData.sin_port = htons(_port);
		_socketData.sin_addr.s_addr = htonl(INADDR_ANY);
		int res = bind(_socket, (sockaddr*)&_socketData, sizeof(_socketData));

		if (res == SOCKET_ERROR) {

			std::cout << "������ ��� ������������ ������" << std::endl;
			return FALSE;
		}

		std::cout << "����� ������� �������� " << std::endl;

		res = listen(_socket, 20);
		if (res == SOCKET_ERROR) {

			std::cout << "������ �������� ������ � ����� ��������" << std::endl;
			return FALSE;
		}

		std::cout << "����� �������� � ��������� �������� " << std::endl;
		return TRUE;
	}

	SOCKET ServerSocket::SocketAcceptConnection() {
		SOCKET newClient;

		newClient = accept(_socket, NULL, NULL);
		if (newClient == INVALID_SOCKET) {

			std::cout << "������ �������� ������� ����������" << std::endl;
			return 0;
		}

		return newClient;

	}
}