/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientService.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 11:27:01 by zera              #+#    #+#             */
/*   Updated: 2022/01/12 23:57:47 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerClientService.hpp"

void	ServerClientService::addServerClient(int socket) {
	_serverClients.push_back(new ServerClient(socket));
}

void	ServerClientService::addRequest(int socket, ServerMessage *request) {
	for (std::vector<ServerClient*>::iterator client = _serverClients.begin();
			client < _serverClients.end(); client++) {
		if ((*client)->getSocket() == socket) {
			(*client)->getRequests().push_back(request);
			return ;
		}
	}
}

void	ServerClientService::addResponse(ServerMessage *response) {
	if (response->getServerCommand() == Commands::REQUEST) {
		return ;
	}
	for (std::vector<ServerClient*>::iterator client = _serverClients.begin();
			client < _serverClients.end(); client++) {
		if ((*client)->getSocket() == response->getUID().getUserId()) {
			for (std::vector<ServerMessage*>::iterator request = (*client)->getRequests().begin();
				request < (*client)->getRequests().end(); request++) {
					if ((*request)->getUID() == response->getUID()) {
						// if ((*request)->addResponse(response) == true) {
						// 	_execudeRequest((*client), (*request));
						// }
					}
				}
		}
	}
}

bool	ServerClientService::needResponse(ClientRequest &request) {

}

bool	ServerClientService::needResponse(ServerMessage &request) {

}

int		ServerClientService::sendRequest(int socket) {
	for (std::vector<ServerClient*>::iterator serverClient = _serverClients.begin();
		 serverClient < _serverClients.end(); serverClient++) {
		if ((*serverClient)->getSocket() == socket) {
			for (std::vector<ServerMessage*>::iterator rq = ++(*serverClient)->getRequests().begin();
				 rq <= (*serverClient)->getRequests().end(); rq++) {
				send(socket, (*(--rq))->toString().c_str(), (*(--rq))->toString().length(), 0);
				(*serverClient)->getRequests().erase((--rq));
			}
			
		}
	}
}

void	ServerClientService::_execudeRequest(ServerClient &serverClient, ServerMessage *request) {
	// request.
}
