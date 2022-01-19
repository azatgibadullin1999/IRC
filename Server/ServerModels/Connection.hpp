/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 14:05:54 by zera              #+#    #+#             */
/*   Updated: 2022/01/18 22:36:28 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __CONNECTION_HPP__
# define __CONNECTION_HPP__


# include <string>
# include <unistd.h>
# include <vector>
# include "../../models/ClientRequest.hpp"
# include "../../models/ServerMessage.hpp"


class Connection
{
	public:
		enum ConnectionType {
			NONE,
			CLIENT,
			SERVER
		};
		Connection(int socket) {
			_socket = socket;
			_type = NONE;
		}

		~Connection(void) {
			close(_socket);
		}

		int					getSocket() { return _socket; }
		ConnectionType		getType() { return _type; }
		std::string			getRawRq() { return _rawRq; }
		ClientRequest		*getClientRequest() { return _clientRq; }
		ServerMessage		*getServerMessage() { return _serverRq; }
		void				setClientRequest(ClientRequest *clientRequest) { _clientRq = clientRequest; }
		void				setServerMessage(ServerMessage *serverMessage) { _serverRq = serverMessage; }
		std::vector<std::string>	&getResponses() { return _responses; }
		Connection*			setType(ConnectionType type) {
			_type = type;
			return this;
		}
		Connection*			setRawRq(std::string rawRq) {
			_rawRq = rawRq;
			return this;
		}


	private:
		int					_socket;
		ConnectionType		_type;
		std::string			_rawRq;
		ClientRequest		*_clientRq;
		ServerMessage		*_serverRq;
		std::vector<std::string>	_responses;

};

#endif