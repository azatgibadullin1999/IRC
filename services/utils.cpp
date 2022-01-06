/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 17:31:56 by root              #+#    #+#             */
/*   Updated: 2022/01/06 15:13:24 by root             ###   ########.fr       */
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
