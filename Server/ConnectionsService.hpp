/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConnectionsService.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 13:35:05 by zera              #+#    #+#             */
/*   Updated: 2022/01/16 20:38:12 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __CONNECTIONSSERVICE_HPP__
# define __CONNECTIONSSERVICE_HPP__


# include <vector>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include "ServerModels/Connection.hpp"
# include "../models/models.hpp"


class ConnectionException : public std::exception {
	private:
		char			*_result;
	public:
		ConnectionException(const char *msg) throw() : std::exception() {
			std::string total = std::string("Socket Exception: ") + msg;
			_result = strdup(total.c_str());
		}
		virtual ~ConnectionException() throw() {
			delete(_result);
		}
		virtual const char* what() const throw() {
			return _result;
		}
};


class ConnectionsService
{
	public:
		ConnectionsService(void) { }
		~ConnectionsService(void) ;

		void						setFds(fd_set &readFds, fd_set &writeFds);

		std::string					addRequest(int socket, std::string rq);

		void						addConnection(int socket);

		int							addConnection(std::string host, int port);

		Connection::ConnectionType	getTypeConnection(int socket);

		void						setTypeConnection(int socket, Connection::ConnectionType type);

		void						disconnect(int socket);

		void						addResponse(int socket, std::string response);

	private:
		std::vector<Connection*>	_connections;

		Connection*					_getConnection(int socket);

};


#endif