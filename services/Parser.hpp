/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 13:01:26 by root              #+#    #+#             */
/*   Updated: 2022/01/06 17:51:44 by root             ###   ########.fr       */
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

		ClientRequest		*generateClientRequest(std::string rawRequest, const std::string &UID) ;

		ServerRequest		*generateServerRequest(std::string rawRequest) ;

		ServerRequest		*generateServerRequest(const ClientRequest &processedReqeust) ;

	private :

		void		__createClientRequestByCommand(const std::string &rawRequest, std::vector<std::string> &requestData) const ;

		void		__createClientRequestByMessage(const std::string &rawRequest, std::vector<std::string> &requestData) const ;

		void		__createServerReqeustByServerRequest(const std::string &rawRequest, std::vector<std::string> &requestData) const ;

		void		__createServerReqeustByClientRequest(const ClientRequest &processedReqeust, std::vector<std::string> &requestData) const ;

} ;


#endif