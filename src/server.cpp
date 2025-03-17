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

 #include <sys/socket.h>
 #include <vector>
 #include <netinet/in.h>
 #include <arpa/inet.h>
 #include <unistd.h>
 #include <iostream>
 #include <poll.h>
 #include "server.hpp"
 #include "errors.hpp"
 #include "utility.hpp"
 #include "../include/config.hpp"
 #include "commands/commands.hpp"
 #include <string.h>
 #include <memory>
 
 // Closes all fd's in the poll vector `pfds`
 void closePollVector(std::vector<pollfd> &pfds)
 {
	 for (pollfd &pfd : pfds) {
		 int fd = pfd.fd;
		 if (fd > 0) {
			 close(fd);
		 }
	 }
 }
 
 // Allows graceful server stops/starts
 void ftp::Server::start()
 {
	std::cout << "Server started." << std::endl;
	 _running = true;
 }
 
 void ftp::Server::stop()
 {
	std::cout << "Server stopped." << std::endl;
	 _running = false;
	 closePollVector(_pfds);
	 _pfds.clear();
 }
 
 // Sets up the socket `serverFd`
 void setupSocket(int &serverFd)
 {
	int opt = 1;
	serverFd = socket(AF_INET, SOCK_STREAM, 0);

	setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	if (serverFd == -1) {
	 throw ftp::SocketCreationError("setupSocket: Failed to create socket");
	}
 }
 
 // Sets up the address for the `serverAddr` with port `port`
 void setupAddr(sockaddr_in &serverAddr, const std::string &port)
 {
	 serverAddr.sin_family = AF_INET;
	 serverAddr.sin_port = htons(stouint16(port));
	 serverAddr.sin_addr.s_addr = INADDR_ANY;
 }
 
 // Binds `serverAddr` socket to `serverFd`
 void bindAddrToSocket(sockaddr_in &serverAddr, int &serverFd)
 {
	socklen_t lenServer = sizeof(serverAddr);

	if (bind(serverFd, (sockaddr *)&serverAddr, lenServer) == -1) {
		throw ftp::BindError("bindAddrToSocket: Failed to bind socket");
	}
 }
 
 // Puts server in passive mode, enabling it to listen for incoming connections
 void listenForConnections(int &serverFd)
 {
	 if (listen(serverFd, ftp::cfg::MAXQUEUESIZE) == -1) {
		 throw ftp::ListenError("listenForConnections: Failed to listen");
	 }
 }
 
 // Sets up the poll vector `pfds`
 void setupPolls(std::vector<pollfd> &pfds, int &serverFd)
 {
	 pollfd serverPfd;
	 serverPfd.fd = serverFd;
	 serverPfd.events = POLLIN;
	 pfds.push_back(serverPfd);
 }
 
 // Initializes the server
 ftp::Server::Server(const std::string &port, const std::string &anonymousDir)
 : _serverIp("127.0.0.1")
 {
	 setupSocket(_serverFd);
	 setupAddr(_serverAddr, port);
	 bindAddrToSocket(_serverAddr, _serverFd);
	 listenForConnections(_serverFd);
	 setupPolls(_pfds, _serverFd);
	 _accounts["Anonymous"] = {"", anonymousDir};
 }
 
 // Closes the server
 ftp::Server::~Server()
 {
	 close(_serverFd);
	 closePollVector(_pfds);
 }
 
 // Monitors connection activity
 int ftp::Server::monitorConnections()
 {
	 int eventCount = poll(_pfds.data(), _pfds.size(), ftp::cfg::POLLTIMEOUT);
 
	 if (eventCount == -1) {
		 throw ftp::PollError("monitorConnections: Failed polling pfds.");
	 }
 
	 return eventCount;
 }
 
 // Appends a new client to `pfds`
 void appendNewClient(std::vector<pollfd> &pfds, int &clientFd)
 {
	 pollfd clientPfd;
	 clientPfd.fd = clientFd;
	 clientPfd.events = POLLIN;
	 pfds.push_back(clientPfd);
 }
 
 ftp::LimitedServerInfo ftp::Server::encapsulateServerInfo(int clientFd)
 {
	 ftp::LimitedServerInfo serverInfo = {
			 [this, clientFd]() { this->cleanClient(clientFd); },
			 _serverIp,                           
			 _accounts                            
	 };
 
	 return serverInfo;		
 }
 
 // Handles new client connections
 void ftp::Server::handleNewConnections()
 {
	 if (_pfds[0].revents & POLLIN)
	 {
		 sockaddr_in clientAddr;
		 socklen_t clientLen = sizeof(clientAddr);
		 int clientFd = accept(_serverFd, (sockaddr *)&clientAddr, &clientLen);
		 if (clientFd <= 0) return;
 
		 appendNewClient(_pfds, clientFd);
		 ftp::LimitedServerInfo serverInfo = encapsulateServerInfo(clientFd);
 
		 _clients.emplace(clientFd, std::make_shared<Client>(
			 clientFd, clientAddr, serverInfo));
		 write(clientFd, ftp::responses::WELCOME, strlen(ftp::responses::WELCOME));
		 std::cout << "handleNewConnections: Accepted new client FD: " << clientFd << std::endl;
	 }
 }
 
 // Removes a client
 void ftp::Server::removeClient(int clientFd)
 {
	 if (clientFd > 0) {
		 close(clientFd);
	 }
	 _clients.erase(clientFd);
 }
 
 void ftp::Server::cleanClient(int clientFd)
 {
	if (clientFd == 0) {
		std::cerr << "cleanClient: Unable to clear server." << std::endl;
	}
 
	removeClient(clientFd);
	for (size_t i = 1; i < _pfds.size(); i++) {
		if (_pfds[i].fd == clientFd) {
	 		_pfds.erase(_pfds.begin() + i);
	 		std::cout << "cleanClient: Cleaning up client FD: ";
			std::cout << clientFd << std::endl;
		}
	}
 }
 
 // Processes client messages and marks clients that need to be deleted.
 void ftp::Server::processClientActivity(std::vector<size_t> &clientsToRemove, size_t &i)
 {
	 char buffer[1024];
	 ssize_t bytesRead = read(_pfds[i].fd, buffer, sizeof(buffer));
 
	 if (bytesRead <= 0) {
		 clientsToRemove.push_back(_pfds[i].fd);
	 } else {
		 std::cout << "processClientActivity: Processing from FD: " << _pfds[i].fd;
		 std::cout << " Command: " << std::string(buffer, bytesRead) << std::endl;
		 processCommand(_clients[_pfds[i].fd], std::string(buffer, bytesRead));
	 }
 }
 
 // Handles client activity
 void ftp::Server::handleClientActivity()
 {
	 std::vector<size_t> clientsToRemove;
 
	 for (size_t i = _pfds.size(); i-- > 1;) {
		 if (_pfds[i].revents & POLLIN) {
			 processClientActivity(clientsToRemove, i);
		 }
	 }
	 for (int fd: clientsToRemove) {
		 cleanClient(fd);
	 }
 }
 
 // Main server loop
 void ftp::Server::run()
 {
	 while (_running) {
		 int eventCount = monitorConnections();
		 if (eventCount > 0) {
			 handleNewConnections();
			 handleClientActivity();
		 }
	 }
 }
 
 // Returns a reference to `accounts`
 std::unordered_map<std::string, ftp::accountInfo> &ftp::Server::getAccounts()
 {
	 return _accounts;
 }