/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 22:21:00 by root              #+#    #+#             */
/*   Updated: 2022/01/06 15:52:19 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Commands.hpp"

Commands::Commands() {
	_commands.reserve(8);
	_commands.push_back("NICK ");
	_commands.push_back("JOIN ");
	_commands.push_back("LEAVE ");
	_commands.push_back("QUIT ");
	_commands.push_back("WHO ");
	_commands.push_back("PRIVMSG ");
	_commands.push_back("LIST ");
	_commands.push_back("HELP ");
}

Commands::~Commands() { }

Commands::ClientCommandType		Commands::isCommand(const std::string &str) const {
	std::vector<std::string>::const_iterator it = _commands.begin();
	
	for (; it != _commands.end(); it++) {
		if (!it->compare(0, it->size(), str, 0, it->size()))
			return ClientCommandType((it.base() - _commands.begin().base()) + 1);
	}

	return MESSAGE;
}
