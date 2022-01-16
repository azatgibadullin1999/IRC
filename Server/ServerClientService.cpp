/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientService.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 11:27:01 by zera              #+#    #+#             */
/*   Updated: 2022/01/16 21:05:04 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerClientService.hpp"

void	ServerClientService::addServerClient(int socket) {
	std::cout << "New Server!!!" << std::endl;
	_serverClients.push_back(new ServerClient(socket));
}

void		ServerClientService::sendConnectionRequest(int socket, ServerMessage *request) {
	_getServerClient(socket)->getMsgToSend()->push_back(request);
}


Response	*ServerClientService::addRequest(int socket, ServerMessage *request, Response *response) {
	ServerClient *sender = _getServerClient(socket);
	if (request->getServerCommand() == Commands::REQUEST) {
		// Need Response
		if (sender == NULL) {
			// From client add to OUR requests
			if (response->getCommandStatus() == Commands::SUCCESS_NO_SEND || _serverClients.empty()) {
				delete request;
				return response;
			} else {
				_ourMessages.push_back(request);
				// save response
				request->setNumberOfWaitResponses(_serverClients.size());
				for (std::vector<ServerClient*>::iterator serverClient = _serverClients.begin();
					serverClient < _serverClients.end(); serverClient++) {
					(*serverClient)->getMsgToSend()->push_back(request);
				}
			}
		} else {
			// From server add to ServerClient requests
			if (response->getCommandStatus() != Commands::SUCCESS_SEND) {
				sender->getMsgToSend()->push_back(new ServerMessage(sender->getPassword(), Commands::RESPONSE,
					response->getClientCommand(), response->getArguments(), response->getUID().toString()));
				delete request;
				delete response;
			} else {
				sender->getRequests()->push_back(request);
				// save response
				request->setNumberOfWaitResponses(_serverClients.size() - 1);
				for (std::vector<ServerClient*>::iterator serverClient = _serverClients.begin();
					serverClient < _serverClients.end(); serverClient++) {
					if ((*serverClient) != sender) {
						(*serverClient)->getMsgToSend()->push_back(request);
					}
				}
			}
		}
		// Add toSend for other ServerClients and set count
		// if countResponse 0; client return Response ; server form ServerResponse and add toSend for server
	}
	return NULL;
}

void	ServerClientService::addConnectionRequest(int socket, ServerMessage *request) {
	ServerClient *sender = _getServerClient(socket);
	if (request->getServerCommand() == Commands::REQUEST_CONNECT) {
		// Password has been verified
		// Add their password to ServerClient
		sender->setPassword(request->getClientArgs()[0]);
		// Generate Response for ServerClient
		sender->getMsgToSend()->push_back(new ServerMessage(sender->getPassword(), Commands::RESPONSE_CONNECT, ""));
	}
}

Response	*ServerClientService::addResponse(ServerMessage *response) {
	if (response->getServerCommand() == Commands::RESPONSE) {
		for (std::vector<ServerMessage*>::iterator msg = _ourMessages.begin();
			 msg < _ourMessages.end(); msg++) {
			if ((*msg)->getUID() == response->getUID()) {
				(*msg)->addResponse(response);
				if ((*msg)->getNumberOfWaitResponses() == 0) {
					return _execudeRequest(msg);
				}
			}
		}
		for (std::vector<ServerClient*>::iterator client = _serverClients.begin();
			client < _serverClients.end(); client++) {
			for (std::vector<ServerMessage*>::iterator rq = (*client)->getRequests()->begin();
				 rq < (*client)->getRequests()->end(); rq++) {
				if ((*rq)->getUID() == response->getUID()) {
					(*rq)->addResponse(response);
					if ((*rq)->getNumberOfWaitResponses() == 0) {
						_execudeRequest(*client, rq);
						return NULL;
					}
				}
			}
		}
	}
	return NULL;
}

void	ServerClientService::inBoxMsg(int socket, ServerMessage *serverMessage) {
	ServerClient *sender = _getServerClient(socket);
}

void	ServerClientService::setFds(fd_set &writeFds) {
	for (std::vector<ServerClient*>::iterator client = _serverClients.begin();
			client < _serverClients.end(); client++) {
		if(!(*client)->getMsgToSend()->empty()) {
			FD_SET((*client)->getSocket(), &writeFds);
		}
	}
}

void		ServerClientService::sendServerMsg(int socket) {
	ServerClient *serverClient = _getServerClient(socket);
	if (serverClient != NULL) {
		std::vector<ServerMessage*> *toSend = serverClient->getMsgToSend();
		while (toSend->size() != 0) {
			ServerMessage *rq = *(toSend->rbegin());
			send(socket, rq->toString().c_str(), rq->toString().length(), 0);
			toSend->erase(--toSend->end());
			delete (rq);
		}
	}
}

Response	*ServerClientService::_execudeRequest(std::vector<ServerMessage*>::iterator &request) {
	delete *request;
	_ourMessages.erase(request);
	return NULL;
}

void		ServerClientService::_execudeRequest(ServerClient *serverClient, std::vector<ServerMessage*>::iterator &request) {
	delete *request;
	serverClient->getRequests()->erase(request);
}

ServerClient		*ServerClientService::_getServerClient(int socket) {
	for (std::vector<ServerClient*>::iterator serverClient = _serverClients.begin();
		 serverClient < _serverClients.end(); serverClient++) {
		if ((*serverClient)->getSocket() == socket) {
			return (*serverClient);
		}
	}
	return NULL;
}