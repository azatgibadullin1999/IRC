/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 17:16:03 by root              #+#    #+#             */
/*   Updated: 2022/01/19 21:17:36 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "utils.hpp"
#include <stdlib.h>

Parser::Parser() { }

Parser::~Parser() { }

ClientRequest		*Parser::generateClientRequest(std::string rawRequest, const UID &uid) {
	Commands::ClientCommandType		type;
	std::vector<std::string>		requestData;
	ft::strltrim(rawRequest);
	
	type = _commands.whichClientCommand(rawRequest);
	if (type) {
		size_t	spacePos = rawRequest.find(' ');
		if (spacePos != rawRequest.npos)
			__createClientRequest(std::string(rawRequest, spacePos), requestData);
	}
	else
		__createClientRequest(rawRequest, requestData);

	return new ClientRequest(requestData, type, uid);
}

ServerMessage		*Parser::generateServerMessage(const std::string &rawRequest) {
	std::string						password;
	Commands::ServerCommandType		serverCommandType;
	Commands::ClientCommandType		clientCommandType;
	Commands::Status				status;
	std::vector<std::string>		requestData;
	std::string						uid;

	__createServerReqeustByServerMessage(
		rawRequest,
		password,
		serverCommandType,
		clientCommandType,
		status,
		requestData,
		uid);
	if (serverCommandType == Commands::RESPONSE || serverCommandType == Commands::RESPONSE_CONNECT) {
		return new ServerMessage(password, serverCommandType, status, requestData, uid);
	} else {
		return new ServerMessage(password, serverCommandType, clientCommandType, requestData, uid);
	}
}

ServerMessage		*Parser::generateServerMessage(const ClientRequest &processedReqeust) {
	Commands::ServerCommandType		serverCommandType;
	Commands::ClientCommandType		clientCommandType;
	std::vector<std::string>		requestData;

	__createServerReqeustByClientRequest(
		processedReqeust,
		serverCommandType,
		clientCommandType,
		requestData);

	return new ServerMessage(serverCommandType, processedReqeust.getCommand(), requestData, processedReqeust.getUID().toString());
}

bool				Parser::isServerMessage(const std::string &rawRequest) {
	return _commands.isServerCommand(rawRequest);
}


	//
	//	Private methods
	//


void		Parser::__createClientRequest(const std::string &rawRequest, std::vector<std::string> &requestData) const {
	std::string::const_iterator		it2 = rawRequest.begin();
	std::string::const_iterator		it1;


	for(; (!isprint(*it2) || isspace(*it2)) && it2 < rawRequest.end(); ++it2) { }
	while (it2 < rawRequest.end()) {
		it1 = it2;
		for(; isprint(*it2) && !isspace(*it2) && it2 < rawRequest.end(); ++it2) { }
		requestData.push_back(std::string(it1, it2));
		for(; (!isprint(*it2) || isspace(*it2)) && it2 < rawRequest.end(); ++it2) { }
	}
}

void		Parser::__createServerReqeustByServerMessage(const std::string &rawRequest,
						std::string &password,
						Commands::ServerCommandType &serverCommandType,
						Commands::ClientCommandType &clientCommandType,
						Commands::Status &status,
						std::vector<std::string> &requestData,
						std::string &uid) const {
	size_t		posBegin;
	size_t		posEnd;

	posBegin = rawRequest.find(' ') + 1;
	posEnd = rawRequest.find(' ', posBegin);
	password.insert(0, &rawRequest[posBegin], posEnd - posBegin);

	posBegin = posEnd + 1;
	posEnd = rawRequest.find(' ', posBegin);
	serverCommandType = static_cast<Commands::ServerCommandType>(atoll(&rawRequest[posBegin]));

	if (serverCommandType == Commands::RESPONSE || serverCommandType == Commands::RESPONSE_CONNECT) {
		posBegin = posEnd + 1;
		posEnd = rawRequest.find(' ', posBegin);
		status = static_cast<Commands::Status>(atoll(&rawRequest[posBegin]));
	} else {
		posBegin = posEnd + 1;
		posEnd = rawRequest.find(' ', posBegin);
		clientCommandType = static_cast<Commands::ClientCommandType>(atoll(&rawRequest[posBegin]));
	}

	posBegin = rawRequest.find_first_of('[') + 1;
	if ((posEnd = rawRequest.find_last_of(']')) == std::string::npos)
		posEnd = rawRequest.size();
	for (size_t bufpos = posBegin; bufpos < posEnd;) {
		posBegin = bufpos;
		while (rawRequest[bufpos] != '\r'
				&& bufpos < posEnd) { ++bufpos; }

		requestData.push_back(std::string(&rawRequest[posBegin], bufpos - posBegin));

		while (rawRequest[bufpos] == '\r'
				&& bufpos < posEnd) { ++bufpos; }
	}

	posBegin = posEnd + 2;
	uid.insert(0, &rawRequest[posBegin], rawRequest.size() - posBegin);
}
		
void		Parser::__createServerReqeustByClientRequest(
				const ClientRequest &processedReqeust,
				Commands::ServerCommandType &serverCommandType,
				Commands::ClientCommandType &clientCommandType,
				std::vector<std::string> &requestData) const {
	serverCommandType = Commands::REQUEST;
	clientCommandType = processedReqeust.getCommand();
	requestData = processedReqeust.getArguments();
}
