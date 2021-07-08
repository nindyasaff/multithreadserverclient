#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include "Server.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main() {
	// Make the server
	Server serverTest;
	// Start the server
	serverTest.ServerBegin();

	return 0;
}