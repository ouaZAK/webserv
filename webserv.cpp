#include "webserv.hpp"

webserv::webserv(int domain, int service, int protocol, int port, size_t interface)
{
	sock = socket(domain, service, protocol);
	address.sin_family = domain;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = interface;

	connection = connectToNetwork(sock, address);
}

webserv::~webserv()
{

}

