/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 17:08:52 by zera              #+#    #+#             */
/*   Updated: 2022/01/11 20:05:33 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// ClientService				Server::_clientService = ClientService();
// ServerClientService			Server::_serverClientService = ServerClientService();
Parser						Server::_parser = Parser();


Server::Server(ServerSettings * settings) :
	_serverSocket(ServerSocket(settings->getPort())) {}

void	Server::run () {
	std::cout << "Starting..." << _serverSocket.getSocket() << std::endl;
	fd_set readFds;
	fd_set writeFds;
	_fdMax = _serverSocket.getSocket();
	FD_ZERO(&_connectionFds);
	while (true)
	{
		FD_ZERO(&readFds);
		FD_ZERO(&writeFds);
		FD_COPY(&_connectionFds, &readFds);
		FD_SET(_serverSocket.getSocket(), &readFds);
		// FD_COPY(&_connectionFds, &writeFds);
		std::cout << "select" << std::endl;
		if (select(_fdMax + 1, &readFds, &writeFds, NULL, NULL) == -1) {
			perror("select");
			_exit(4);
		}
		checkStatusReadFd(readFds);
	}
}

void Server::checkStatusReadFd(fd_set &readFds) {
	if (FD_ISSET(_serverSocket.getSocket(), &readFds))
		connectEvent();
	for (unsigned long fd = 4; fd <= _fdMax; fd++) {
		if (FD_ISSET(fd, &readFds)) {
			readEvent(fd);
		}
		// if (FD_ISSET(fd, &writeFds)) {
		// 	sendEvent(fd);
		// }
	}
}

void Server::connectEvent() {
	try {
		int		socket = _serverSocket.accept();
		// _clientService.addClient(socket);
		FD_SET(socket, &_connectionFds);
		_fdMax = std::max(_fdMax, socket);
		std::cout << "Client connected " << socket << std::endl;
	} catch(std::exception &e) {
		perror("socket accept");
	}
}

void Server::disconnectEvent(int fd) {
	FD_CLR(fd, &_connectionFds);
	//clientService.delete(fd);
	std::cout << "Client disconnected " << fd << std::endl;
}

void Server::readEvent(int fd) {
	char			buffer[BUFFER_SIZE + 1];
	int nDataLenght = recv(fd, buffer, BUFFER_SIZE, 0);
	if (nDataLenght < 1) {
		disconnectEvent(fd);
		return ;
	}
	buffer[nDataLenght] = 0;
	// std::cout << "Reading: ";
	// std::cout << "\"" << buffer << "\" "  << nDataLenght << std::endl;
	UID		uid = UID(/*_serverSettings->getPort()*/8080, 1, fd);
	ClientRequest *clientRequest = _parser.generateClientRequest(buffer, uid);
	if (clientRequest->isCommand())
		std::cout << "ClientCommand " << clientRequest->getCommand() << " " << clientRequest->getArguments()[1] << std::endl;
	else
		std::cout << "ClientMsg " << clientRequest->getMessage() << std::endl;

	// clientService.addRequest(fd, clientRequest);
}

void Server::sendEvent(Client client) {
	// for (std::vector<Client*>::iterator i = _clients.begin(); i < _clients.end(); i++)
	// {
	// 	if ((*i)->getSocket() != client.getSocket())
	// 		send((*i)->getSocket(), _buffer, nDataLenght, 0);
	// }
}



// SERVER Password Request CONNECT
// SERVER Password [NICKNAME Azat] 8080:4:3
// SERVER Password Response CONNECT

// NICKNAME Azat