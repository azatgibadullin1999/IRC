/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 22:07:48 by root              #+#    #+#             */
/*   Updated: 2022/01/07 19:37:21 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ClientRequest.hpp"

ClientRequest::ClientRequest(const std::vector<std::string> &requestData, const Commands::ClientCommandType type, const UID &uid) :
	_requestData(requestData), _type(type), _uid(uid) { }

ClientRequest::~ClientRequest() { }

bool				ClientRequest::isMessage() const {
	return _type;
}

bool				ClientRequest::isComand() const {
	return _type;
}

Commands::ClientCommandType		ClientRequest::whichComand() const {
	return _type;
}

const std::string				&ClientRequest::getComand() const {
	return _requestData[0];
}

const std::vector<std::string>	&ClientRequest::getArguments() const {
	return _requestData;
}

const std::string				&ClientRequest::getMessage() const {
	return _requestData[0];
}

const UID						&ClientRequest::getUID() const {
	return _uid;
}

void			ClientRequest::setNumberResponses(unsigned int num) {
	_numberOfWaitResponses = num;
}

unsigned int	ClientRequest::decrementNumberResponses() {
	return --_numberOfWaitResponses;
}

unsigned int	ClientRequest::getNumberResponses() const {
	return _numberOfWaitResponses;
}

void			ClientRequest::addResponse(const ServerMessage &resp) {
	_responses.push_back(resp);
}

bool			ClientRequest::compareNumberResponse() const {
	return _responses.size() == _numberOfWaitResponses;
}
