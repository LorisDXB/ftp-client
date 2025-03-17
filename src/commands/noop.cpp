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

void ftp::handleNOOP(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
    write(client->getFd(), ftp::responses::NOOP,
        strlen(ftp::responses::NOOP));
}