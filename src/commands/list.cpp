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

void ftp::execLIST(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args, int fdToUse)
{
    std::string dirToRead = args.size() > 1 ? resolvePath(client, args[1])
        : client->getCurrDir();
    std::string cmd("ls -l " + dirToRead);
    FILE *pipe = popen(cmd.c_str(), "r"); 

    if (!pipe) {
        write(client->getFd(), ftp::responses::NOFILE_FOUND, strlen(ftp::responses::NOFILE_FOUND));
        return;    
    }

    write(client->getFd(), ftp::responses::LIST_SEND,
        strlen(ftp::responses::LIST_SEND));

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        write(fdToUse, buffer, strlen(buffer));
    }
    write(client->getFd(), ftp::responses::DATACONN_CLOSE,
        strlen(ftp::responses::DATACONN_CLOSE));
}

void ftp::handleLIST(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
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
	        if (connFd < 0) _exit(0);

            execLIST(client, args, connFd);
            close(connFd);
        } else {
            execLIST(client, args, client->getDataFd());
        }
        _exit(0);
    }
    client->closeDataFd();
    client->setTransferType(NONE);
}
