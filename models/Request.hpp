/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 12:34:03 by root              #+#    #+#             */
/*   Updated: 2022/01/05 16:26:05 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__


# include <iostream>
# include <vector>
# include "models.hpp"
# include "Commands.hpp"
# include "utils.hpp"


class Request {

	private :

		enum Status {
			MESSAGE = 0,
			COMMAND = 1,
		} ;

		static Commands				_commands;
		std::vector<std::string>	_requestData;
		Commands::CommandType		_status;

	public :

		Request() ;

		~Request() ;

		bool				isMessage() const ;

		bool				isComand() const ;

		Commands::CommandType	whichComand() const ;

		const std::vector<std::string>	&getComand() const ;

		const std::string				&getMessage() const ;

		Request		&createRequest(std::string rawRequest) ;

	private :

		void		__createRequestByCommand(std::string &rawRequest) ;

		void		__createRequestByMessage(std::string &rawRequest) ;

} ;


#endif