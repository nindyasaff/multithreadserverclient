#include "Client.h"

Client::Client() {
	// Set Ip, port, and version
	ipAddress = "127.0.0.1";
	port = 54000;
	ver = MAKEWORD(2, 2);


	// Start the winshock
	wsResult = WSAStartup(ver, &data);
	// Check is winshok created
	if (wsResult != 0)
	{
		std::cout << "Can`t start Winsock, Err #" << wsResult << std::endl;
	}

	// Make the socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	// Check is socket created
	if (sock == INVALID_SOCKET)
	{
		std::cout << "Can`t create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
	}

	// Bind the socket to an ip address, and port to a socket
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Enter a username
	std::cout << "Enter a username: "; std::cin >> username;
	std::cin.ignore(1, '\n');

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cout << "Can`t connect to server, Err #" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
	}
}

void Client::Begin() {

	// Recieve thread
	std::thread recv_thread(&Client::recv_socket, this, sock);
	// Send thread
	std::thread send_thread(&Client::SentData, this);
	

	// Close down everything if end
	recv_thread.join();
	send_thread.join();
	closesocket(sock);
	WSACleanup();
}

void Client::SentData() {
	// Do-while loop to send data
	std::string userInput;
	do
	{
		try
		{
			
			// Prompt the user for some text
			getline(std::cin, userInput);
			
			if (userInput.size() > 0) // Make sure the user has typed in something
			{
				// Send the text
				std::string msgToSend = username + " : " + userInput;
				send(sock, msgToSend.c_str(), msgToSend.size() + 1, 0);
			}
		}
		catch (int e)
		{
			std::cout << "Server disconnected\n";
			break;
		}
	} while (userInput.size() > 0);
}

void Client::recv_socket(SOCKET sock)
{
	// For take the data
	char buf[4096];
	while (true)
	{
		try
		{
			//Checking connection to server
			int r = recv(sock, NULL, 0, 0);
			if (!r)
			{
				// Fill this data with zero
				ZeroMemory(buf, 4096);
				// Recieve data from server
				int bytesReceived = recv(sock, buf, 4096, 0);
				// Check id data contain something
				if (bytesReceived > 0)
				{
					std::cout << "SERVER> " << std::string(buf, 0, bytesReceived) << std::endl;
				}
			}
			else
			{
				throw (r);
			}
		}
		catch (int e)
		{
			std::cout << "Server disconnected\n";
			break;
		}
	}
}