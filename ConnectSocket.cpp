#define _CRT_SECURE_NO_WARNINGS
#include "ConnectSocket.h"

namespace Socket {

	bool ConnectSocket::InitializeSocket(int addresss_specification, int specification_type) {

		_socket = socket(addresss_specification, specification_type, 0);

		if (_socket == INVALID_SOCKET) {
			std::cout << "Ошибка инициализация сокета" << std::endl;
			return FALSE;
		}
		std::cout << "Инициализация сокета прошла успешно" << std::endl;
		return TRUE;
	}

	void ConnectSocket::SetServer_IPadress(char ip[]) {

		if (strlen(ip) >= 7) {
			_serverIp = inet_addr(ip);
		}

		else {
			std::cout << "IP адрес не введён или введён не правильно, будет использован адрес по умолчанию" << std::endl;
			if (_serverIp != LOCALSERVER_IP)
				_serverIp = LOCALSERVER_IP;
			strcpy(ip, "127.0.0.1");
			_serverIp = inet_addr(ip);
		}

		if (_serverIp == INADDR_NONE)
		{
			std::cout << "Ошибка преобразования адреса сервера\n";
			//closesocket(_socket); 
			//WSACleanup();
			return;
		}
		std::cout << "Текущий адрес сервера для подключения " << ip << std::endl;

	}

	bool ConnectSocket::ConnetcToServer() {

		if (_socketData.sin_family != AF_INET)
			_socketData.sin_family = AF_INET;

		_socketData.sin_port = htons(_port);
		_socketData.sin_addr.s_addr = _serverIp;

		int ConnetStatus = connect(_socket, (sockaddr*)&_socketData, sizeof(_socketData));
		DWORD a = GetLastError();
		if (ConnetStatus == SOCKET_ERROR) {
			std::cout << "Ошибка при попытке подключения к серверу " << std::endl;
			//closesocket(_socket); 
			//WSACleanup();
			return FALSE;
		}

		std::cout << "Подключение к серверу прошло успешно" << std::endl;

		return TRUE;
	}

	void ConnectSocket::CleanWinsock(){
		closesocket(_socket); 
        WSACleanup();
		std::cout << " Библиотека Winsock и работаеющие сокеты очищены " << std::endl;
	}

	bool ConnectSocket::SocketSend(char data[]) {

		int result = send(_socket, data, strlen(data), 0);

		if (result == SOCKET_ERROR) {
			std::cout << "Ошибка отправки" << std::endl;
			return FALSE;
		}
		std::cout << "Сообщение отправлено" << std::endl;
		return TRUE;
	}

	bool ConnectSocket::SocketRecive(char* resData) {
		
		int result = recv(_socket, resData, 99, 0);

		if (result == SOCKET_ERROR) {
			std::cout << "Ошибка получения" << std::endl;
			return FALSE;
		}
		std::cout << "Сообщение получено" << std::endl;
		return TRUE;
	}

	unsigned short gamma(char* pwd){
		char buf[20];
		int i;
		unsigned long flag;
		static unsigned long g;
		if (pwd){
			memset(buf, 0x55, 20); 
			memcpy(buf, pwd, strlen(pwd)); 
			for (i = 0, g = 0; i < 20; i++) 
				g += (unsigned long)(buf[i] << (i % 23));
		}

		for (i = 5; i > 0; i--) {
			flag = g & 1;
			g = g >> 1;
			if (flag) g |= 0x80000000;
		}

		return g; 
	}


	void ConnectSocket::CryptData(char* source) {
		char DataBuffer[1000];
		unsigned short* px, * py, g;

		ZeroMemory(DataBuffer, 1000);
		px = (unsigned short*)source;
		py = (unsigned short*)DataBuffer;

		g = gamma(_cryptKey);
		int len = strlen(source);

		for (int i = 0; i < len; i++, py++, px++)
			*py = *px ^ gamma(0);

		DataBuffer[len] = 0;

		MoveMemory(source, DataBuffer, len);
	}

}