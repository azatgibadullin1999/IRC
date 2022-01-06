/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UID.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 17:58:52 by root              #+#    #+#             */
/*   Updated: 2022/01/06 19:35:31 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UID.hpp"

UID::UID(unsigned long port, unsigned long messageId, unsigned long userId) :
	_port(port), _messageId(messageId), _userId(userId) { }

UID::UID(const std::string &uid) :
	_port( atoll(uid.c_str()) ),
	_messageId( atoll(uid.substr(uid.find_first_of(':') + 1).c_str()) ),
	_userId( atoll(uid.substr(uid.find_last_of(':') + 1).c_str()) ) { }

UID::~UID() { }

std::string		UID::toString() const {
	return std::string(ft::to_string(_port) + ":" + ft::to_string(_messageId) + ":" + ft::to_string(_userId));
}

unsigned long	UID::getPort() const {
	return _port;
}

unsigned long	UID::getMessageId() const {
	return _messageId;
}

unsigned long	UID::getUserId() const {
	return _userId;
}
