/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 22:21:00 by root              #+#    #+#             */
/*   Updated: 2022/01/04 22:25:26 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Commands.hpp"

Commands::Commands() {
	_comadi.reserve(6);
	_comadi.push_back("NICK ");
	_comadi.push_back("PASS ");
	_comadi.push_back("OPER ");
	_comadi.push_back("QUIT ");
	_comadi.push_back("JOIN ");
	_comadi.push_back("PART ");
}

Commands::~Commands() { }

bool	Commands::isCommand(const std::string &str) {
	std::vector<std::string>::iterator it = _comadi.begin();
	
	for (; it != _comadi.end(); it++) {
		if (*it == str)
			return true;
	}

	return false;
}