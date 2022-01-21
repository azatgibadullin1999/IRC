/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larlena <larlena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 19:20:11 by zera              #+#    #+#             */
/*   Updated: 2022/01/21 14:37:30 by larlena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__


# include <string>
# include "../../models/ClientRequest.hpp"
# include "../../services/utils.hpp"


class Client
{

	private :

		std::string		_nickName;

		std::string		_login;
		std::string		_password;

		std::string		_chanel;
		int				_socket;
		unsigned long	_userId;
		unsigned int	_idRequest;
		bool			_loginStatus;
		bool			_operatorStatus;
		bool			_privileged;


	public :

		std::vector<std::string>	responses;

		Client(unsigned long socket, const std::string &login, const std::string &password) ;

		~Client(void) ;

		int				getSocket() const ;

		unsigned long	getUserId() const ;

		unsigned int	getIdRequest() ;

		const std::string	&getNickName() const ;

		void				setNickName(const std::string &newNickName) ;

		const std::string	&getLogin() const ;

		const std::string	&getPassword() const ;

		const std::string	&getChanel() const ;

		void				setChanel(const std::string &newChannel) ;

		bool				isLogin() const ;

		void				loginIn(unsigned long newSocket) ;

		void				loginOut() ;

		bool				isPrivileged() const ;

		void				becomePrivileged() ;

		void	addResponse(const std::string &response) ;

	private :


} ;


#endif