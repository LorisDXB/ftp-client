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

// Handles the CDUP (Change to Parent Directory) command  
// Attempts to move the client to the parent directory  
void ftp::handleCDUP(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
    std::string resolvedPath = resolvePath(client, "..");
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