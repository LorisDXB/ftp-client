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

// Writes a message to the client's closet before clearing it, it'll be cleared from the poll vector
// Afterwards due to it not sending any signals anymore. (this is handled in server.cpp)
void ftp::handleQUIT(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
    write(client->getFd(), ftp::responses::GOODBYE,
        strlen(ftp::responses::GOODBYE));
    client->closeDataFd();
    client->closeFd();
    client->serverSideCleanUp();
}