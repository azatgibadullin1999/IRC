/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 17:08:52 by zera              #+#    #+#             */
/*   Updated: 2022/01/04 22:20:51 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() {
	_host = "127.0.0.1";
	_port = 11111;
	_serverSocket = new ServerSocket(_host.c_str(), _port);
}

void Server::run () {
	std::cout << "Starting..." << _serverSocket->getSocket() << std::endl;
	while (true)
	{
		FD_ZERO(&_readFds);
		FD_ZERO(&_writeFds);
		FD_SET(_serverSocket->getSocket(), &_readFds);
		_fdMax = _serverSocket->getSocket();
		for (unsigned long i = 0; i < _clients.size(); i++) {
			if (_fdMax < _clients[i]->getSocket())
				_fdMax = _clients[i]->getSocket();
			if (_clients[i]->getState() == Client::READ) {
				// std::cout << "Stage reading..." << _clients[i]->getSocket() << std::endl;
				FD_SET(_clients[i]->getSocket(), &_readFds);
			} else {
				// std::cout << "Stage writing..." << _clients[i]->getSocket() << std::endl;
				FD_SET(_clients[i]->getSocket(), &_writeFds);
			}
		}
		std::cout << "select" << std::endl;
		if (select(_fdMax + 1, &_readFds, &_writeFds, NULL, NULL) == -1) {
			perror("select");
			_exit(4);
		}
		for (unsigned long i = 0; i < _clients.size(); i++) {
			if (FD_ISSET(_clients[i]->getSocket(), &_readFds)) {
				readEvent(*_clients[i]);
			} else if (FD_ISSET(_clients[i]->getSocket(), &_writeFds)) {
				sendEvent(*_clients[i]);
			}
		}
		if (FD_ISSET(_serverSocket->getSocket(), &_readFds)) {
			connectEvent();
		}
	}
}

void Server::connectEvent() {
	try {
		Client *client = new Client(_serverSocket->accept());
		_clients.push_back(client);
		std::cout << "Client connected" << std::endl;
	} catch(std::exception &e) {
		perror("socket accept");
		std::cout << e.what() << std::endl;
	}
}

void Server::readEvent(Client client) {
	char			_buffer[BUFFER_SIZE + 1];
	std::cout << "Reading: ";
	int nDataLenght = recv(client.getSocket(), _buffer, BUFFER_SIZE, 0);
	_buffer[nDataLenght] = 0;
	std::cout << "\"" << _buffer << "\"" << std::endl;
	for (std::vector<Client*>::iterator i = _clients.begin(); i < _clients.end(); i++)
	{
		if ((*i)->getSocket() != client.getSocket())
			send((*i)->getSocket(), _buffer, nDataLenght, 0);
	}
	
}

void Server::sendEvent(Client client) {

}

