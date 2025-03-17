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

#include "client.hpp"
#include <unordered_map>
#include "server.hpp"
#include <functional>


ftp::Client::Client(int clientFd, sockaddr_in &clientAddr,
	LimitedServerInfo serverInfo)
	: _clientFd(clientFd), _clientAddr(clientAddr), _serverInfo(serverInfo) {}

ftp::Client::~Client()
{
	closeDataFd();
	closeFd();
}

int ftp::Client::getFd()
{
	return _clientFd;
}

void ftp::Client::setDataFd(int dataFd)
{
	_dataFd = dataFd;
}

int ftp::Client::getDataFd()
{
	return _dataFd;
}

bool ftp::Client::isConnected()
{
	return _isConnected;
}

void ftp::Client::setUsername(std::string username)
{
	_username = username;
}

void ftp::Client::setIsConnected(bool value)
{
	_isConnected = value;
}

std::unordered_map<std::string, ftp::accountInfo> &ftp::Client::getAccounts()
{
	return _serverInfo.accounts;
}

void ftp::Client::clearUsername()
{
	_username.clear();
}

std::string &ftp::Client::getServerIp()
{
	return _serverInfo.serverIp;
}

std::string ftp::Client::getUsername()
{
	return _username;
}

std::string &ftp::Client::getCurrDir()
{
	return _currDir;
}

std::string ftp::Client::getHomeDir()
{
	const auto &it = _serverInfo.accounts.find(_username);

	if (it != _serverInfo.accounts.end()) {
		return it->second.homeDir;
	}
	return std::string("");
}

ftp::TransferType ftp::Client::getTransferType()
{
	return _transferType;
}

void ftp::Client::setTransferType(ftp::TransferType value)
{
	_transferType = value;
}

void ftp::Client::closeFd()
{
	int fdToClose = getFd();

	if (fdToClose > 0) {
		close(fdToClose);
		setFd(-1);
	}
}

void ftp::Client::closeDataFd()
{
	int fdToClose = getDataFd();

	if (fdToClose > 0) {
		close(fdToClose);
		setDataFd(-1);
	}
}

void ftp::Client::serverSideCleanUp()
{
	_serverInfo.cleanSelf();
}

void ftp::Client::setFd(int clientFd)
{
	_clientFd = clientFd;	
}

void ftp::Client::setCurrDir(std::string newDir)
{
	_currDir = newDir;
}