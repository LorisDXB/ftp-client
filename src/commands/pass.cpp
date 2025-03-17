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

// Checks if `username` is a correct username.
bool isPasswdCorrect(std::shared_ptr<ftp::Client> &client, std::string passwd)
{
    std::unordered_map<std::string, ftp::accountInfo> &accounts =
        client->getAccounts();

    const auto &it = accounts.find(client->getUsername());

    return (it->second.passwd == passwd);
}

// Handles file retrieval while following RFC959, opens the file given, reads it,
// writes it to data socket.
void ftp::handlePASS(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
    if (client->getUsername().empty()) {
        write(client->getFd(), ftp::responses::PASS_BFR_USER,
            strlen(ftp::responses::PASS_BFR_USER));
        return;
    }
    if (args.size() < 2) {
        args.push_back(std::string(""));
    }
    if (!isPasswdCorrect(client, args[1])) {
        client->clearUsername();
        write(client->getFd(), ftp::responses::LOGIN_FAIL,
            strlen(ftp::responses::LOGIN_FAIL));
        return;
    }

    client->setIsConnected(true);
    client->setCurrDir(client->getHomeDir());
    write(client->getFd(), ftp::responses::LOGIN_SUCCESS,
        strlen(ftp::responses::LOGIN_SUCCESS));
}