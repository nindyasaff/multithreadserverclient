#include "Server.h"

Server::Server() {
	// Using 2.2 version
	ver = MAKEWORD(2, 2);

	// Initializing Winsock lib
	wsOk = WSAStartup(ver, &wsData);
	// Check is it's ok
	if (wsOk != 0)
	{
		std::cout << "Can`t Initialize Winsock! Quitting\n";
	}

	// Make socket with IPv4, TCP
	listening = socket(AF_INET, SOCK_STREAM, 0);
	// Check if socket ok
	if (listening == INVALID_SOCKET)
	{
		std::cout << "Can`t create a socket! Quitting\n";
	}

	// Bind thd socket to an ip address and port to a socket
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket to start listening
	listen(listening, SOMAXCONN);

	// Add listening socket to master list
	FD_ZERO(&master);
	FD_SET(listening, &master);

	// Set running to true
	running = true;
}

void Server::ServerBegin() {
	// Make and join the thread
	std::thread ServerThread(&Server::LoopServer, this);
	ServerThread.join();
}

void Server::LoopServer() {

	while (running)
	{
		fd_set copy = master;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		
		try
		{
			// Looping all socket inside master
			for (int i = 0; i < socketCount; i++)
			{
				fd_set copy = master;
				int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
				SOCKET sock = copy.fd_array[i];

				// If the socket is listening socket
				if (sock == listening)
				{
					// Accept a new conneciton
					SOCKET client = accept(listening, nullptr, nullptr);

					// Add the new connection to the list of connected clients
					FD_SET(client, &master);

					// Send a welcome message to the connted client
					std::string welcomeMsg = "Welcome to the Chat Server!\r\n";
					send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);

					// broadcast welcome message
					std::ostringstream ss;
					ss << "Client connect to chat server.\r\n";
					std::string strOut = ss.str();
					for (int i = 0; i < master.fd_count; i++)
					{
						SOCKET outSock = master.fd_array[i];
						if (outSock != listening && outSock != client)
						{
							send(outSock, strOut.c_str(), strOut.size() + 1, 0);
						}
					}
					// logging welcome message to server
					std::cout << strOut << std::endl;
				}
				else
				{
					// Recieve data from client and send it to others

					// To save the data
					char buf[4096];
					// Fill all with zero
					ZeroMemory(buf, 4096);

					// Receive Message
					int bytesIn = recv(sock, buf, 4096, 0);
					if (bytesIn <= 0)
					{
						// Drop the client
						closesocket(sock);
						FD_CLR(sock, &master);
					}
					else
					{
						// Check to see if it`s a command.  \quit kills the server
						if (buf[0] == '\\')
						{
							std::string cmd = std::string(buf, bytesIn);
							if (cmd == "\\quit")
							{
								running = false;
								break;
							}
						}

						// Send message to other client, and definiately not the listening socket
						std::ostringstream ss;
						ss << buf << "\r\n";
						std::string strOut = ss.str();
						for (int i = 0; i < master.fd_count; i++)
						{
							SOCKET outSock = master.fd_array[i];
							if (outSock != listening && outSock != sock)
							{
								send(outSock, strOut.c_str(), strOut.size() + 1, 0);
							}
						}
						// logging user`s message to server
						std::cout << strOut << std::endl;
					}
				}
			}
		}
		catch (...)
		{
			ClosingServer();
			break;
		}
	}
}

void Server::ClosingServer() {
	// Close listening socket
	FD_CLR(listening, &master);
	closesocket(listening);

	// broadcast closing messasge and close clinet socket
	std::string msg = "Server is shutting down. Goodbye\r\n";
	while (master.fd_count > 0)
	{
		SOCKET sock = master.fd_array[0];

		send(sock, msg.c_str(), msg.size() + 1, 0);

		FD_CLR(sock, &master);
		closesocket(sock);
	}

	// Cleanup Winsock
	WSACleanup();
}