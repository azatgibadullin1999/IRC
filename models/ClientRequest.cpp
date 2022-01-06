/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 22:07:48 by root              #+#    #+#             */
/*   Updated: 2022/01/06 16:48:45 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ClientRequest.hpp"

ClientRequest::ClientRequest(const std::vector<std::string> &requestData, const Commands::ClientCommandType type, const std::string &UID) :
	_requestData(requestData), _type(type), _UID(UID) { }

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

const std::string				&ClientRequest::getUID() const {
	return _UID;
}
