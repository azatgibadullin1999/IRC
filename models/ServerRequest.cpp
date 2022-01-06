/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:40:47 by root              #+#    #+#             */
/*   Updated: 2022/01/06 19:49:14 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerRequest.hpp"

ServerRequest::ServerRequest(const std::vector<std::string> &requestData, const Commands::ServerCommandType type) :
	_requestData(requestData), _type(type) { }

ServerRequest::~ServerRequest() { }

const std::string		&ServerRequest::getPassword() const {
	return _requestData[PASSWORD];
}

const std::string		&ServerRequest::getServerCommand() const {
	return _requestData[SERVER_COMMAND];
}

const std::string		&ServerRequest::getClientArgs() const {
	return _requestData[CLIENT_ARGS];
}

const std::string		&ServerRequest::getUID() const {
	return _requestData[UID];
}
