/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 19:20:11 by zera              #+#    #+#             */
/*   Updated: 2022/01/17 15:49:28 by root             ###   ########.fr       */
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
		unsigned long	_socket;
		unsigned long	_userId;
		unsigned int	_idRequest;
		bool			_loginStatus;
		bool			_operatorStatus;


	public :

		std::vector<ClientRequest*>	requests;
		std::vector<std::string>	responses;

		Client(unsigned long socket, const std::string &login, const std::string &password) ;

		~Client(void) ;

		unsigned long	getSocket() const ;

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

		void	addRequest(ClientRequest *request) ;

		void	deleteRequest(const UID &uid);

		void	addResponse(const std::string &response) ;

		std::vector<ClientRequest*>::iterator	findeReqeust(const UID &uid) ;

	private :

		void	__deleteRequests();

} ;


#endif