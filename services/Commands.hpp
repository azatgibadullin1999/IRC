/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 18:32:23 by root              #+#    #+#             */
/*   Updated: 2022/01/15 19:55:32 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	__COMMANDS_HPP__
# define __COMMANDS_HPP__


# include <iostream>
# include <vector>


struct		Commands {

	private :
	
		std::vector<std::string>		_clientCommands;
		std::vector<std::string>		_serverCommands;
	
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
		} ;

		enum ServerCommandType {
			REQUEST,
			RESPONSE,
			REQUEST_CONNECT,
			RESPONSE_CONNECT,
		} ;

		Commands() ;

		~Commands() ;

		ClientCommandType		whichClientCommand(const std::string &str) const ;

		bool					isServerCommand(const std::string &str) const ;

} ;


#endif