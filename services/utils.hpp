/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larlena <larlena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 22:19:02 by root              #+#    #+#             */
/*   Updated: 2022/01/22 13:54:36 by larlena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_HPP__
# define __UTILS_HPP__


# include <iostream>
# include <string>
# include <vector>


namespace ft {

std::string		&strltrim(std::string &str) ;
std::string		&strrtrim(std::string &str) ;
std::string		&strtrim(std::string &str) ;
std::string		to_string(unsigned long value) ;

}

struct SuccessType { } ;
struct FailType { } ;
struct ChannelType { } ;

class Message {
	public :
		static const std::string	toServerResponse(const std::string &resp, SuccessType);
		static const std::string	toServerResponse(const std::string &resp, FailType);
		static const std::string	toServerResponse(const std::string &resp, ChannelType);
		static const std::string	toString(const std::vector<std::string> &args);
		static const std::string	toList(const std::vector<std::string> &args);
		static const std::string	toMessage(const std::vector<std::string> &args);
		static const std::string	toPrivateMessage(const std::vector<std::string> &args);
} ;

class ColorMessage {
	public :
		static const std::string	serverPrefixSuccess() ;
		static const std::string	serverPrefixFail() ;
		static const std::string	serverPrefixChannel() ;
		static const std::string	privateMessagePrefix() ;
		static const std::string	clientPrefix(const std::string &nickname) ;
		static const std::string	channelPrefix(const std::string &channel) ;
} ;


#endif