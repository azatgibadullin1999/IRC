/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 19:20:11 by zera              #+#    #+#             */
/*   Updated: 2022/01/06 20:07:21 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__


# include <string>


class Client
{
	public:
		enum STATE {
			READ,
			WRITE
		};

		Client(int socket) {
			_socket = socket;
			_idRequest = 0;
		}
		~Client(void) {}

		int getSocket() {
			return (_socket);
		}
	
		STATE getState() {
			return (_state);
		}
		
	private:
		std::string					_nickName;
		std::string					_chanel;
		int							_socket;
		STATE						_state;
		unsigned int				_idRequest;
		std::vector<ClientRequest>	_requests;

};

#endif