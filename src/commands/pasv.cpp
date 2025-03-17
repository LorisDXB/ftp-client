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

#include "commands.hpp"
#include "../errors.hpp"
#include "../../include/config.hpp"
#include "../utility.hpp"
#include <algorithm>
#include <string>

// Binds data socket and retrieves its information with getsockname.
bool bindDataSocket(int &dataFd, sockaddr_in &dataAddr)
{
	if (bind(dataFd, (sockaddr *)&dataAddr, sizeof(dataAddr)) == 0) {
		socklen_t addrLen = sizeof(dataAddr);
		if (getsockname(dataFd, (sockaddr*)&dataAddr, &addrLen) == 0) {
			return true;
		}
	}
	close(dataFd);
	throw ftp::BindError("bindSocketToRandomPort: Failed to retrieve assigned port.");
}

// Binds the `dataFd` socket to a random port number using its helper function:
// `bindDataSocket`.
bool bindSocketToRandomPort(int &dataFd, sockaddr_in &dataAddr)
{
	dataFd = socket(AF_INET, SOCK_STREAM, 0);
	if (dataFd == -1) {
		throw ftp::SocketCreationError("bindSocketToRandomPort: Failed to create socket.");
	}

	for (int i = 0; i < ftp::cfg::RTRYPASV; i++) {
		int port = generateRandomPort();

		dataAddr.sin_family = AF_INET;
		dataAddr.sin_port = htons(port);
		dataAddr.sin_addr.s_addr = INADDR_ANY;

		if (bindDataSocket(dataFd, dataAddr)) {
			return true;
		}
	}
	close(dataFd);
	return false;
}

// Opens a data socket and listens it, sends back the information to connect to it
// to the client.
void ftp::handlePASV(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
	int dataFd;
	sockaddr_in dataAddr;

	if (!bindSocketToRandomPort(dataFd, dataAddr)) {
		throw ftp::BindError("handlePASV: Failed to bind random port.");
	}

	if (listen(dataFd, 0) == -1) {
		throw ftp::ListenError("handlePASV: failed to listen.");
	}

	std::string serverIp = client->getServerIp();
	std::replace(serverIp.begin(), serverIp.end(), '.', ',');

	int port = ntohs(dataAddr.sin_port);
	int p1 = port / 256;
    int p2 = port % 256;

	client->setDataFd(dataFd);
	client->setTransferType(PASSIVE);
	std::string response = std::string(ftp::responses::PASV) + " (" +
						   serverIp + "," +
	                       std::to_string(p1) + "," + std::to_string(p2) + ").\r\n";
	write(client->getFd(), response.c_str(), response.size());
}