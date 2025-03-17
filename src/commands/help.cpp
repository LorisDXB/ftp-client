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

// Self-explanatory
void ftp::handleHELP(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
    std::string helpMessage =
        "214-The following commands are recognized:\r\n"
        " USER   PASS   CWD    CDUP   QUIT   DELE   PWD\r\n"
        " PASV   PORT   HELP   NOOP   RETR   STOR   LIST\r\n"
        "214 Help command successful.\r\n";

    write(client->getFd(), helpMessage.c_str(), helpMessage.length());
}