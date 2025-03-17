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
#include <string.h>

void ftp::handlePORT(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
	if (args.size() < 2) {
		write(client->getFd(), ftp::responses::INVALID_ARGS,
			strlen(ftp::responses::INVALID_ARGS));
		return;
	}

	std::vector<std::string> connInfo = splitStringDelim(args[1], ',');
	if (connInfo.size() < 6) {
		write(client->getFd(), ftp::responses::INVALID_ARGS,
			strlen(ftp::responses::INVALID_ARGS));
		return;
	}

	std::string socketIp = connInfo[0] + "." + connInfo[1] + "." + connInfo[2] + "." + connInfo[3];
	int socketPort = std::stoi(connInfo[4]) * 256 + std::stoi(connInfo[5]);
	int connSocket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in conAddr;
	conAddr.sin_addr.s_addr = inet_addr(socketIp.c_str());
	conAddr.sin_family = AF_INET;
	conAddr.sin_port = htons(socketPort);

	if (connect(connSocket, (sockaddr *)&conAddr, sizeof(conAddr)) == -1) {
		write(client->getFd(), ftp::responses::DATACONN_FAIL,
			strlen(ftp::responses::DATACONN_FAIL));
		return;
	}
	client->setDataFd(connSocket);
	client->setTransferType(ACTIVE);
	write(client->getFd(), ftp::responses::PORT_SUCC,
		strlen(ftp::responses::PORT_SUCC));
}