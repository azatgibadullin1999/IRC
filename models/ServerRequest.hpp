/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerRequest.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 20:36:13 by root              #+#    #+#             */
/*   Updated: 2022/01/06 19:49:36 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __SERVER_REQUEST_HPP__
# define __SERVER_REQUEST_HPP__


# include <vector>
# include <string>
# include "../services/Commands.hpp"


class ServerRequest {

	private:

		/*
		SERVER password CONNECT RESPONSE
		SERVER password CONNECT REQUEST

		SERVER password RESPONSE [***] UID
		SERVER password REQUEST [***] UID

		SERVER password REQUEST [ 1 Azat ] UID
		*/

		std::vector<std::string>		_requestData;
		Commands::ServerCommandType		_type;

	public:

		enum ArgumentNumber {
			SERVER,
			PASSWORD,
			SERVER_COMMAND,
			CLIENT_ARGS,
			UID,
		} ;

		ServerRequest(const std::vector<std::string> &requestData, const Commands::ServerCommandType type) ;

		~ServerRequest() ;

		const std::string		&getPassword() const ;

		const std::string		&getServerCommand() const ;

		const std::string		&getClientArgs() const ;

		const std::string		&getUID() const ;

} ;


#endif