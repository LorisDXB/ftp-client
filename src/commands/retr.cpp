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

#include <memory>
#include "../client.hpp"
#include "commands.hpp"
#include <fstream>
#include "../../include/config.hpp"
#include <string>
#include <string.h>
#include "../utility.hpp"

// Accepts a connection to the data socket of the `client`,
// sends an error code if unable to.
int ftp::acceptDataConnection(std::shared_ptr<ftp::Client> &client)
{
    sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    int connFd = accept(client->getDataFd(), (sockaddr*)&clientAddr, &addrLen);
    
    if (connFd == -1) {
        write(client->getFd(), ftp::responses::DATACONN_FAIL, 34);
		return -1;
    }
	return connFd;
}

void ftp::execRETR(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
    std::string filename = args[1];
    std::ifstream file(resolvePath(client, filename), std::ios::binary);

    if (!file.is_open()) {
        write(client->getFd(), ftp::responses::NOFILE_FOUND, 21);
        return;
    }

    write(client->getFd(), ftp::responses::DATACONN_OPEN, 30);

    if (client->getTransferType() == PASSIVE) {
	    int connFd = acceptDataConnection(client);
	    if (connFd < 0) _exit(0);

        char buffer[4096];
        file.read(buffer, sizeof(buffer));
        write(connFd, buffer, file.gcount());
        close(connFd);
    } else {
        char buffer[4096];
        file.read(buffer, sizeof(buffer));
        write(client->getDataFd(), buffer, file.gcount());
    }

    write(client->getFd(), ftp::responses::TRANSFER_COMPLETE, 24);
}

// Handles file retrieval while following RFC959, opens the file given, reads it,
// writes it to data socket.
void ftp::handleRETR(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
    if (client->getTransferType() == NONE) {
        write(client->getFd(), ftp::responses::DATACONN_FAIL,
            strlen(ftp::responses::DATACONN_FAIL));
        return;
    }
    if (args.size() < 2) {
        write(client->getFd(), ftp::responses::NOFILE_SPEC,
            strlen(ftp::responses::NOFILE_SPEC));
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        write(client->getFd(), ftp::responses::FAIL,
            strlen(ftp::responses::FAIL));
        return;
    }

    if (pid == 0) {
        execRETR(client, args);
        _exit(0);
    }
    client->closeDataFd();
    client->setTransferType(NONE);
}
