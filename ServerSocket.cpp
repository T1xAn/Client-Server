#include "ServerSocket.h"

namespace ServerSocket {

	void ServerSocket::CleanWinsock() {
		closesocket(_socket);
		WSACleanup();
		std::cout << " Библиотека Winsock и работающие сокеты очищены " << std::endl;
	}

	bool ServerSocket::InitializeSocket(int addresss_specification, int specification_type) {

		_socket = socket(addresss_specification, specification_type, 0);

		if (_socket == INVALID_SOCKET) {
			std::cout << "Ошибка инициализация сокета" << std::endl;
			return FALSE;
		}
		std::cout << "Инициализация сокета прошла успешно" << std::endl;
		return TRUE;
	}

	bool ServerSocket::SocketStartListening() {

		if (_socketData.sin_family != AF_INET)
			_socketData.sin_family = AF_INET;

		_socketData.sin_port = htons(_port);
		_socketData.sin_addr.s_addr = htonl(INADDR_ANY);
		int res = bind(_socket, (sockaddr*)&_socketData, sizeof(_socketData));

		if (res == SOCKET_ERROR) {

			std::cout << "Ошибка при прявязывании сокета" << std::endl;
			return FALSE;
		}

		std::cout << "Сокет успешно привязан " << std::endl;

		res = listen(_socket, 20);
		if (res == SOCKET_ERROR) {

			std::cout << "Ошибка перехода сокета в режим ожидания" << std::endl;
			return FALSE;
		}

		std::cout << "Сокет переведён в состояние ожидания " << std::endl;
		return TRUE;
	}

	SOCKET ServerSocket::SocketAcceptConnection() {
		SOCKET newClient;

		newClient = accept(_socket, NULL, NULL);
		if (newClient == INVALID_SOCKET) {

			std::cout << "Ошибка принятии запроса соединения" << std::endl;
			return 0;
		}

		return newClient;

	}
}