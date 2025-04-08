#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "../include/Server.h"


namespace MyChat
{
	ServerKernel::ServerKernel()
	{
		memset(&m_ListenSocket, 0, sizeof(m_SocketAddr_Info));
		memset(&m_SocketAddr_Info, 0, sizeof(m_SocketAddr_Info));
	}

	ServerKernel::~ServerKernel()
	{

	}

	int ServerKernel::init()
	{
		// ���������
		WORD wVersionRequird = MAKEWORD(2, 2);	// MAKEWORD(���汾,���汾)
		WSADATA wdScokMsg;
		int status = WSAStartup(wVersionRequird, &wdScokMsg);
		switch (status)
		{
		case WSASYSNOTREADY:
		{
			std::cout << "[ServerKernel][Init Error]: �����������ԣ����߼�������" << std::endl;
			return -1;
		}
		break;
		case WSAVERNOTSUPPORTED:
		{
			std::cout << "[ServerKernel][Init Error]: ����������" << std::endl;
			return -1;
		}
		break;
		case WSAEPROCLIM:
		{
			std::cout << "[ServerKernel][Init Error]: �볢�Թص�����Ҫ���������Ϊ��ǰ���������ṩ�������Դ" << std::endl;
			return -1;
		}
		break;
		case WSAEINPROGRESS:
		{
			std::cout << "[ServerKernel][Init Error]: ����������" << std::endl;
			return -1;
		}
		break;
		case SEC_E_OK:
		{
			std::cout << "[ServerKernel]: WSA Start Success!" << std::endl;
		}
		break;
		default:
			std::cout << "[ServerKernel][Init Error]: Unknown Error, Code: " << status << std::endl;
			return -1;
		}

		// У��汾
		if (2 != HIBYTE(wdScokMsg.wVersion) || 2 != LOBYTE(wdScokMsg.wVersion))
		{
			std::cout << "[ServerKernel][Init Error]: �汾������" << std::endl;
			WSACleanup();
			return -1;
		}

		return 0;
	}

	int ServerKernel::start()
	{
		// ����������socket�������׽��֣�
		m_ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (INVALID_SOCKET == m_ListenSocket)
		{
			std::cout << "[ServerKernel][Start Error]: ����socketʧ�� error:" << WSAGetLastError() << std::endl;
			WSACleanup();
			return -1;
		}

		// �󶨵�ַ
		std::string ipv4_address = "127.0.0.1";
		USHORT port = 55564;

		m_SocketAddr_Info.sin_family = AF_INET;
		m_SocketAddr_Info.sin_addr.s_addr = inet_addr(ipv4_address.c_str());	// INADDR_ANY --- �κε�ַ������
		m_SocketAddr_Info.sin_port = htons(port);
		if (SOCKET_ERROR == bind(m_ListenSocket, (struct sockaddr*)&m_SocketAddr_Info, sizeof(m_SocketAddr_Info)))
		{
			std::cout << "[ServerKernel][Start Error]: �󶨵�ַʧ�� error:" << WSAGetLastError() << std::endl;
			closesocket(m_ListenSocket);
			WSACleanup();
			return -1;
		}
		std::cout << "[ServerKernel]: �󶨵�ַ: " << ipv4_address << "�˿�: " << port << std::endl;

		// ��ʼ����
		if (SOCKET_ERROR == listen(m_ListenSocket, SOMAXCONN))
		{
			std::cout << "[ServerKernel][Start Error]: ����ʧ�� error:" << WSAGetLastError() << std::endl;
			closesocket(m_ListenSocket);
			WSACleanup();
			return -1;
		}
		std::cout << "[ServerKernel]: ��ʼ����" << std::endl;

		m_bRunning = true;
		m_uTickTime = GetTickCount64();
		return 0;
	}

	void ServerKernel::update()
	{
		auto time = GetTickCount64();
		if (time - m_uTickTime > 20 * 1000) {
			m_bRunning = false;
			std::cout << "[ServerKernel][Update Error]: Listen Out Of Time." << std::endl;
			closesocket(m_ListenSocket);
			WSACleanup();
		}

		// ��������
		SOCKADDR_IN sockClient;
		int nLen = sizeof(sockClient);
		SOCKET socketClient = accept(m_ListenSocket, (struct sockaddr*)&sockClient, &nLen);
		if (INVALID_SOCKET == socketClient)
		{
			std::cout << "[ServerKernel][Update Error]: ��������ʧ�� error:" << WSAGetLastError() << std::endl;
			closesocket(m_ListenSocket);
			WSACleanup();
		}
		std::cout << "[ServerKernel]: �ͻ������ӳɹ�" << std::endl;
		std::cout << "[ServerKernel]: �ͻ��˵�ַ: " << sockClient.sin_addr.s_addr << "�˿ں�: " << sockClient.sin_port << std::endl;
	}

	int ServerKernel::isRunning()
	{
		return m_bRunning;
	}

	int ServerKernel::GetErrorCode()
	{
		return 0;
	}
}