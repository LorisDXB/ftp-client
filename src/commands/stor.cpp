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
#include "../server.hpp"
#include "../utility.hpp"
#include <filesystem>

std::fstream openFile(const std::string& filename) {
    std::fstream file;
    
    file.open(filename, std::ios::out | std::ios::binary);
    
    if (!file.is_open()) {
        file.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("openFile: Failed to create file");
        }
    }
    return file;
}

void execSTOR(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args, int fdToUse)
{
    std::string fileName = resolvePath(client, args[1]);
    std::fstream file = openFile(fileName);
    
    if (!file.is_open()) {
        write(client->getFd(), ftp::responses::NOFILE_FOUND,
        strlen(ftp::responses::NOFILE_FOUND));
        return;
    }

    write(client->getFd(), ftp::responses::DATACONN_OPEN,
        strlen(ftp::responses::DATACONN_OPEN));
    
    char buffer[4096];
    int bytesRead = 0;

    while (bytesRead < 1) {
        bytesRead = read(fdToUse, buffer, sizeof(buffer));
    }

    file.write(buffer, bytesRead);
    file.close();
    write(client->getFd(), ftp::responses::TRANSFER_COMPLETE,
        strlen(ftp::responses::TRANSFER_COMPLETE));
}

void ftp::handleSTOR(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
	if (args.size() < 2) {
		write(client->getFd(), ftp::responses::INVALID_ARGS,
			strlen(ftp::responses::INVALID_ARGS));
		return;
	}

    if (client->getTransferType() == NONE) {
        write(client->getFd(), ftp::responses::DATACONN_FAIL,
            strlen(ftp::responses::DATACONN_FAIL));
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        write(client->getFd(), ftp::responses::FAIL,
            strlen(ftp::responses::FAIL));
        return;
    }

    if (pid == 0) {
        if (client->getTransferType() == PASSIVE) {
	        int connFd = acceptDataConnection(client);
	        if (connFd < 0) {
                write(client->getFd(), ftp::responses::DATACONN_FAIL,
                    strlen(ftp::responses::DATACONN_FAIL));
                _exit(1);
            }

            execSTOR(client, args, connFd);
            close(connFd);
        } else {
            execSTOR(client, args, client->getDataFd());
        }
        _exit(0);
    }
    
    client->closeDataFd();
    client->setTransferType(NONE);
}
