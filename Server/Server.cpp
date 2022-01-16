/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 17:08:52 by zera              #+#    #+#             */
/*   Updated: 2022/01/16 14:41:03 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

ClientService				Server::_clientService = ClientService();
ServerClientService			Server::_serverClientService = ServerClientService();
Parser						Server::_parser = Parser();
ConnectionsService			Server::_connectionsService = ConnectionsService();


Server::Server(ServerSettings * settings) :
	_serverSettings(settings),
	_serverSocket(ServerSocket(_serverSettings->getPort())) {
		if (_serverSettings->haveConnection()){
			int socketConnection = _connectionsService.addConnection(_serverSettings->getHostConnection(), _serverSettings->getPortConnection());
			_serverClientService.addServerClient(socketConnection);
			_serverClientService.sendConnectionRequest(socketConnection,
				new ServerMessage(_serverSettings->getPasswordConnection(), Commands::REQUEST_CONNECT, _serverSettings->getPassword()));
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
	_serverClientService.setFds(writeFds);
	_clientService.setFds(writeFds);
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
	
	std::string rawRq = _connectionsService.addRequest(fd, buffer);
	if (rawRq.empty()) {
		return ;
	}
	if (_connectionsService.getTypeConnection(fd) == Connection::NONE) {
		connectionReadEvent(fd, rawRq);
	} else if (_connectionsService.getTypeConnection(fd) == Connection::SERVER) {
		serverClientReadEvent(fd, rawRq);
	} else if (_connectionsService.getTypeConnection(fd) == Connection::CLIENT) {
		clientReadEvent(fd, rawRq);
	}

}

void	Server::connectionReadEvent(int fd, std::string &rawRq) {
	if (_parser.isServerMessage(rawRq)) {
		try {
			ServerMessage *serverMsg = _parser.generateServerMessage(rawRq);

			if (serverMsg->getServerCommand() == Commands::REQUEST_CONNECT) {
				if (serverMsg->getPassword() == _serverSettings->getPassword()) {
					_connectionsService.setTypeConnection(fd, Connection::SERVER);
					_serverClientService.addServerClient(fd);
					_serverClientService.addConnectionRequest(fd, serverMsg);
				}
			}
		} catch (std::exception &e) {
			// Error e.what
		}
	} else {
		// Connection idReq
		UID		uid = UID(atoll(_serverSettings->getPort().c_str()), fd, 0);
		try {
			ClientRequest *clientRequest = _parser.generateClientRequest(rawRq, uid);
			if (clientRequest->getCommand() == Commands::REGISTR || 
				clientRequest->getCommand() == Commands::LOGIN) {
				std::cout << clientRequest->getCommand() << " " << clientRequest->getArguments()[0] << " " << clientRequest->getArguments()[1] << std::endl;
				Response *resp = _clientService.checkToExecute(clientRequest);
				std::cout << resp->getCommandStatus() << std::endl;
				std::cout << "Чекер " << fd << resp->getArguments()[0] << " " << resp->getArguments()[1] << std::endl;
				if (resp->getCommandStatus() == Commands::SUCCESS) {
					resp = _serverClientService.addRequest(fd,
							_parser.generateServerMessage(*clientRequest, _serverSettings->getPassword()), resp);
					if (resp != NULL) {
						// Если серверов нет и всё гуд
						std::cout << "Серверов нет и всё гуд" << std::endl;
						_connectionsService.setTypeConnection(fd, Connection::CLIENT);
						if (clientRequest->getCommand() == Commands::REGISTR) {
							std::cout << fd << resp->getArguments()[0] << " " << resp->getArguments()[1] << " " << " отправлен на регистрацию" << std::endl;
							_clientService.registrClient(fd, resp);
							std::cout << fd << " зарегался" << std::endl;
						} else if (clientRequest->getCommand() == Commands::LOGIN) {
							_clientService.loginClient(fd, resp);
						}
					} else {
						// добавить реквест коннектиону
					}
				} else if (resp != NULL && resp->getCommandStatus() != Commands::SUCCESS) {
					delete resp;
					delete clientRequest;
					// Error нужно зарегаться или залогиниться
				}
			} else {
				delete clientRequest;
				// Error нужно зарегаться или залогиниться
			}
		} catch (std::exception &e) {
			// Error e.what
		}
	}
}

void		Server::serverClientReadEvent(int fd, std::string &rawRq) {
		try {
			ServerMessage *serverMsg = _parser.generateServerMessage(rawRq);
			Response *resp;
			if (serverMsg->getServerCommand() == Commands::REQUEST) {
				ClientRequest *clientRequest = new ClientRequest(serverMsg->getClientArgs(),
											 serverMsg->getClientCommand(), serverMsg->getUID());
				resp = _clientService.checkToExecute(clientRequest);
				_serverClientService.addRequest(fd, serverMsg, resp);
			} else if (serverMsg->getServerCommand() == Commands::RESPONSE) {
				resp = _serverClientService.addResponse(serverMsg);
				if (resp != NULL) {
					_clientService.execute(resp);
				}
			} else if (serverMsg->getServerCommand() == Commands::RESPONSE_CONNECT) {
				// Пока ничего(
			}
		} catch (std::exception &e) {
			
		}
}

void		Server::clientReadEvent(int fd, std::string &rawRq) {
	try {
		// клиент гет идреквест
		UID		uid = UID(atoll(_serverSettings->getPort().c_str()), fd, 1);
		ClientRequest *clientRequest = _parser.generateClientRequest(rawRq, uid);
		_clientService.addRequest(fd, clientRequest);
		Response *response =_clientService.checkToExecute(clientRequest);
		if (response->getCommandStatus() == Commands::SUCCESS) {
			// убрать пароль из генератора и добавить сеттер
			ServerMessage *serverMessage = _parser.generateServerMessage(*clientRequest, _serverSettings->getPassword());
			response = _serverClientService.addRequest(fd, serverMessage, response);
			if (response != NULL) {
				std::cout << fd << " запрос " << response->getClientCommand() << " " << response->getArguments()[0] << std::endl;
				_clientService.execute(response);

			}
		} else {
			// _clientService.error(response);
		}
	} catch (std::exception &e) {

	}
}

void Server::sendEvent(int sock) {
	std::cout << "Send " << sock << " type " << _connectionsService.getTypeConnection(sock) << std::endl;
	if (_connectionsService.getTypeConnection(sock) == Connection::SERVER) {
		_serverClientService.sendServerMsg(sock);
	} else if (_connectionsService.getTypeConnection(sock) == Connection::CLIENT) {
		_clientService.sendResponseToClient(sock);
	}
	FD_CLR(sock, &_writeFds);
}



// SERVER Password Request CONNECT
// SERVER Password [NICKNAME Azat] 8080:4:3
// SERVER Password Response CONNECT

// NICKNAME Azat