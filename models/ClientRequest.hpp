/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larlena <larlena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 12:34:03 by root              #+#    #+#             */
/*   Updated: 2022/01/21 14:54:50 by larlena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_REQUEST_HPP__
# define __CLIENT_REQUEST_HPP__


# include <iostream>
# include <vector>
# include "../services/Commands.hpp"
# include "UID.hpp"


class ClientRequest {

	protected :

		std::vector<std::string>		_requestData;
		UID								_uid;
		Commands::ClientCommandType		_type;

	public :

		ClientRequest(const std::vector<std::string> &requestData, const Commands::ClientCommandType type, const UID &uid) ;

		~ClientRequest() ;

		bool				isMessage() const ;

		bool				isCommand() const ;

		Commands::ClientCommandType		getCommand() const ;

		const std::vector<std::string>	&getArguments() const ;

		std::string						getMessage() const ;

		const UID						&getUID() const ;

} ;


#endif