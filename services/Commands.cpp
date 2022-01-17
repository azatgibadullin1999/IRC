/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 22:21:00 by root              #+#    #+#             */
/*   Updated: 2022/01/17 15:47:07 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Commands.hpp"

Commands::Commands() {
	_clientCommands.reserve(8);
	_clientCommands.push_back("/PRIVMSG");
	_clientCommands.push_back("/NICK");
	_clientCommands.push_back("/JOIN");
	_clientCommands.push_back("/LEAVE");
	_clientCommands.push_back("/QUIT");
	_clientCommands.push_back("/WHO");
	_clientCommands.push_back("/LIST");
	_clientCommands.push_back("/HELP");

	_clientCommands.push_back("/REGISTER");
	_clientCommands.push_back("/LOGIN");

	// _clientCommands.push_back("/OPER");
	// _clientCommands.push_back("/KICK");
	// _clientCommands.push_back("/KILL");

	_serverCommands.push_back("SERVER ");
}

Commands::~Commands() { }

Commands::ClientCommandType		Commands::whichClientCommand(const std::string &str) const {
	std::vector<std::string>::const_iterator it = _clientCommands.begin();
	
	for (; it != _clientCommands.end(); it++) {
		if (it->size() <= str.size()
			&& !it->compare(0, it->size(), str, 0, it->size())
			&& (str[it->size()] == '\n' || str[it->size()] == ' '))
			return ClientCommandType((it.base() - _clientCommands.begin().base()) + 1);
	}

	if (!str.empty() && str[0] == '/')
		return ClientCommandType::COMMAND_NOT_FOUND;

	return MESSAGE;
}

bool	Commands::isServerCommand(const std::string &str) const {
	std::vector<std::string>::const_iterator it = _serverCommands.begin();
	
	for (; it != _serverCommands.end(); it++) {
		if (!it->compare(0, it->size(), str, 0, it->size()))
			return true;
	}

	return false ;
}
