/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 22:07:48 by root              #+#    #+#             */
/*   Updated: 2022/01/05 14:07:00 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

Request::Request() : _requestData() { }

Request::~Request() { }

bool				Request::isMessage() const {
	return _status;
}

bool				Request::isComand() const {
	return _status;
}

const std::vector<std::string>	&Request::getComand() const {
	return _requestData;
}

const std::string				&Request::getMessage() const {
	return _requestData[0];
}

Request		&Request::createRequest(std::string rawRequest) {
	ft::strtrim(rawRequest);
	
	// if (_commands.isCommand(rawRequest)) {
	// 	_status = COMMAND;
	// 	__createRequestByCommand(rawRequest);
	// }
	// else {
		_status = MESSAGE;
		__createRequestByMessage(rawRequest);
	// }
	
	return *this;
}

void		Request::__createRequestByCommand(std::string &rawRequest) {
	std::string::iterator	it2 = rawRequest.begin();
	std::string::iterator	it1;
	
	while (*it2 != '\0') {
		it1 = it2;
		for(; isprint(*it2) && !isspace(*it2) && *it2 != '\0'; ++it2) { }
		_requestData.push_back(std::string(it1, it2));
		for(; !isprint(*it2) && isspace(*it2) && *it2 != '\0'; ++it2) { }
	}
}

void		Request::__createRequestByMessage(std::string &rawRequest) {
	_requestData.push_back(rawRequest);
}