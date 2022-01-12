/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:40:47 by root              #+#    #+#             */
/*   Updated: 2022/01/12 13:17:05 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerMessage.hpp"

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

const Commands::ServerCommandType		&ServerMessage::getServerCommand() const {
	return _serverCommandType;
}

const Commands::ClientCommandType		&ServerMessage::getClientCommand() const {
	return _clientCommandType;
}

const std::vector<std::string>			&ServerMessage::getClientArgs() const {
	return _clientArgs;
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

bool		ServerMessage::addResponse(ServerMessage *response) {
	_response.push_back(response);
	return !--_numberOfWaitResponses;
}

const std::string		ServerMessage::toString() const {
	std::string		dst;

	dst += "SERVER " + _password + " " + ft::to_string(_serverCommandType) + " " + ft::to_string(_clientCommandType) + " " + "[";
	
	std::vector<std::string>::const_iterator	it = _clientArgs.begin();
	
	for (; it != _clientArgs.end(); it++)
		dst += *it + " ";
	
	dst += "] " + _uid.toString();
	
	return dst;
}
