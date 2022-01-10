#ifndef SOCKETBASE_HPP
# define SOCKETBASE_HPP

# include <unistd.h>
# include <iostream>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <sys/socket.h>
# include <errno.h>
# include <string.h>
# include <sys/types.h>
# include <netdb.h>

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

class SocketBase {
	private:
		struct addrinfo		*_address;
		int					_socket;

	protected:
		struct addrinfo		*getAddress();

		void				setSocket(int socket);
	public:
		SocketBase(const std::string &domain, const std::string &port) throw(SocketException);
		SocketBase(SocketBase const & s);
		virtual ~SocketBase();

		int				getSocket();
};


#endif