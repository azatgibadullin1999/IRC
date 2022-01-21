/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larlena <larlena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 17:31:56 by root              #+#    #+#             */
/*   Updated: 2022/01/20 15:15:51 by larlena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "utils.hpp"


std::string		&ft::strltrim(std::string &str) {
	std::string::iterator	it = str.begin();
	
	for (; it != str.end() && (isspace(*it) || !isprint(*it)); it++) { }
	str.erase(str.begin(), it);
	
	return str;
}

std::string		&ft::strrtrim(std::string &str) {
	std::string::reverse_iterator	it = str.rbegin();

	for (; it != str.rend() && (isspace(*it) || !isprint(*it)); it++) { }
	str.erase(it.base(), str.rbegin().base());

	return str;
}

std::string		&ft::strtrim(std::string &str) {
	return ft::strltrim(ft::strrtrim(str));
}

std::string		ft::to_string(unsigned long value) {
	std::string		dst;
	unsigned int	base = 10;

	if (value == 0)
		return "0";
	while (value != 0) {
		dst.insert(dst.begin(),'0' + (value % base));
		value /= base;
	}

	return dst;
}

const std::string	Message::toServerResponse(const std::string &message, SuccessType) {
	return ColorMessage::serverPrefixSuccess() + message + '\n';
}

const std::string	Message::toServerResponse(const std::string &message, FailType) {
	return ColorMessage::serverPrefixFail() + message + '\n';
}

const std::string		Message::toList(const std::vector<std::string> &args) {
	std::vector<std::string>::const_iterator	it = args.begin();
	std::string		dst;

	dst += ColorMessage::serverPrefixSuccess() + *it++ + '\n';
	for (; it != args.end(); it++)
		dst += "\t " + *it + '\n';

	return dst;
}

const std::string		Message::toMessage(const std::vector<std::string> &args) {
	std::vector<std::string>::const_iterator	it = args.begin();
	std::string		dst;

	dst += '[' + ColorMessage::channelPrefix(*it++) + ']';
	dst += '[' + ColorMessage::clientPrefix(*it++) + "] ";
	for (; it != args.end(); it++)
		dst += *it + ' ';
	dst += '\n';

	return dst;
}

const std::string		Message::toPrivateMessage(const std::vector<std::string> &args) {
	std::vector<std::string>::const_iterator	it = args.begin();
	std::string		dst;

	dst += '[' + ColorMessage::clientPrefix(*it++) + "] ";
	++it;
	for (; it != args.end(); it++)
		dst += *it + ' ';
	dst += '\n';

	return dst;
}

const std::string	ColorMessage::serverPrefixSuccess() {
	return "\033[48;5;22m[SERVER]\033[m ";
}

const std::string	ColorMessage::serverPrefixFail() {
	return "\033[48;5;52m[SERVER]\033[m ";
}

const std::string	ColorMessage::clientPrefix(std::string nickname) {
	unsigned long	color = 0;

	for (size_t i = 0; i < nickname.size(); i++)
		color += nickname[i];
	color %= 229;
	++color;

	nickname.insert(0, "\033[38;5;" + ft::to_string(color) + 'm');
	nickname.insert(nickname.size(), "\033[m");
	return nickname;
}

const std::string	ColorMessage::channelPrefix(std::string channel) {
	unsigned long	color = 0;

	for (size_t i = 0; i < channel.size(); i++)
		color -= channel[i];
	color %= 229;
	++color;

	channel.insert(0, "\033[38;5;" + ft::to_string(color) + 'm');
	channel.insert(channel.size(), "\033[m");
	return channel;
}

