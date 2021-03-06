/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hapryl <hapryl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 17:09:40 by zera              #+#    #+#             */
/*   Updated: 2022/01/22 14:05:40 by hapryl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __SERVER_HPP__
# define __SERVER_HPP__

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 2056
#endif

# include <stdio.h>
# include <sys/time.h>
# include <sys/types.h>
# include <unistd.h>
# include <vector>
# include "Sockets/ServerSocket.hpp"
# include "Settings/ServerSettings.hpp"
# include "ClientService.hpp"
# include "ConnectionsService.hpp"
# include "../services/Parser.hpp"
# include "../models/models.hpp"
# include "../services/utils.hpp"


class Server
{
	public:
		Server(ServerSettings * settings);
		~Server(void) {
			delete(_serverSettings);
		}
		void		run();

	private:
		static ClientService				_clientService;
		static ConnectionsService			_connectionsService;
		static Parser						_parser;

		ServerSettings						*_serverSettings;
		ServerSocket						_serverSocket;
		fd_set								_readFds;
		fd_set								_writeFds;
		int									_fdMax;

		void		setFds(fd_set &readFds, fd_set &writeFds);
		void		checkStatusReadFd(fd_set &readFds, fd_set &writeFds);
		void		connectEvent();
		void		disconnectEvent(int fd);
		void		readEvent(int fd);
		void		connectionReadEvent(int fd, std::string &rawRq);
		void		clientReadEvent(int fd, std::string &rawRq);
		void		sendEvent(int sock);

};


#endif