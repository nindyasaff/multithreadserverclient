#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#include "Client.h"
#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main() {
	// Make client class
	Client client;
	// Start client (thread)
	client.Begin();

	return 0;
}