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

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <sys/socket.h>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <poll.h>
#include <thread>
#include <memory>
#include <unordered_map>
#include <functional>

namespace ftp {

	struct accountInfo;

	struct LimitedServerInfo {
		std::function<void()> cleanSelf;
		std::string serverIp;
		std::unordered_map<std::string, accountInfo> accounts;
	};

	enum TransferType {
		ACTIVE,
		PASSIVE,
		NONE
	};

	class Client
	{
		private:
			int _clientFd;
			int _dataFd;

			TransferType _transferType = NONE;
			LimitedServerInfo _serverInfo;

			sockaddr_in _clientAddr;
			bool _isConnected = false;
			std::string _username;
			std::string _currDir;
		public:
			explicit Client(int clientFd, sockaddr_in &clientAddr,
				LimitedServerInfo serverInfo);
			~Client();

			int getFd();
			void setFd(int dataFd);

			int getDataFd();
			std::string getUsername();

			void setDataFd(int dataFd);
			bool isConnected();

			void setUsername(std::string username);
			void setIsConnected(bool value);

			void clearUsername();

			std::unordered_map<std::string, accountInfo> &getAccounts();
			std::string &getServerIp();

			std::string getHomeDir();
			std::string &getCurrDir();

			void setCurrDir(std::string newDir);

			TransferType getTransferType();
			void setTransferType(TransferType value);

			void closeFd();
			void closeDataFd();
			void serverSideCleanUp();
	};
};

#endif // CLIENT_HPP