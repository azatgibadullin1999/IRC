/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 12:34:03 by root              #+#    #+#             */
/*   Updated: 2022/01/07 21:12:08 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_REQUEST_HPP__
# define __CLIENT_REQUEST_HPP__


# include <iostream>
# include <vector>
# include "../services/Commands.hpp"
# include "ServerMessage.hpp"
# include "UID.hpp"


class ClientRequest {

	protected :

		std::vector<std::string>		_requestData;
		UID								_uid;
		Commands::ClientCommandType		_type;
		unsigned int					_numberOfWaitResponses;
		std::vector<ServerMessage>		_responses;

	public :

		ClientRequest(const std::vector<std::string> &requestData, const Commands::ClientCommandType type, const UID &uid) ;

		~ClientRequest() ;

		bool				isMessage() const ;

		bool				isCommand() const ;

		Commands::ClientCommandType		whichCommand() const ;

		const std::string				&getCommand() const ;

		const std::vector<std::string>	&getArguments() const ;

		const std::string				&getMessage() const ;

		const UID						&getUID() const ;

		void			setNumberResponses(unsigned int num) ;

		unsigned int	addResponse(const ServerMessage &resp) ;

} ;


#endif