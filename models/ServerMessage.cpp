/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:40:47 by root              #+#    #+#             */
/*   Updated: 2022/01/08 13:15:51 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerMessage.hpp"

ServerMessage::ServerMessage(const std::vector<std::string> &requestData, const Commands::ServerCommandType type) :
	_requestData(requestData), _uid(getStringUID()), _type(type), _waitingResponses(0) { }

ServerMessage::~ServerMessage() { }

const Commands::ServerCommandType	&ServerMessage::getType() const {
	return _type;
}


const std::string					&ServerMessage::getPassword() const {
	__checkArguments(PASSWORD);
	return _requestData[PASSWORD];
}

const std::string					&ServerMessage::getServerCommand() const {
	__checkArguments(SERVER_COMMAND);
	return _requestData[SERVER_COMMAND];
}

const std::string					&ServerMessage::getClientArgs() const {
	__checkArguments(CLIENT_ARGS);
	return _requestData[CLIENT_ARGS];
}

const std::string					&ServerMessage::getStringUID() const {
	__checkArguments(CLIENT_ARGS);
	return _requestData[STR_UID];
}

const UID							&ServerMessage::getUID() const {
	return _uid;
}

std::vector<ServerMessage*>			&ServerMessage::getResponses() {
	return _responses;
}


void								ServerMessage::setUID(UID uid) {
	_uid = uid;
}

void								ServerMessage::setWaitingResponses(int waitingResponses) {
	_waitingResponses = waitingResponses;
}

bool								ServerMessage::addResponse(ServerMessage *response) {
	_responses.push_back(response);
	if (--_waitingResponses == 0) {
		return (true);
	}
	return (false);
}


void		ServerMessage::__checkArguments(ArgumentNumber n) const {
	if (_requestData.size() <= n)
		throw FewArguments();
}
