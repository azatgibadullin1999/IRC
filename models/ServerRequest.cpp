/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRequest.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 21:40:47 by root              #+#    #+#             */
/*   Updated: 2022/01/07 14:15:52 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ServerRequest.hpp"

ServerRequest::ServerRequest(const std::vector<std::string> &requestData, const Commands::ServerCommandType type) :
	_requestData(requestData), _type(type) { }

ServerRequest::~ServerRequest() { }

const std::string		&ServerRequest::getPassword() const {
	__checkArguments(PASSWORD);
	return _requestData[PASSWORD];
}

const std::string		&ServerRequest::getServerCommand() const {
	__checkArguments(SERVER_COMMAND);
	return _requestData[SERVER_COMMAND];
}

const std::string		&ServerRequest::getClientArgs() const {
	__checkArguments(CLIENT_ARGS);
	return _requestData[CLIENT_ARGS];
}

const std::string		&ServerRequest::getUID() const {
	__checkArguments(UID);
	return _requestData[UID];
}

void		ServerRequest::__checkArguments(ArgumentNumber n) const {
	if (_requestData.size() <= n)
		throw FewArguments();
}
