/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientService.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 10:22:23 by zera              #+#    #+#             */
/*   Updated: 2022/01/16 10:53:51 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __SERVERCLIENTSERVICE_HPP__
# define __SERVERCLIENTSERVICE_HPP__


# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include "ServerModels/ServerClient.hpp"
# include "../models/models.hpp"
# include "../models/Response.hpp"


class ServerClientService
{
	public:
		ServerClientService(void) { }
		~ServerClientService(void) { }

		void	addServerClient(int socket);

		void		sendConnectionRequest(int socket, ServerMessage *request);

		Response	*addRequest(int socket, ServerMessage *request, Response *response);

		void		addConnectionRequest(int socket, ServerMessage *request);

		Response	*addResponse(ServerMessage *request);

		void	inBoxMsg(int socket, ServerMessage *serverMessage);

		void	sendServerMsg(int socket);

		void	setFds(fd_set &writeFds);

	private:
		std::vector<ServerClient*>		_serverClients;

		std::vector<ServerMessage*>		_ourMessages;

		ServerClient		*_getServerClient(int socket);

		Response			*_execudeRequest(std::vector<ServerMessage*>::iterator &request);

		void				_execudeRequest(ServerClient *serverClient, std::vector<ServerMessage*>::iterator &request);
};


#endif