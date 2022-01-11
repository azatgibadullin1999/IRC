/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerSettings.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/08 19:56:19 by zera              #+#    #+#             */
/*   Updated: 2022/01/11 19:50:30 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef __SERVERSETTINGS_HPP__
# define __SERVERSETTINGS_HPP__


# include <string>
# include <string.h>
# include <stdlib.h>
# include <iostream>


class ServerSettingsException : public std::exception {
	private:
		char			*_result;
	public:
		ServerSettingsException(const char *msg) throw() : std::exception() {
			std::string total = std::string("Socket Exception: ") + msg;
			_result = strdup(total.c_str());
		}
		virtual ~ServerSettingsException() throw() {
			delete(_result);
		}
		virtual const char* what() const throw() {
			return _result;
		}
};


class ServerSettings {
	
	private :

		std::string		_port;

		std::string		_password;
		
		bool			_connection;

		std::string		_hostConnection;
		int				_portConnection;
		std::string		_passwordConnection;

		void			_parseConnection(const std::string &connection);

	public :
		std::string		getPort() const ;

		std::string		getPassword() const ;

		bool			haveConnection() const ;

		std::string		getHostConnection() const ;

		int				getPortConnection() const ;

		std::string		getPasswordConnection() const ;

		ServerSettings(const std::string port, const std::string password) ;

		ServerSettings(const std::string connection, const std::string port,
						 const std::string password) throw(ServerSettingsException) ;

		~ServerSettings() { }

} ;


#endif