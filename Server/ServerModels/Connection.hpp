/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/12 14:05:54 by zera              #+#    #+#             */
/*   Updated: 2022/01/14 13:27:56 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __CONNECTION_HPP__
# define __CONNECTION_HPP__


# include <string>
# include <unistd.h>


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

};

#endif