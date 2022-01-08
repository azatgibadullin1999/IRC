/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 19:19:48 by zera              #+#    #+#             */
/*   Updated: 2022/01/08 11:53:15 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __SERVERCLIENT_HPP__
# define __SERVERCLIENT_HPP__


# include <vector>
# include "Client.hpp"
# include "../../models/ServerMessage.hpp"


class ServerClient
{
	public:
		ServerClient(Client client) {
			_socket = client.getSocket();
		}
		~ServerClient(void);

		int								getSocket() { return: _socket; }

		std::vector<ServerMessage*>&	getRequests() { return: _requests; }

	private:
		int							_socket;
		std::vector<ServerMessage*>	_requests;
};

#endif