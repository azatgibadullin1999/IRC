#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

# include <string>

class Client
{
	public:
		enum STATE {
			READ,
			WRITE
		};

		Client(int socket) {
			_socket = socket;

		}
		~Client(void) {}

		int getSocket() {
			return (_socket);
		}
	
		STATE getState() {
			return (_state);
		}
		
	private:
		std::string		_nickName;
		std::string		_chanel;
		int				_socket;
		STATE			_state;

};

#endif