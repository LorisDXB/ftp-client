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

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <unordered_map>
#include "../client.hpp"
#include <vector>
#include <memory>

namespace ftp
{
	void execRETR(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args);
	void handleRETR(std::shared_ptr<Client> &client, std::vector<std::string> &args);

	void handlePASV(std::shared_ptr<Client> &client, std::vector<std::string> &args);
	void handleUSER(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args);
	void handlePASS(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args);
	void handlePORT(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args);
	void handleQUIT(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args);
	void handleCWD(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args);
	void handleCDUP(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args);
	void handleDELE(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args);
	void handleSTOR(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args);
	void handleHELP(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args);
	void handleNOOP(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args);

	void handleLIST(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args);
	void execLIST(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args, int fdToUse);

	void handlePWD(std::shared_ptr<ftp::Client> &client, std::vector<std::string> &args);

	void processCommand(std::shared_ptr<Client> &client, const std::string &args);
	int acceptDataConnection(std::shared_ptr<ftp::Client> &client);
};

#endif // COMMAND_HPP