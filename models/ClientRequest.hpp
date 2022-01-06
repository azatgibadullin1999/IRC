/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 12:34:03 by root              #+#    #+#             */
/*   Updated: 2022/01/06 16:48:58 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __CLIENT_REQUEST_HPP__
# define __CLIENT_REQUEST_HPP__


# include <iostream>
# include <vector>
# include "../services/Commands.hpp"


class ClientRequest {

	protected :

		std::vector<std::string>		_requestData;
		std::string						_UID;
		Commands::ClientCommandType		_type;

	public :

		ClientRequest(const std::vector<std::string> &requestData, const Commands::ClientCommandType type, const std::string &UID) ;

		~ClientRequest() ;

		bool				isMessage() const ;

		bool				isComand() const ;

		Commands::ClientCommandType		whichComand() const ;

		const std::string				&getComand() const ;

		const std::vector<std::string>	&getArguments() const ;

		const std::string				&getMessage() const ;

		const std::string				&getUID() const ;

} ;


#endif