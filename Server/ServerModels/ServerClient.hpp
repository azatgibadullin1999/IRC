/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 19:19:48 by zera              #+#    #+#             */
/*   Updated: 2022/01/14 15:56:07 by zera             ###   ########.fr       */
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

		std::vector<ServerMessage*>&	getRequests() { return _requests; }

	private:
		int							_socket;
		std::vector<ServerMessage*>	_requests;
};

#endif