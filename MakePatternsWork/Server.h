#ifndef SERVER_H
#define SERVER_H
#include <vector>
#include "WorldObjects.h"

class Server {
public:
	Server::Server();
	~Server(){}

	static std::vector<ProxySettler> receiveProxyObjectsFromServer();
	static void sendProxyObjectsToServer(std::vector<ProxySettler> newProxySettlers);

private:	
};

#endif
