/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:40:47 by root              #+#    #+#             */
/*   Updated: 2022/01/18 21:36:13 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerMessage.hpp"

ServerMessage::ServerMessage(
	const Commands::ServerCommandType &serverCommandType,
	const Commands::ClientCommandType &clientCommandType,
	const std::vector<std::string> &clientArgs,
	const std::string &uid) :
		_serverCommandType(serverCommandType),
		_clientCommandType(clientCommandType),
		_clientArgs(clientArgs),
		_uid(uid) { }

ServerMessage::ServerMessage(
	const std::string &password,
	const Commands::ServerCommandType &serverCommandType,
	const Commands::ClientCommandType &clientCommandType,
	const std::vector<std::string> &clientArgs,
	const std::string &uid) :
		_password(password),
		_serverCommandType(serverCommandType),
		_clientCommandType(clientCommandType),
		_clientArgs(clientArgs),
		_uid(uid) { }

ServerMessage::ServerMessage(
	const std::string &password,
	const Commands::ServerCommandType &serverCommandType,
	const Commands::Status &status,
	const std::vector<std::string> &clientArgs,
	const std::string &uid) :
		_password(password),
		_serverCommandType(serverCommandType),
		_status(status),
		_clientArgs(clientArgs),
		_uid(uid) { }

ServerMessage::ServerMessage(
	const std::string &passwordToConnect,
	const Commands::ServerCommandType &command,
	const std::string &ourPassword) :
		_password(passwordToConnect),
		_serverCommandType(command),
		_clientCommandType(Commands::MESSAGE),
		_clientArgs(),
		_uid("0:14:88") { 
			_clientArgs.push_back(ourPassword);
		}

ServerMessage::~ServerMessage() { }

const std::string		&ServerMessage::getPassword() const {
	return _password;
}

void					ServerMessage::setPassword(const std::string &password) {
	_password = password;
}

const Commands::ServerCommandType		&ServerMessage::getServerCommand() const {
	return _serverCommandType;
}

const Commands::ClientCommandType		&ServerMessage::getClientCommand() const {
	return _clientCommandType;
}

const Commands::Status					&ServerMessage::getStatus() const {
	return _status;
}

const std::vector<std::string>			&ServerMessage::getClientArgs() const {
	return _clientArgs;
}

std::vector<ServerMessage *>			&ServerMessage::getResponses() {
	return _response;
}

const UID							&ServerMessage::getUID() const {
	return _uid;
}

bool		ServerMessage::isServerConnectCommand() const {
	if (_serverCommandType > 1)
		return true ;
	return false ;
}

void		ServerMessage::setNumberOfWaitResponses(unsigned int num) {
	_numberOfWaitResponses = num;
}

unsigned int		ServerMessage::getNumberOfWaitResponses() {
	return _numberOfWaitResponses;
}

bool		ServerMessage::addResponse(ServerMessage *response) {
	_response.push_back(response);
	return !--_numberOfWaitResponses;
}

const std::string		ServerMessage::toString() const {
	std::string		dst;

	dst += "SERVER " + _password + " " + ft::to_string(_serverCommandType) + " ";
	if (_serverCommandType == Commands::RESPONSE || _serverCommandType == Commands::RESPONSE_CONNECT) {
		dst += ft::to_string(_status) + " " + "[";
	} else {
		dst += ft::to_string(_clientCommandType) + " " + "[";
	}
	
	std::vector<std::string>::const_iterator	it = _clientArgs.begin();
	
	for (; it != _clientArgs.end(); it++)
		dst += *it + " ";
	
	dst += "] " + _uid.toString() + "\n";
	
	return dst;
}
