/*
* < Kawabonga ! >
* ------------------------------
*    \   ^__^
*     \  (oo)\_______
*        (__)\       )\/\
*            ||----w |
*            ||     ||
* 
*/

#include "src/server.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <fstream>

int robustness(int argc, char *argv[])
{
	std::ifstream file(argv[2]);

	if (!file.is_open()) {
		return 84;
	}
	if (argc != 3) {
		return 84;
	}

	return 0;
}

int main(int argc, char *argv[])
{
	if (robustness(argc, argv) == 84) {
		return 84;
	}

	try {
		ftp::Server serverObj(argv[1], argv[2]);

		serverObj.start();
		serverObj.run();
		serverObj.stop();
	} catch (const std::exception &e) {
		std::cerr << "Caught an exception: " << e.what() << std::endl;
		return 84;
	}
	return 0;
}