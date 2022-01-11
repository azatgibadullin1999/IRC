/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 17:09:40 by zera              #+#    #+#             */
/*   Updated: 2022/01/11 20:06:34 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __SERVER_HPP__
# define __SERVER_HPP__

# define BUFFER_SIZE 256

#ifndef FD_COPY
# define FD_COPY(src, dest) memcpy((dest),(src),sizeof *(dest))
#endif


# include <stdio.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>
# include <vector>
# include "Sockets/ServerSocket.hpp"
# include "ServerModels/ServerClient.hpp"
# include "Settings/ServerSettings.hpp"
// # include "ClientService.hpp"
# include "ServerClientService.hpp"
# include "../services/Parser.hpp"
# include "../models/models.hpp"


class Server
{
	public:
		Server(ServerSettings * settings);
		~Server(void) {
			delete(_serverSettings);
		}
		void		run();

	private:
		// static ClientService				_clientService;
		static ServerClientService			_serverClientService;
		static Parser						_parser;

		ServerSettings						*_serverSettings;
		ServerSocket						_serverSocket;
		std::vector<ServerClient*>			_serverClients;
		fd_set								_connectionFds;
		// std::vector<int>					_connectionFds;
		int									_fdMax;

		void		setFds(fd_set *fds);
		void		checkStatusReadFd(fd_set &readFds);
		void		connectEvent();
		void		disconnectEvent(int fd);
		void		readEvent(int fd);
		void		sendEvent(Client client);

};


#endif