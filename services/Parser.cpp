/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 17:16:03 by root              #+#    #+#             */
/*   Updated: 2022/01/21 01:30:45 by zera             ###   ########.fr       */
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