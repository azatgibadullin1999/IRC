/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 18:32:23 by root              #+#    #+#             */
/*   Updated: 2022/01/05 16:17:56 by root             ###   ########.fr       */
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

	enum CommandType {
		MESSAGE	= 0,
		NICK	= 1,
		JOIN	= 2,
		LEAVE	= 3,
		QUIT	= 4,
		WHO		= 5,
		PRIVMSG = 6,
		LIST	= 7,
		HELP	= 8,
	} ;

	Commands() ;

	~Commands() ;

	CommandType		isCommand(const std::string &str) ;

} ;


#endif