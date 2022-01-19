/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessage.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 20:36:13 by root              #+#    #+#             */
/*   Updated: 2022/01/18 16:45:39 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_REQUEST_HPP__
# define __SERVER_REQUEST_HPP__


# include <vector>
# include <string>
# include <exception>
# include "../services/Commands.hpp"
# include "Response.hpp"
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
		Commands::Status				_status;
		std::vector<std::string>		_clientArgs;
		UID								_uid;

		unsigned int					_numberOfWaitResponses;
		std::vector<ServerMessage *>	_response;
		Response						*_clientServiceResponse;


	public:

		enum ArgumentNumber {
			SERVER,
			PASSWORD,
			SERVER_COMMAND,
			CLIENT_ARGS,
			STR_UID,
		} ;

		ServerMessage(const Commands::ServerCommandType &serverCommandType,
					const Commands::ClientCommandType &clientCommandType,
					const std::vector<std::string> &clientArgs,
					const std::string &uid) ;
		
		ServerMessage(const std::string &password,
					const Commands::ServerCommandType &serverCommandType,
					const Commands::ClientCommandType &clientCommandType,
					const std::vector<std::string> &clientArgs,
					const std::string &uid) ;
		
		ServerMessage(const std::string &password,
					const Commands::ServerCommandType &serverCommandType,
					const Commands::Status &status,
					const std::vector<std::string> &clientArgs,
					const std::string &uid) ;

		ServerMessage(const std::string &passwordToConnect,
					const Commands::ServerCommandType &command,
					const std::string &ourPassword) ;

		~ServerMessage() ;

		const std::string		&getPassword() const ;

		void					setPassword(const std::string &password) ;			

		const Commands::ServerCommandType		&getServerCommand() const ;

		const Commands::ClientCommandType		&getClientCommand() const ;

		const Commands::Status					&getStatus() const ;

		const std::vector<std::string>			&getClientArgs() const ;

		std::vector<ServerMessage *>			&getResponses() ;

		const UID							&getUID() const ;

		Response							*getClientServiceResponse() { return _clientServiceResponse; }

		void								setClientServiceResponse(Response *response) { _clientServiceResponse = response; }

		bool		isServerConnectCommand() const ;

		const std::string					toString() const ;

		void		setNumberOfWaitResponses(unsigned int num) ;

		unsigned int						getNumberOfWaitResponses() ;

		bool		addResponse(ServerMessage *response) ;

		class FewArguments : public std::exception {
			const char		*what() const throw() {
				return "Server Request don't have any arguments";
			}
		} ;
} ;


#endif