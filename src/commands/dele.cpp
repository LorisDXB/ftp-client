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

// Handles the deletion of a file from the FTP server at the client's request.
// The function first checks if the file exists and if it's an actual file (not a directory).
// Then, it attempts to remove it.
void ftp::handleDELE(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args)
{
    if (args.size() < 2) {
        write(client->getFd(), ftp::responses::INVALID_ARGS,
            strlen(ftp::responses::INVALID_ARGS));
        return;
    }

    std::string filePath = resolvePath(client, args[1]);

    if (!std::filesystem::exists(filePath)) {
        write(client->getFd(), ftp::responses::NOFILE_FOUND,
            strlen(ftp::responses::NOFILE_FOUND));
        return;
    }

    if (!std::filesystem::is_directory(filePath) && std::filesystem::remove(filePath)) {
        write(client->getFd(), ftp::responses::DELE_COMPLETE,
            strlen(ftp::responses::DELE_COMPLETE));
    } else {
        write(client->getFd(), ftp::responses::DELE_FAIL,
            strlen(ftp::responses::DELE_FAIL));
    }
}