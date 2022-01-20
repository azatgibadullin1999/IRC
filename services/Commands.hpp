/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 18:32:23 by root              #+#    #+#             */
/*   Updated: 2022/01/21 01:26:48 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	__COMMANDS_HPP__
# define __COMMANDS_HPP__


# include <iostream>
# include <vector>


struct		Commands {

	private :
	
		std::vector<std::string>		_clientCommands;
	
	public :

		enum Status {
			SUCCESS,
			FAIL,
			ERROR,
		} ;

		enum ClientCommandType {
			MESSAGE,
			PRIVMSG,
			NICK,
			JOIN,
			LEAVE,
			QUIT,
			WHO,
			LIST,
			HELP,
			REGISTR,
			LOGIN,
			COMMAND_NOT_FOUND,
			OPER,
			KICK,
			KILL,
			DIE,
		} ;

		Commands() ;

		~Commands() ;

		ClientCommandType		whichClientCommand(const std::string &str) const ;

} ;


#endif