/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientService.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 10:22:23 by zera              #+#    #+#             */
/*   Updated: 2022/01/08 16:00:00 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __SERVERCLIENTSERVICE_HPP__
# define __SERVERCLIENTSERVICE_HPP__


# include <vector>
# include "ServerModels/ServerClient.hpp"
# include "../models/models.hpp"


class ServerClientService
{
	public:
		ServerClientService(void);
		~ServerClientService(void);

		void	addRequest(int socket, ServerMessage *request);

		void	addResponse(ServerMessage *response);

		bool	needResponse(ClientRequest &request);

		bool	needResponse(ServerMessage &request);

		int		sendRequest(ServerMessage &request);

	private:
		std::vector<ServerClient>		_serverClients;

		void	_execudeRequest(ServerClient &serverClient, ServerMessage *request);
};


#endif