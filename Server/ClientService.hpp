/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientService.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larlena <larlena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 19:16:51 by zera              #+#    #+#             */
/*   Updated: 2022/01/20 20:25:38 by larlena          ###   ########.fr       */
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
				void	notice(std::vector<Client> &clients, const Response &response) ;
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
					//	execute to operator
				void	oper(std::vector<Client> &clients, const Response &response) ;
				void	kick(std::vector<Client> &clients, const Response &response) ;
				void	kill(std::vector<Client> &clients, const Response &response) ;
				void	die(std::vector<Client> &clients, const Response &response) ;
					//	Check execute to client
				Commands::Status	checkMessage(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkPrivateMessage(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkNotice(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkNickName(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkJoin(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkLeave(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkQuit(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkWho(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkList(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkHelp(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkRegister(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkLogin(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkCommandNotFound(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
					// Check execute to operator
				Commands::Status	checkOper(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkKick(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkKill(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkDie(std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;

			private :
					//	Utils
				void			__deleteRequest(std::vector<Client> &clients, const UID &uid) ;
				const Client	*__findeClient(const std::vector<Client> &clients, const UID &uid) ;
				Client			*__findeClient(std::vector<Client> &clients, const UID &uid) ;

		} ;

		typedef	void				(Methods::*__funcExec)(std::vector<Client> &, const Response &) ;
		typedef Commands::Status	(Methods::*__funcCheck)(std::vector<Client> &, ClientRequest *, std::vector<std::string> &) ;


		Methods					_methods;
		__funcExec				_funcExec[17];
		__funcCheck				_funcCheck[17];
		std::vector<Client>		_clients;
	
	public :
	
		ClientService(void) ;

		~ClientService(void) ;

		void			registrClient(unsigned long socket, Response *response) ;

		void			loginClient(unsigned long socket, Response *response) ;

		void			disconnectClient(unsigned long socket) ; 

		unsigned long	getIdRequest(unsigned long socket) ;

		unsigned long	getUserId(unsigned long socket) ;

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

		void	__functionPointerInit() ;

};


#endif