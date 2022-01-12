/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionsService.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 16:48:16 by zera              #+#    #+#             */
/*   Updated: 2022/01/12 22:29:14 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ConnectionsService.hpp"

std::string					ConnectionsService::addRequest(int socket, std::string rq) {
	for (std::vector<Connection*>::iterator connection = _connections.begin();
		 connection < _connections.end(); connection++) {
		if ((*connection)->getSocket() == socket) {
			std::string rawRq = (*connection)->getRawRq() + rq;
			if (rawRq.find('\n') != std::string::npos) {
				(*connection)->setRawRq(rawRq);
				return ("");
			} else {
				(*connection)->setRawRq("");
				return rawRq;
			}
		}
	}
	return ("");
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
	_connections.push_back((new Connection(sd))->setType(Connection::SERVER));
	return (sd);
}

Connection::ConnectionType	ConnectionsService::getTypeConnection(int socket) {
	return Connection::SERVER;
}
