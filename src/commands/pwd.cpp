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

void ftp::handlePWD(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
    std::string pwd = client->getCurrDir();
    std::string response = ftp::responses::PWD_START + pwd + ftp::responses::PWD_END;

    write(client->getFd(), response.c_str(), response.size());
}