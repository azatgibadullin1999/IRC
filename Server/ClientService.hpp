/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientService.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 19:16:51 by zera              #+#    #+#             */
/*   Updated: 2022/01/15 22:24:41 by root             ###   ########.fr       */
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
				void	message(std::vector<Client> &clients, const Response &response, std::vector<int> &retSockets) ;
				void	privateMessage(std::vector<Client> &clients, const Response &response, std::vector<int> &retSockets) ;
				void	nickName(std::vector<Client> &clients, const Response &response, std::vector<int> &retSockets) ;
				void	join(std::vector<Client> &clients, const Response &response, std::vector<int> &retSockets) ;
				void	leave(std::vector<Client> &clients, const Response &response, std::vector<int> &retSockets) ;
				void	quit(std::vector<Client> &clients, const Response &response, std::vector<int> &retSockets) ;
				void	who(std::vector<Client> &clients, const Response &response, std::vector<int> &retSockets) ;
				void	list(std::vector<Client> &clients, const Response &response, std::vector<int> &retSockets) ;
				void	help(std::vector<Client> &clients, const Response &response, std::vector<int> &retSockets) ;
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
				Commands::Status	checkRegistr(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
				Commands::Status	checkLogin(const std::vector<Client> &clients, ClientRequest *request, std::vector<std::string> &responseArgs) ;
			private :
					//	Utils
				const Client		*__findeClient(const std::vector<Client> &clients, const UID &uid) ;
				Client				*__findeClient(std::vector<Client> &clients, const UID &uid) ;

		} ;

		typedef	void				(Methods::*__funcExec)(std::vector<Client> &, const Response &, std::vector<int> &) ;
		typedef Commands::Status	(Methods::*__funcCheck)(const std::vector<Client> &, ClientRequest *, std::vector<std::string> &) ;


		Methods					_methods;
		__funcExec				_funcExec[9];
		__funcCheck				_funcCheck[11];
		std::vector<Client>		_clients;
	
	public :
	
		ClientService(void);

		~ClientService(void);

		Client			&registrClient(unsigned long socket, Response *response) ;

		Client			&loginClient(unsigned long socket, Response *response) ;

		void			addRequest(int socket, ClientRequest *request) ;

		void			setFds(fd_set &writeFds) ;

		void        	sendResponse(int socket) ;

		void			sendResponseToClient(unsigned int socket);

		Response		*checkToExecute(ClientRequest *request);

		std::vector<int>	execute(Response *response);

	private:

		std::vector<Client>::iterator	__findeClient(unsigned int userId, FindeUserId) ;

		std::vector<Client>::iterator	__findeClient(unsigned int socket, FindeSocket) ;

		std::vector<Client>::iterator	__findeClient(const std::string &nickName) ;

};


#endif