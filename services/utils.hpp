/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 22:19:02 by root              #+#    #+#             */
/*   Updated: 2022/01/17 14:15:28 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_HPP__
# define __UTILS_HPP__


# include <iostream>
# include <string>


namespace ft {

std::string		&strltrim(std::string &str) ;
std::string		&strrtrim(std::string &str) ;
std::string		&strtrim(std::string &str) ;
std::string		to_string(unsigned long value) ;

}

class ColorMessage {
	public :
		static const std::string	serverPrefixSuccess() ;
		static const std::string	serverPrefixFail() ;
		static const std::string	clientPrefix(std::string nickname) ;
		static const std::string	channelPrefix(std::string channel) ;
} ;


#endif