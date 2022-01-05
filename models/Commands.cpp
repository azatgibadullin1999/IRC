/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 22:21:00 by root              #+#    #+#             */
/*   Updated: 2022/01/05 16:23:18 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Commands.hpp"

Commands::Commands() {
	_commands.reserve(8);
	_commands.push_back("NICK");
	_commands.push_back("JOIN");
	_commands.push_back("LEAVE");
	_commands.push_back("QUIT");
	_commands.push_back("WHO");
	_commands.push_back("PRIVMSG");
	_commands.push_back("LIST");
	_commands.push_back("HELP");
}

Commands::~Commands() { }

Commands::CommandType		Commands::isCommand(const std::string &str) {
	std::vector<std::string>::iterator it = _commands.begin();
	
	for (; it != _commands.end(); it++) {
		if (!it->compare(0, it->size(), str, 0, it->size()))
			return CommandType((it.base() - _commands.begin().base()) + 1);
	}

	return MESSAGE;
}
