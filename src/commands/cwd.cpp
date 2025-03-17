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

// Writes a message to the client's closet before clearing it, it'll be cleared from the poll vector
// Afterwards due to it not sending any signals anymore. (this is handled in server.cpp)
void ftp::handleCWD(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
    if (args.size() < 2) {
        write(client->getFd(), ftp::responses::BADCMD,
            strlen(ftp::responses::BADCMD));
        return;
    }
    std::string resolvedPath = resolvePath(client, args[1]);
    if (resolvedPath.empty()) {
        write(client->getFd(), ftp::responses::CWD_PERM,
            strlen(ftp::responses::CWD_PERM));
        return;
    }

    std::ifstream newDir(resolvedPath, std::ios::binary);

    if (!newDir.is_open()) {
        write(client->getFd(), ftp::responses::CWD_NOTFOUND,
            strlen(ftp::responses::CWD_NOTFOUND));
        return;
    }

    client->setCurrDir(resolvedPath);
    write(client->getFd(), ftp::responses::CWD_SUCCESS,
        strlen(ftp::responses::CWD_SUCCESS));
}