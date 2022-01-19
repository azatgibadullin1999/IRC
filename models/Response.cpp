/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 16:18:03 by root              #+#    #+#             */
/*   Updated: 2022/01/19 13:49:11 by zera             ###   ########.fr       */
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

std::string		Response::toString() const {
	std::vector<std::string>::const_iterator	it = _requestData.begin();
	std::string		dst;

	for (; it != _requestData.end(); it++)
		dst += *it + ' ';
	dst += '\n';

	return dst;
}

std::string		Response::toList() const {
	std::vector<std::string>::const_iterator	it = _requestData.begin();
	std::string		dst;

	dst += ColorMessage::serverPrefixSuccess() + *it++ + '\n';
	for (; it != _requestData.end(); it++)
		dst += "\t " + *it + '\n';

	return dst;
}

std::string		Response::toMessage() const {
	std::vector<std::string>::const_iterator	it = _requestData.begin();
	std::string		dst;

	dst += '[' + ColorMessage::channelPrefix(*it++) + ']';
	dst += '[' + ColorMessage::clientPrefix(*it++) + "] ";
	for (; it != _requestData.end(); it++)
		dst += *it + ' ';
	dst += '\n';

	return dst;
}

std::string		Response::toPrivateMessage() const {
	std::vector<std::string>::const_iterator	it = _requestData.begin();
	std::string		dst;

	dst += '[' + ColorMessage::clientPrefix(*it++) + "] ";
	++it;
	for (; it != _requestData.end(); it++)
		dst += *it + ' ';
	dst += '\n';

	return dst;
}
