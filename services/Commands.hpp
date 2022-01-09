/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 18:32:23 by root              #+#    #+#             */
/*   Updated: 2022/01/08 13:43:16 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	__COMMANDS_HPP__
# define __COMMANDS_HPP__


# include <iostream>
# include <vector>


struct		Commands {

	private :
	
		std::vector<std::string>		_commands;
	
	public :

	enum ClientCommandType {
		MESSAGE,
		NICK,
		JOIN,
		PRIVMSG,
		LEAVE,
		QUIT,
		WHO	,
		LIST,
		HELP,
	} ;

	enum ServerCommandType {
		REQUEST,
		RESPONSE,
	} ;

	Commands() ;

	~Commands() ;

	ClientCommandType		isCommand(const std::string &str) const ;

} ;


#endif