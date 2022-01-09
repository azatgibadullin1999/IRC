/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 22:07:48 by root              #+#    #+#             */
/*   Updated: 2022/01/09 14:04:57 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ClientRequest.hpp"

ClientRequest::ClientRequest(const std::vector<std::string> &requestData, const Commands::ClientCommandType type, const UID &uid) :
	_requestData(requestData), _type(type), _uid(uid) { }

ClientRequest::~ClientRequest() { }

bool				ClientRequest::isMessage() const {
	return !_type;
}

bool				ClientRequest::isCommand() const {
	return _type;
}

Commands::ClientCommandType		ClientRequest::getCommand()const {
	return _type;
}

const std::vector<std::string>	&ClientRequest::getArguments() const {
	return _requestData;
}

std::string						ClientRequest::getMessage() const {
	std::string		dst;
	std::vector<std::string>::const_iterator		it = _requestData.begin();

	for (; it < _requestData.end(); it++)
		dst += *it + " ";

	return dst;
}

const UID						&ClientRequest::getUID() const {
	return _uid;
}
