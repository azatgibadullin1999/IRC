/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 12:34:03 by root              #+#    #+#             */
/*   Updated: 2022/01/03 14:18:17 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __REQUEST_HPP__
# define __REQUEST_HPP__


# include <string>


class Request {

	private :

		std::string		_author;
		std::string		_message;

	public :

		Request(const std::string &author, const std::string &message) :
			_author(author),
			_message(message) { }

		const std::string	&getAuthor() const {
			return _author;
		}

		const std::string	&getMessage() const {
			return _message;
		}

} ;


#endif