#pragma once
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#pragma comment (lib, "ws2_32.lib")

class Client {
private:
	// Ip addres server
	std::string ipAddress;
	// The port
	int port;
	// The data
	WSAData data;
	// Version
	WORD ver;
	//
	int wsResult;
	// The socket
	SOCKET sock;
	//
	sockaddr_in hint;

	// Method to recieve data
	void recv_socket(SOCKET sock);
	// Method to send data
	void SentData();

public:
	// Constructor
	Client();

	// Begin recieve and send
	void Begin();
	std::string username;
};