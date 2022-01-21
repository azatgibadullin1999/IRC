/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 12:34:29 by root              #+#    #+#             */
/*   Updated: 2022/01/19 23:50:17 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__


# include <iostream>
# include <vector>
# include "UID.hpp"
# include "../services/Commands.hpp"
# include "../services/utils.hpp"


class Response {

	private :

		std::vector<std::string>		_requestData;
		UID								_uid;
		Commands::Status				_status;
		Commands::ClientCommandType		_clientCommand;

	public :

		Response(const std::vector<std::string> &requestData,
				const UID &uid,
				const Commands::Status &status,
				const Commands::ClientCommandType &clientCommand) ;

		~Response() ;

		const UID		&getUID() const ;

		Commands::Status	getCommandStatus() const ;

		Commands::ClientCommandType	getClientCommand() const ;

		const std::vector<std::string>	&getArguments() const ;

		void			setClientCommand(Commands::ClientCommandType type);
		
		std::string		toString() const ;

} ;


#endif