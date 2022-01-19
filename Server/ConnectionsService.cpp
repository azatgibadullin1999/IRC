/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionsService.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 16:48:16 by zera              #+#    #+#             */
/*   Updated: 2022/01/19 19:46:45 by zera             ###   ########.fr       */
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

bool						ConnectionsService::addRegistrationRequest(int socket, ClientRequest *rq) {
	Connection *connection = _getConnection(socket);
	std::cout << "Set rq uid in connection service" << rq->getUID().toString();
	if (connection != NULL) {
		connection->setClientRequest(rq);
	}
	return false;
}

int							ConnectionsService::checkClientRequest(const UID &uid) {
	std::cout << "---START---" << std::endl;
	for (std::vector<Connection*>::iterator connection = _connections.begin();
		 connection < _connections.end(); connection++) {
		if ((*connection)->getClientRequest() != NULL) {
			if ((*connection)->getClientRequest()->getUID() == uid) {
				std::cout << (*connection)->getSocket() << "UID " << (*connection)->getClientRequest()->getUID().toString() << " = " << uid.toString() << std::endl;
				delete (*connection)->getClientRequest();
				(*connection)->setClientRequest(NULL);
				std::cout << "---END---" << std::endl;
				return (*connection)->getSocket();
			}
		}
	}
	std::cout << "---END---" << std::endl;
	return 0;
}

void						ConnectionsService::addConnection(int socket) {
	_connections.push_back(new Connection(socket));
}

int							ConnectionsService::addConnection(std::string host, int port) {
	struct sockaddr_in	addr;
	int 				sd;

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		throw (ConnectionException("Socket create failed"));
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(host.c_str());
	connect(sd, (sockaddr*)&addr, sizeof(sockaddr));
	int error_code;
	unsigned int error_code_size = sizeof(error_code);
	getsockopt(sd, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
	if (error_code != 0) {
		throw (ConnectionException(strerror(error_code)));
	}
	Connection *connection = new Connection(sd);
	connection->setType(Connection::SERVER);
	_connections.push_back(connection);
	return (sd);
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
