/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hapryl <hapryl@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 16:18:03 by root              #+#    #+#             */
/*   Updated: 2022/01/22 16:20:52 by hapryl           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(const std::vector<std::string> &requestData,
					const UID &uid,
					const Commands::Status &status,
					const Commands::ClientCommandType &clientCommand) :
						_requestData(requestData),
						_uid(uid),
						_status(status),
						_clientCommand(clientCommand) { }

Response::~Response() { }

const UID		&Response::getUID() const {
	return _uid;
}

Commands::Status	Response::getCommandStatus() const {
	return _status;
}

Commands::ClientCommandType		Response::getClientCommand() const {
	return _clientCommand;
}

const std::vector<std::string>	&Response::getArguments() const {
	return _requestData;
}

void			Response::setClientCommand(Commands::ClientCommandType type) {
	_clientCommand = type;
}

void			Response::setStatus(Commands::Status status) {
	_status = status;	
}

std::string		Response::toString() const {
	std::vector<std::string>::const_iterator	it = _requestData.begin();
	std::string		dst;

	for (; it != _requestData.end(); it++)
		dst += *it + ' ';
	dst += '\n';

	return dst;
}
