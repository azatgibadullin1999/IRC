/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionsService.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 16:48:16 by zera              #+#    #+#             */
/*   Updated: 2022/01/21 01:46:15 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ConnectionsService.hpp"

ConnectionsService::~ConnectionsService(void) {
	for (std::vector<Connection*>::iterator connection = _connections.begin();
		 connection < _connections.end(); connection++) {
		delete(*connection);
	}
	_connections.clear();
}

void						ConnectionsService::setFds(fd_set &readFds, fd_set &writeFds) {
	for (std::vector<Connection*>::iterator connection = _connections.begin();
		 connection < _connections.end(); connection++) {
		FD_SET((*connection)->getSocket(), &readFds);
		if ((*connection)->getResponses().size() != 0) {
			FD_SET((*connection)->getSocket(), &writeFds);
		}
	}
}

std::string					ConnectionsService::addRequest(int socket, std::string rq) {
	Connection*		connection = _getConnection(socket);
	if (connection) {
		std::string rawRq = connection->getRawRq() + rq;
		if (rawRq.find('\n') == std::string::npos) {
			connection->setRawRq(rawRq);
			return ("");
		} else {
			connection->setRawRq("");
			return rawRq;
		}
	}
	return ("");
}

void						ConnectionsService::addConnection(int socket) {
	_connections.push_back(new Connection(socket));
}

Connection::ConnectionType	ConnectionsService::getTypeConnection(int socket) {
	Connection*		connection = _getConnection(socket);
	if (connection != NULL) {
		return connection->getType();
	}
	return Connection::NONE;
}

void						ConnectionsService::disconnect(int socket) {
	for (std::vector<Connection*>::iterator connection = _connections.begin();
		 connection < _connections.end(); connection++) {
		if ((*connection)->getSocket() == socket) {
			_connections.erase(connection);
			return ;
		}
	}
}

void						ConnectionsService::setTypeConnection(int socket, Connection::ConnectionType type) {
	_getConnection(socket)->setType(type);
}

void						ConnectionsService::addResponse(int socket, std::string response) {
	Connection *connection = _getConnection(socket);
	if (connection != NULL) {
		connection->getResponses().push_back(response);
	}
}

void						ConnectionsService::sendMsg(int socket) {
	Connection *connection = _getConnection(socket);
	if (connection != NULL) {
		for(std::vector<std::string>::iterator response = connection->getResponses().begin();
				response < connection->getResponses().end(); response++) {
			send(socket, (*response).c_str(), (*response).size(), 0);
		}
		connection->getResponses().clear();
	}
}

Connection*					ConnectionsService::_getConnection(int socket) {
	for (std::vector<Connection*>::iterator connection = _connections.begin();
		 connection < _connections.end(); connection++) {
		if ((*connection)->getSocket() == socket) {
			return (*connection);
		}
	}
	return (NULL);
}
