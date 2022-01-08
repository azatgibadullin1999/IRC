/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessage.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 20:36:13 by root              #+#    #+#             */
/*   Updated: 2022/01/08 20:01:10 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_REQUEST_HPP__
# define __SERVER_REQUEST_HPP__


# include <vector>
# include <string>
# include <exception>
# include "../services/Commands.hpp"
# include "UID.hpp"


class ServerMessage {

	private:

		/*
		SERVER password CONNECT RESPONSE
		SERVER password CONNECT REQUEST

		SERVER password RESPONSE [***] UID
		SERVER password REQUEST [***] UID

		SERVER password REQUEST [ 1 Azat ] UID
		*/

		std::string						_password;
		Commands::ServerCommandType		_serverCommandType;
		Commands::ClientCommandType		_clientCommandType;
		std::vector<std::string>		_clientArgs;
		UID								_uid;

	public:

		enum ArgumentNumber {
			SERVER,
			PASSWORD,
			SERVER_COMMAND,
			CLIENT_ARGS,
			STR_UID,
		} ;

		ServerMessage(const std::string &password,
					const Commands::ServerCommandType &serverCommandType,
					const Commands::ClientCommandType &clientCommandType,
					const std::vector<std::string> &clientArgs,
					const std::string &uid) ;

		~ServerMessage() ;

		const std::string		&getPassword() const ;

		const Commands::ServerCommandType		&getServerCommand() const ;

		const Commands::ClientCommandType		&getClientCommand() const ;

		const std::vector<std::string>			&getClientArgs() const ;

		const UID				&getUID() const ;

		const std::string		toString() const ; 

		class FewArguments : public std::exception {
			const char		*what() const throw() {
				return "Server Request don't have any arguments";
			}
		} ;

} ;


#endif