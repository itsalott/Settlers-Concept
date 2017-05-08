#include "stdafx.h"
#include "Server.h"

// TODO: the whole world should be a proxy
static std::vector<ProxySettler> proxySettlers;

Server::Server() {
	proxySettlers = std::vector<ProxySettler>();
}

std::vector<ProxySettler> Server::receiveProxyObjectsFromServer() {
	return proxySettlers;
}

void Server::sendProxyObjectsToServer(std::vector<ProxySettler> newProxySettlers) {
	std::cout << "\nServer received " << newProxySettlers.size() << " proxy objects..." << std::endl;

	/*for (int i = 0; i < newProxySettlers.size(); ++i) {
		std::cout << "Object " << i << " has pos: " << newProxySettlers[i].pos.x << ", " << newProxySettlers[i].pos.z << std::endl;
	}*/

	std::cout << "Message size in bytes: " << sizeof(std::vector<ProxySettler>) + (sizeof(ProxySettler) * newProxySettlers.size()) << std::endl;

	// TODO: instead of clearing, implement an update to find changes
	proxySettlers.clear();
	proxySettlers = newProxySettlers;
}