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


#include "commands.hpp"
#include "../client.hpp"
#include <string>
#include <functional>
#include "../utility.hpp"
#include "../../include/config.hpp"
#include "../errors.hpp"
#include <algorithm>
#include <string.h>

// Check if `client` is allowed to execute `cmdName`,
// e.g `client` can't use RETR if he isn't connected.
bool isAllowedToExecute(std::shared_ptr<ftp::Client> &client, std::string cmdName)
{
	static std::vector<std::string> offlineFunctions {
		"USER",
		"PASS",
		"QUIT"
	};

	const auto &it = std::find(offlineFunctions.begin(),
		offlineFunctions.end(), cmdName);

	if (it == offlineFunctions.end() && !client->isConnected()) {
		return false;
	}
	return true;
}

// Takes a string of arguments sent by the client, split it and verify if it's
// a command call, if it is then execute the command with the given args.
void ftp::processCommand(std::shared_ptr<ftp::Client> &client, const std::string &args)
{
	static std::unordered_map<std::string,
		std::function<void(std::shared_ptr<ftp::Client> &,
		std::vector<std::string> &)>> commandMap
	{
		{"RETR", ftp::handleRETR},
		{"PASV", ftp::handlePASV},
		{"USER", ftp::handleUSER},
		{"PASS", ftp::handlePASS},
		{"PORT", ftp::handlePORT},
		{"QUIT", ftp::handleQUIT},
		{"CWD", ftp::handleCWD},
		{"LIST", ftp::handleLIST},
		{"PWD", ftp::handlePWD},
		{"DELE", ftp::handleDELE},
		{"STOR", ftp::handleSTOR},
		{"CDUP", ftp::handleCDUP},
		{"HELP", ftp::handleHELP},
		{"NOOP", ftp::handleNOOP}
	};

	std::vector<std::string> splitArgs = splitString(args);

	if (splitArgs.size() == 0) {
		std::cerr << "processCommand: Not enough arguments." << std::endl;
		write(client->getFd(), ftp::responses::BADCMD, strlen(ftp::responses::BADCMD));
		return;
	}

	const auto &it = commandMap.find(splitArgs[0]);

	if (it == commandMap.end()) {
		write(client->getFd(), ftp::responses::BADCMD, strlen(ftp::responses::BADCMD));
		return;
	}

	if (isAllowedToExecute(client, splitArgs[0])) {
		it->second(client, splitArgs);
	} else {
		write(client->getFd(), ftp::responses::NOT_LOGGED_IN,
		strlen(ftp::responses::NOT_LOGGED_IN));
	}
}