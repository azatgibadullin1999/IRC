/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 19:19:48 by zera              #+#    #+#             */
/*   Updated: 2022/01/15 22:00:55 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __SERVERCLIENT_HPP__
# define __SERVERCLIENT_HPP__


# include <vector>
# include "../../models/ServerMessage.hpp"


class ServerClient
{
	public:
		ServerClient(int socket) {
			_socket = socket;
		}
		~ServerClient(void) { }

		int								getSocket() { return _socket; }

		std::vector<ServerMessage*>*	getRequests() { return &_requests; }

		std::vector<ServerMessage*>*	getMsgToSend() { return &_msgToSend; }

		std::string						getPassword() { return _password; }

		void							setPassword(std::string password) { _password = password; }
	private:
		int							_socket;
		std::vector<ServerMessage*>	_requests;
		std::vector<ServerMessage*>	_msgToSend;
		std::string					_password;
};

#endif