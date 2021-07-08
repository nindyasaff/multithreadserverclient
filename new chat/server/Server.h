#pragma once
#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <thread>
#pragma comment(lib,"ws2_32.lib")

class Server {
private:
	// For winshok
	WSADATA wsData;
	// Version
	WORD ver;
	// For checking winshock
	int wsOk;
	// Server socket
	SOCKET listening;
	//
	sockaddr_in hint;
	// Kind of list or array
	fd_set master;

	// Bool server
	bool running;

	// Looping in server
	void LoopServer();
	// Close server method
	void ClosingServer();

public:
	// Constructor
	Server();
	// Start the server
	void ServerBegin();
};