/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 22:07:48 by root              #+#    #+#             */
/*   Updated: 2022/01/21 01:44:39 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ClientRequest.hpp"

ClientRequest::ClientRequest(const std::vector<std::string> &requestData, const Commands::ClientCommandType type, const UID &uid) :
	_requestData(requestData), _uid(uid), _type(type) { }

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

void		ClientRequest::setChannelAndNick(const std::string &channel, const std::string &nick) {
	_requestData.insert(_requestData.begin(), nick);
	_requestData.insert(_requestData.begin(), channel);
}

void		ClientRequest::setNick(const std::string &nick) {
	_requestData.insert(_requestData.begin(), nick);
}