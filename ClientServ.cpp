#define _CRT_SECURE_NO_WARNINGS
#include "ClientServ.h"

namespace ClientServ {

	void ClientServ::GetClientIpAddress() {
		
		int len = sizeof(sockaddr_in);
		getpeername(_socket, (sockaddr*)&_socketData, &len);
		
		strcpy(_ipAddress,inet_ntoa(_socketData.sin_addr));
		std::cout << "Ip адрес нового подключенного клиента:  " << _ipAddress << std::endl;

	}

	void ClientServ::CloseClientSocket() {
		closesocket(_socket);
	}
	
	bool ClientServ::ReciveData(char* Data) {
		int result = recv(_socket, Data, 99, 0);

		if (result == SOCKET_ERROR) {
			std::cout << "Ошибка получения" << std::endl;
			return FALSE;
		}

		int EncFlag = Data[0];
		char* ptr = &Data[1];
		MoveMemory(Data, ptr, result - 1);
		Data[result - 1] = '\0';

		Data[result] = '\0';
		if (EncFlag == 1) {
			CryptData(Data);
			std::cout << "Зашифрованное cообщение получено" << std::endl;

		}
		else if(EncFlag == 2) {
			std::cout << "Сообщение получено" << std::endl;
		}

		std::cout << "Содержание сообщение: " << Data << std::endl;
		if (EncFlag == 1)
			Data[0] = '\0';
		return TRUE;
	}
	
	bool ClientServ::SendData(char* Data) {

		if (send(_socket, Data, strlen(Data), 0)) {
			std::cout << "Ответ отправлен" << std::endl;
			return TRUE;
		}

		std::cout << "Ошибка при отправке сообщения" << std::endl;
		return FALSE;
	}

	unsigned short gamma(char* pwd) {
		char buf[20];
		int i;
		unsigned long flag;
		static unsigned long g;
		if (pwd) {
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


	void ClientServ::CryptData(char* source) {
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

	DWORD WINAPI ServingClient(void* ClientData) {
		char    DataBuffer[1000];
		ClientServ* ClientServiceSocket = (ClientServ*)ClientData;

		while (TRUE) {
			ZeroMemory(DataBuffer, 1000);
			char SendAns[100];
			if (ClientServiceSocket->ReciveData(DataBuffer)) {
				if (DataBuffer[0] == '\0')
					strcpy_s(SendAns, "Зашифрованное cообщение было обработано сервером");
				else
					strcpy_s(SendAns, "Сообщение было обработано сервером");
				if (!ClientServiceSocket->SendData(SendAns))
					break;
			}
			else {
				std::cout << "Сокет клиента "  << ClientServiceSocket->_ipAddress << " будет закрыт " << std::endl;
				
				ClientServiceSocket->CloseClientSocket();
				break;
			}
		}
		std::cout << "Отключение клиента... " << std::endl;
		return 0;
	}
}