/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 17:08:52 by zera              #+#    #+#             */
/*   Updated: 2022/01/14 15:35:37 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// ClientService				Server::_clientService = ClientService();
ServerClientService			Server::_serverClientService = ServerClientService();
Parser						Server::_parser = Parser();
ConnectionsService			Server::_connectionsService = ConnectionsService();


Server::Server(ServerSettings * settings) :
	_serverSocket(ServerSocket(settings->getPort())) {
		if (settings->haveConnection()){
			int socketConnection = _connectionsService.addConnection(settings->getHostConnection(), settings->getPortConnection());
			_serverClientService.addServerClient(socketConnection);
			_serverClientService.addRequest(socketConnection,
				new ServerMessage(settings->getPasswordConnection(), Commands::REQUEST_CONNECT, settings->getPassword()));
			_fdMax = std::max(socketConnection, _fdMax);
		}
	}

void	Server::run () {
	std::cout << "Starting..." << _serverSocket.getSocket() << std::endl;
	_fdMax = std::max(_serverSocket.getSocket(), _fdMax);
	while (true)
	{
		setFds(_readFds, _writeFds);
		std::cout << "select" << std::endl;
		if (select(_fdMax + 1, &_readFds, &_writeFds, NULL, NULL) == -1) {
			perror("select");
			_exit(4);
		}
		checkStatusReadFd(_readFds, _writeFds);
	}
}

void	Server::setFds(fd_set &readFds, fd_set &writeFds) {
	FD_ZERO(&readFds);
	FD_ZERO(&writeFds);
	_connectionsService.setFds(readFds);
	FD_SET(_serverSocket.getSocket(), &readFds);
	// FD_COPY(&_writeFds, &writeFds);
}


void	Server::checkStatusReadFd(fd_set &readFds, fd_set &writeFds) {
	if (FD_ISSET(_serverSocket.getSocket(), &readFds))
		connectEvent();
	for (unsigned long fd = 4; fd <= _fdMax; fd++) {
		if (FD_ISSET(fd, &readFds)) {
			readEvent(fd);
		}
		if (FD_ISSET(fd, &writeFds)) {
			sendEvent(fd);
		}
	}
}

void Server::connectEvent() {
	try {
		int		socket = _serverSocket.accept();
		_connectionsService.addConnection(socket);
		_fdMax = std::max(_fdMax, socket);
		std::cout << "Connection accepted " << socket << std::endl;
	} catch(std::exception &e) {
		perror("socket accept");
	}
}

void Server::disconnectEvent(int fd) {
	_connectionsService.disconnect(fd);
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
/*
	1. Добавить в коннектион
	2. Проверить всё ли
	3. Проверяем от кого коннектион
	4. None -> Смотрим на сообщение если Коннектион сервис подтвердит коннект то адд сервер если ток пароль не правильный то ноне останется если что-то другое то пользователь
	5. Client -> В клиент парсерРк

*/
	std::string rawRq = _connectionsService.addRequest(fd, buffer);
	if (rawRq.empty()) {
		return ;
	}
	if (_connectionsService.getTypeConnection(fd) == Connection::NONE) {
		// if parser {
		// 	ServerMessage *serverMsg = _parser.generateServerMessage(rawRq);
		// 	if (serverMsg->getPassword() == _serverSettings->getPassword()) {
		// 		_connectionsService.setTypeConnection(fd, Connection::SERVER);
		// 		_serverClientService.addServerClient(fd);
		// 		_serverClientService.addRequest(fd, serverMsg);
		// 	}
		// } else {
		// 	UID		uid = UID(atoll(_serverSettings->getPort().c_str()), 1, fd);
		// 	ClientRequest *clientRequest = _parser.generateClientRequest(rawRq, uid);
		// 	_connectionsService.setTypeConnection(fd, Connection::CLIENT);
		// 	_clientService.addClient(fd);
		// 	_clientService.addRequest(fd, clientRequest);
		// }
	} else if (_connectionsService.getTypeConnection(fd) == Connection::SERVER) {
		// try {
		// 	ServerMessage *serverMsg = _parser.generateServerMessage(rawRq);
			
		// } catch (std::exception &e) {
			
		// }
	}

}

void Server::sendEvent(int sock) {
	_serverClientService.sendRequest(sock);
	std::cout << "Send " << sock << std::endl;
	FD_CLR(sock, &_writeFds);
}



// SERVER Password Request CONNECT
// SERVER Password [NICKNAME Azat] 8080:4:3
// SERVER Password Response CONNECT

// NICKNAME Azat