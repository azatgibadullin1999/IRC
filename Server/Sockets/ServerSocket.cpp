#include "ServerSocket.hpp"

ServerSocket::ServerSocket(const std::string &port)
	throw(SocketException) : SocketBase("", port) {
	std::cout << "Connecting" << std::endl;
	connect();
	std::cout << "Listening" << std::endl;
	startListening();
}

ServerSocket::ServerSocket(const std::string &domain, const std::string &port)
	throw(SocketException) : SocketBase(domain, port) {
	std::cout << "Connecting" << std::endl;
	connect();
	std::cout << "Listening" << std::endl;
	startListening();
}

ServerSocket::~ServerSocket() { }

void	ServerSocket::connect() {
	struct addrinfo *address = getAddress();
	if (bind(getSocket(), address->ai_addr, address->ai_addrlen)) {
		std::cout << "" << std::endl;
		throw SocketException(strerror(errno));
	}
}

void	ServerSocket::startListening() {
	if (listen(getSocket(), SOMAXCONN))
		throw SocketException("Listen");
}

int		ServerSocket::accept() {
	int newSocket = ::accept(getSocket(), NULL, NULL);
	if (newSocket < 0)
		throw SocketException("accept");
	return newSocket;
}
