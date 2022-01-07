/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 17:16:03 by root              #+#    #+#             */
/*   Updated: 2022/01/07 17:25:41 by root             ###   ########.fr       */
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
	ft::strtrim(rawRequest);
	
	type = _commands.isCommand(rawRequest);
	requestData.insert(requestData.begin(), ft::to_string(type));
	if (type)
		__createClientRequestByCommand(std::string(rawRequest, rawRequest.find(' ')), requestData);
	else
		__createClientRequestByMessage(rawRequest, requestData);

	return new ClientRequest(requestData, type, uid);
}

ServerMessage		*Parser::generateServerMessage(std::string rawRequest) {
	Commands::ServerCommandType		type;
	std::vector<std::string>		requestData;

	requestData.reserve(5);
	__createServerReqeustByServerMessage(rawRequest, requestData);
	type = static_cast<Commands::ServerCommandType>(std::atol(requestData[ServerMessage::SERVER_COMMAND].c_str()));

	return new ServerMessage(requestData, type);
}

ServerMessage		*Parser::generateServerMessage(const ClientRequest &processedReqeust) {
	Commands::ServerCommandType		type;
	std::vector<std::string>		requestData;

	requestData.reserve(5);
	__createServerReqeustByClientRequest(processedReqeust, requestData);
	type = Commands::REQUEST;

	return new ServerMessage(requestData, type);
}

//	Private methods

void		Parser::__createClientRequestByCommand(const std::string &rawRequest, std::vector<std::string> &requestData) const {
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

void		Parser::__createClientRequestByMessage(const std::string &rawRequest, std::vector<std::string> &requestData) const {
	requestData.push_back(rawRequest);
}

void		Parser::__createServerReqeustByServerMessage(const std::string &rawRequest, std::vector<std::string> &requestData) const {
	std::string::const_iterator		it2 = rawRequest.begin();
	std::string::const_iterator		it1;

	while (it2 < rawRequest.end()) {
		it1 = it2;
		if (*it2 == '[') {
			std::string::const_iterator		ittmp = it2;
			for (;ittmp < rawRequest.end(); ittmp++) {
				if (*ittmp == ']')
					it2 = ++ittmp;
			}
		}
		else
			for(; isprint(*it2) && !isspace(*it2) && it2 < rawRequest.end(); ++it2) { }
		requestData.push_back(std::string(it1, it2));
		for(; (!isprint(*it2) || isspace(*it2)) && it2 < rawRequest.end(); ++it2) { }
	}
}
		
void		Parser::__createServerReqeustByClientRequest(const ClientRequest &processedReqeust, std::vector<std::string> &requestData) const {
	requestData.push_back("SERVER");
	requestData.push_back(/* password */"XxX__PaSsWoRd__XxX");
	requestData.push_back(ft::to_string(Commands::REQUEST) + " ");
	std::string		tmpClientRequest;
	tmpClientRequest += "[";
	for (std::vector<std::string>::const_iterator it = processedReqeust.getArguments().begin(); it < processedReqeust.getArguments().end(); it++) {
		if (it == processedReqeust.getArguments().end() - 1)
			tmpClientRequest += *it + "]";
		else
			tmpClientRequest += *it + " ";
	}
	requestData.push_back(tmpClientRequest);
	requestData.push_back(processedReqeust.getUID().toString());
}
