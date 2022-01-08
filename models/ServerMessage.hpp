/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessage.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 20:36:13 by root              #+#    #+#             */
/*   Updated: 2022/01/08 15:49:41 by zera             ###   ########.fr       */
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

		std::vector<std::string>		_requestData;
		UID								_uid;
		Commands::ServerCommandType		_type;
		unsigned int					_waitingResponses;

	public:

		enum ArgumentNumber {
			SERVER,
			PASSWORD,
			SERVER_COMMAND,
			CLIENT_ARGS,
			STR_UID,
		} ;

		ServerMessage(const std::vector<std::string> &requestData, const Commands::ServerCommandType type) ;

		~ServerMessage() ;

		const Commands::ServerCommandType	&getType() const ;

		const std::string					&getPassword() const ;

		const std::string					&getServerCommand() const ;

		const std::string					&getClientArgs() const ;

		const UID							&getUID() const ;

		const std::string					&getStringUID() const ;

		std::vector<ServerMessage*>			&getResponses() ;

		void								setUID(UID uid) ;

		void								setWaitingResponses(int waitingResponses) ;

		bool								addResponse(ServerMessage *response) ;

		class FewArguments : public std::exception {
			const char		*what() const throw() {
				return "Server Request don't have any arguments";
			}
		} ;

	private :

		void		__checkArguments(ArgumentNumber n) const ;

		std::vector<ServerMessage*>		_responses;

} ;


#endif