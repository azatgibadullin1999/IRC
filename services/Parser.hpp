/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 13:01:26 by root              #+#    #+#             */
/*   Updated: 2022/01/14 16:38:08 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __PARSER_HPP__
# define __PARSER_HPP__


# include <string>
# include <iostream>
# include "../models/models.hpp"


class Parser {

	private :

		Commands		_commands;

	public :

		Parser() ;

		~Parser() ;

		ClientRequest		*generateClientRequest(std::string rawRequest, const UID &uid) ;

		ServerMessage		*generateServerMessage(const ClientRequest &processedReqeust, const std::string &password) ;

		ServerMessage		*generateServerMessage(const std::string &rawRequest) ;

		bool				isServerMessage(const std::string &rawRequest);

	private :

		void		__createClientRequest(
						const std::string &rawRequest,
						std::vector<std::string> &requestData) const ;

		// void		__createClientRequestByMessage(
		// 				const std::string &rawRequest,
		// 				std::vector<std::string> &requestData) const ;

		void		__createServerReqeustByServerMessage(
						const std::string &rawRequest,
						std::string &password,
						Commands::ServerCommandType &serverCommandType,
						Commands::ClientCommandType &clientCommandType,
						std::vector<std::string> &requestData,
						std::string &uid) const ;

		void		__createServerReqeustByClientRequest(
						const ClientRequest &processedReqeust,
						Commands::ServerCommandType &serverCommandType,
						Commands::ClientCommandType &clientCommandType,
						std::vector<std::string> &requestData) const ;

} ;


#endif