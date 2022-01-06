/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientService.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/05 19:16:51 by zera              #+#    #+#             */
/*   Updated: 2022/01/06 19:47:18 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __CLIENTSERVICE_HPP__
# define __CLIENTSERVICE_HPP__


# include <vector>
# include "ServerModels/Client.hpp"


class ClientService
{
	public:
		ClientService(void);
		~ClientService(void);

		// Finds Client from socket and add to him request

		void	addRequest(int socket, ClientRequest *request);

		// Check number of Responses to UID from request and executes request if all responses here

		void	addResponse(ServerMessage *serverMessage);

		// Executes requested command

		// !	After this func executes request need to be deleted 	!

		void	execute(ClientRequest *request);

		// Change your nickname

		void	nickName(Client *client, std::string &nickName);

		// Join channel

		void	join(Client *client, std::string &channel);

		// Leave current channel

		void	leave(Client *client);

		// Disconnect from server

		void	quit(Client *client);

		// List of users in channel

		void	who(Client *client);

		// Send private message

		void	privateMessage(Client *sender, std::string &reciever);

		// Message

		void	message(std::string &reciever);
		
		// List channels

		void	list(Client *client);

		// Help

		void	help(Client *client);

	private:
		std::vector<Client*>			_clients;
		

};


#endif