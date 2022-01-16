/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 17:09:40 by zera              #+#    #+#             */
/*   Updated: 2022/01/16 11:55:28 by zera             ###   ########.fr       */
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
# include "Settings/ServerSettings.hpp"
# include "ClientService.hpp"
# include "ServerClientService.hpp"
# include "ConnectionsService.hpp"
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
		static ClientService				_clientService;
		static ConnectionsService			_connectionsService;
		static ServerClientService			_serverClientService;
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
		void		serverClientReadEvent(int fd, std::string &rawRq);
		void		clientReadEvent(int fd, std::string &rawRq);
		void		sendEvent(int sock);

};


#endif