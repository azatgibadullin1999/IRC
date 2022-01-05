/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/03 17:31:56 by root              #+#    #+#             */
/*   Updated: 2022/01/05 14:03:01 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "models.hpp"

std::string		&strltrim(std::string &str) {
	std::string::iterator	it = str.begin();
	
	for (; it != str.end() && (isspace(*it) || !isprint(*it)); it++) { }
	str.erase(str.begin(), it);
	
	return str;
}

std::string		&strrtrim(std::string &str) {
	std::string::reverse_iterator	it = str.rbegin();

	for (; it != str.rend() && (isspace(*it) || !isprint(*it)); it++) { }
	str.erase(it.base(), str.rbegin().base());

	return str;
}

std::string		&strtrim(std::string &str) {
	return strltrim(strrtrim(str));
}
