/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 17:09:40 by zera              #+#    #+#             */
/*   Updated: 2022/01/04 21:43:17 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# define BUFFER_SIZE 256

# include <stdio.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>
# include <vector>
# include "Sockets/ServerSocket.hpp"
# include "ServerModels/Client.hpp"
# include "ServerModels/ServerClient.hpp"


class Server
{
	public:
		Server(void);
		~Server(void) {}
		void		run();

	private:
		std::string							_host;
		int									_port;
		ServerSocket*						_serverSocket;
		std::vector<Client*>				_clients;
		std::vector<ServerClient*>			_serverClients;
		fd_set								_readFds;
		fd_set								_writeFds;
		int									_fdMax;

		void		connectEvent();
		void		readEvent(Client client);
		void		sendEvent(Client client);

};


#endif