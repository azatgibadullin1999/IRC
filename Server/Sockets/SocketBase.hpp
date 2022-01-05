#ifndef SOCKETBASE_HPP
# define SOCKETBASE_HPP

# include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

class SocketBase {
	private:
		struct sockaddr_in	_address;
		int					_socket;

	protected:
		struct sockaddr_in	getAddress();

		void				setSocket(int socket);
	public:
		SocketBase() {}
		SocketBase(const char *domain, int port) throw();
		SocketBase(SocketBase const & s);
		virtual ~SocketBase();

		virtual void	connect() = 0;
		int				getSocket();
};


class SocketException : public std::exception {
	private:
		char			*_result;
	public:
		SocketException(const char *msg) throw() : std::exception() {
			std::string total = std::string("Socket Exception: ") + msg;
			_result = strdup(total.c_str());
		}
		virtual ~SocketException() throw() {
			delete(_result);
		}
		virtual const char* what() const throw() {
			return _result;
		}
};

#endif