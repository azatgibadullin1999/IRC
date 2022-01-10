#ifndef SERVERSOCKET_HPP
# define SERVERSOCKET_HPP

#include "SocketBase.hpp"

class ServerSocket : public SocketBase {
	private:
	public:
		ServerSocket(const std::string &domain, const std::string &port) throw(SocketException);
		virtual ~ServerSocket();

		void	connect();
		void	startListening();
		int		accept();
};

#endif