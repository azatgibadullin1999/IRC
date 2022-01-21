/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 13:01:26 by root              #+#    #+#             */
/*   Updated: 2022/01/21 01:28:35 by zera             ###   ########.fr       */
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

	private :

		void		__createClientRequest(
						const std::string &rawRequest,
						std::vector<std::string> &requestData) const ;

} ;


#endif