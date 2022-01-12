/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientService.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 10:22:23 by zera              #+#    #+#             */
/*   Updated: 2022/01/12 23:54:53 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __SERVERCLIENTSERVICE_HPP__
# define __SERVERCLIENTSERVICE_HPP__


# include <vector>
# include <sys/types.h>
# include <sys/socket.h>
# include "ServerModels/ServerClient.hpp"
# include "../models/models.hpp"


class ServerClientService
{
	public:
		ServerClientService(void);
		~ServerClientService(void);

		void	addServerClient(int socket);

		void	addRequest(int socket, ServerMessage *request);

		void	addResponse(ServerMessage *response);

		bool	needResponse(ClientRequest &request);

		bool	needResponse(ServerMessage &request);

		int		sendRequest(int socket);

	private:
		std::vector<ServerClient*>		_serverClients;

		void	_execudeRequest(ServerClient &serverClient, ServerMessage *request);
};


#endif