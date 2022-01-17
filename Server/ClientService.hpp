/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientService.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 19:16:51 by zera              #+#    #+#             */
/*   Updated: 2022/01/17 15:53:29 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __CLIENTSERVICE_HPP__
# define __CLIENTSERVICE_HPP__


# include <vector>
# include <sys/socket.h>
# include <unistd.h>
# include <utility>
# include "ServerModels/Client.hpp"
# include "../models/ClientRequest.hpp"
# include "../models/ServerMessage.hpp"
# include "../models/Response.hpp"

class Methods;


class ClientService
{

	private :

		struct FindeSocket { } ;

		struct FindeUserId { } ;
	
		class Methods {

			public :
					//	execute to client
				void	message(std::vector<Client> &clients, const Response &response) ;
				void	privateMessage(std::vector<Client> &clients, const Response &response) ;
				void	nickName(std::vector<Client> &clients, const Response &response) ;
				void	join(std::vector<Client> &clients, const Response &response) ;
				void	leave(std::vector<Client> &clients, const Response &response) ;
				void	quit(std::vector<Client> &clients, const Response &response) ;
				void	who(std::vector<Client> &clients, const Response &response) ;
				void	list(std::vector<Client> &clients, const Response &response) ;
				void	help(std::vector<Client> &clients, const Response &response) ;
				void	Register(std::vector<Client> &clients, const Response &response) ;
				void	login(std::vector<Client> &clients, const Response &response) ;
				void	commandNotFound(std::vector<Client> &clients, const Response &response) ;
					//	Check execute to client
				Commands::Status	checkMessage(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkPrivateMessage(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkNickName(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkJoin(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkLeave(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkQuit(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkWho(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkList(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkHelp(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkRegister(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkLogin(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkCommandNotFound(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;

			private :
					//	Utils
				void			__deleteRequest(std::vector<Client> &clients, const UID &uid) ;
				const Client	*__findeClient(const std::vector<Client> &clients, const UID &uid) ;
				Client			*__findeClient(std::vector<Client> &clients, const UID &uid) ;

		} ;

		typedef	void				(Methods::*__funcExec)(std::vector<Client> &, const Response &) ;
		typedef Commands::Status	(Methods::*__funcCheck)(const std::vector<Client> &, ClientRequest *, std::vector<std::string> &) ;


		Methods					_methods;
		__funcExec				_funcExec[12];
		__funcCheck				_funcCheck[12];
		std::vector<Client>		_clients;
	
	public :
	
		ClientService(void) ;

		~ClientService(void) ;

		void			registrClient(unsigned long socket, Response *response) ;

		void			loginClient(unsigned long socket, Response *response) ;

		void			disconnectClient(unsigned long socket) ; 

		unsigned long	getIdRequest(unsigned long socket) ;

		void			addRequest(int socket, ClientRequest *request) ;

		void			setFds(fd_set &writeFds) ;

		void        	sendResponse(int socket) ;

		void			sendResponseToClient(unsigned int socket) ;

		Response		*checkToExecute(ClientRequest *request) ;

		void			execute(Response *response) ;

	private:

		std::vector<Client>::iterator	__findeClient(unsigned int userId, FindeUserId) ;

		std::vector<Client>::iterator	__findeClient(unsigned int socket, FindeSocket) ;

		std::vector<Client>::iterator	__findeClient(const std::string &nickName) ;

};


#endif