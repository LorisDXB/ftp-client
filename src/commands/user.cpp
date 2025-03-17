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
bool isUsernameCorrect(std::shared_ptr<ftp::Client> &client, std::string username)
{
    std::unordered_map<std::string, ftp::accountInfo> &accounts =
        client->getAccounts();

    const auto &it = accounts.find(username);

    return (it != accounts.end());
}

// Handles file retrieval while following RFC959, opens the file given, reads it,
// writes it to data socket.
void ftp::handleUSER(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
    std::string username;

    if (args.size() < 2) {
        write(client->getFd(), ftp::responses::USER_KO,
            strlen(ftp::responses::USER_KO));
        return;
    }

    if (!isUsernameCorrect(client, args[1])) {
        username = "Anonymous";
    } else {
        username = args[1];
    }

    client->setUsername(username);
    write(client->getFd(), ftp::responses::USER_OK,
        strlen(ftp::responses::USER_OK));
}