#ifndef __SERVERCLIENT_HPP__
# define __SERVERCLIENT_HPP__

class ServerClient
{
	public:
		ServerClient(void);
		~ServerClient(void);

	private:
		int		_socket;
};

#endif