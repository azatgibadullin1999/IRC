#include "SocketBase.hpp"

SocketBase::SocketBase(const std::string &domain, const std::string &port) throw(SocketException) {
	struct addrinfo hints, *rp;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(!domain.empty() ? domain.c_str() : NULL, port.c_str(), &hints, &_address) != 0) {
		throw SocketException("getaddrinfo error");
	}
	try {
		setSocket(_socket = socket(_address->ai_family, _address->ai_socktype, _address->ai_protocol));
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	std::cout << "Creating Socket" << std::endl;
	freeaddrinfo(_address);
}

SocketBase::~SocketBase() {
	close(_socket);
}

struct addrinfo *SocketBase::getAddress() {
	return _address;
}

int SocketBase::getSocket() {
	return _socket;
}

void SocketBase::setSocket(int socket) {
	int yes = 1;
	int flags;

	// if (bind(getSocket(), _address->ai_addr, _address->ai_addrlen)) {
	// 	std::cout << "" << std::endl;
	// 	throw SocketException(strerror(errno));
	// }
	if ((flags = fcntl(socket, F_GETFL, 0)) < 0)
	    throw SocketException("Error F_GETFL");
	flags |= O_NONBLOCK;
	if (fcntl(socket, F_SETFL, flags) < 0)
	    throw SocketException("Error F_SETFL");
	if ((_socket = socket) < 0 || setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0)
		throw SocketException("Socket didn't created");
}
