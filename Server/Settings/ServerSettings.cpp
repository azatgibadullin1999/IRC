/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSettings.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/10 11:41:00 by zera              #+#    #+#             */
/*   Updated: 2022/01/21 01:39:36 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerSettings.hpp"

ServerSettings::ServerSettings(const std::string port, const std::string password) {
	_connection = false;
	_port = port;
	_password = password;
}

ServerSettings::ServerSettings(const std::string connection, const std::string port,
					const std::string password) throw(ServerSettingsException) {
	_connection = true;
	_parseConnection(connection);
	_port = port;
	_password = password;
}

void			ServerSettings::_parseConnection(const std::string & connection) {
	 int count = 0;
	for (unsigned long i = 0; (i = connection.find(':', i)) != std::string::npos; i++) {
		count++;
	}
	if (count != 2) {
		throw (ServerSettingsException("Connection must have 3 args separated by ':'"));
	}
	if (*connection.begin() != '[')
		throw (ServerSettingsException("Connection must begin with ["));
	if (*(--connection.end()) != ']')
		throw (ServerSettingsException("Connection must ends with ] "));
	size_t hostEnd = connection.find(':');
	_hostConnection.insert(0, &connection[1], hostEnd - 1);

	size_t portStart = hostEnd + 1;
	size_t portEnd = connection.find(':', portStart);
	_portConnection = atoll(&connection[portStart]);

	size_t passwordStart = portEnd + 1;
	size_t passwordEnd = connection.find(']');
	_passwordConnection.insert(0, &connection[passwordStart], passwordEnd - passwordStart);
}

std::string		ServerSettings::getPort() const {
	return _port;
}

std::string		ServerSettings::getPassword() const {
	return _password;
}

bool			ServerSettings::haveConnection() const {
	return _connection;
}

std::string		ServerSettings::getHostConnection() const {
	return _hostConnection;
}

int				ServerSettings::getPortConnection() const {
	return _portConnection;
}

std::string		ServerSettings::getPasswordConnection() const {
	return _passwordConnection;
}