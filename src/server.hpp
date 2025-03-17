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

 #ifndef SERVER_HPP
 #define SERVER_HPP
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
 #include "client.hpp"
 
 namespace ftp {
	 struct accountInfo {
		 std::string passwd;
		 std::string homeDir;
	 };
 
	 struct LimitedServerInfo;
	 
 
	 class Server
	 {
		 private:
			 int _serverFd;
			 sockaddr_in _serverAddr;
			 std::vector <pollfd> _pfds;
			 int _running;
 
			 std::unordered_map<int, std::shared_ptr<Client>> _clients;
 
			 std::unordered_map<std::string, accountInfo> _accounts;
 
			 std::string _serverIp;
		 public:
			 explicit Server(const std::string &port, const std::string &anonymousDir);
			 ~Server();	
 
			 void run();
 
			 void start();
			 void stop();
 
			 int monitorConnections();
 
			 void handleNewConnections();
			 void handleClientActivity();
 
			 void removeClient(int clientFd);
			 void processClientActivity(std::vector<size_t> &clientsToRemove, size_t &i);
 
			 std::unordered_map<std::string, accountInfo> &getAccounts();
 
			 void cleanClient(int i);
 
 			 ftp::LimitedServerInfo encapsulateServerInfo(int clientFd);
	 };
 };
 
 #endif // SERVER_HPP