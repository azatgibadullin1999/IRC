/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessage.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:40:47 by root              #+#    #+#             */
/*   Updated: 2022/01/07 17:24:52 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerMessage.hpp"

ServerMessage::ServerMessage(const std::vector<std::string> &requestData, const Commands::ServerCommandType type) :
	_requestData(requestData), _uid(getStringUID()), _type(type) { }

ServerMessage::~ServerMessage() { }

const std::string		&ServerMessage::getPassword() const {
	__checkArguments(PASSWORD);
	return _requestData[PASSWORD];
}

const std::string		&ServerMessage::getServerCommand() const {
	__checkArguments(SERVER_COMMAND);
	return _requestData[SERVER_COMMAND];
}

const std::string		&ServerMessage::getClientArgs() const {
	__checkArguments(CLIENT_ARGS);
	return _requestData[CLIENT_ARGS];
}

const std::string		&ServerMessage::getStringUID() const {
	__checkArguments(CLIENT_ARGS);
	return _requestData[STR_UID];
}

const UID				&ServerMessage::getUID() const {
	return _uid;
}

void		ServerMessage::__checkArguments(ArgumentNumber n) const {
	if (_requestData.size() <= n)
		throw FewArguments();
}
