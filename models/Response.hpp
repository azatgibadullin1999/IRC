/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 12:34:29 by root              #+#    #+#             */
/*   Updated: 2022/01/10 19:21:07 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__


# include <iostream>
# include <vector>
# include "UID.hpp"
# include "../services/Commands.hpp"


class Response {

	private :

		std::vector<std::string>		_requestData;
		UID								_uid;
		Commands::Status				_status;

	public :

		Response(const std::vector<std::string> &requestData,
				const UID &uid,
				const Commands::Status &status) ;

		~Response() ;

		const UID		&getUID() const ;

		Commands::Status	getCommandStatus() const ;

		const std::vector<std::string>	&getArguments() const ;

} ;


#endif