/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientService.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larlena <larlena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 14:24:58 by root              #+#    #+#             */
/*   Updated: 2022/01/22 19:51:00 by larlena          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientService.hpp"


ClientService::ClientService() : _methods() {
	__functionPointerInit();
	_clients.push_back(Client(1, "xXx_admin_xXx", "Admin"));
	_clients[0].becomePrivileged();
	_clients[0].loginOut();
}

ClientService::~ClientService() { }

void	ClientService::registrClient(unsigned long socket, Response *response) {
	if (response->getCommandStatus() == Commands::SUCCESS) {
		_clients.push_back(Client(socket, response->getArguments()[0], response->getArguments()[1]));
		Client *cl = __findeClient(socket, FindeSocket()).base();
		cl->addResponse(Message::toServerResponse("you have successfully registered", SuccessType()));
		_methods.__sendChannelMessage(_clients, cl, "new client on server " + ColorMessage::clientPrefix(cl->getNickName()));
	}
}

void	ClientService::loginClient(unsigned long socket, Response *response) {
	if (response->getCommandStatus() == Commands::SUCCESS) {
		Client		*cl = __findeClient(response->getArguments()[0]).base();

		cl->loginIn(socket);
		cl->addResponse(Message::toServerResponse("you have successfully logged in", SuccessType()));
		_methods.__sendChannelMessage(_clients, cl, ColorMessage::clientPrefix(cl->getNickName()) + " login on server");
	}
}

void	ClientService::disconnectClient(unsigned long socket) {
	Client	*cl = __findeClient(socket, FindeSocket()).base();

	if (cl == _clients.end().base())
		return ;
	cl->loginOut();
	_methods.__sendChannelMessage(_clients, cl, ColorMessage::clientPrefix(cl->getNickName()) + " logout from server");
}

unsigned long	ClientService::getIdRequest(unsigned long socket) {
	Client	*cl = __findeClient(socket, FindeSocket()).base();

	if (cl == _clients.end().base())
		return -1;
	return cl->getIdRequest();
}

unsigned long	ClientService::getUserId(unsigned long socket) {
	Client	*cl = __findeClient(socket, FindeSocket()).base();

	if (cl == _clients.end().base())
		return -1;
	return cl->getUserId();
}

void		ClientService::setFds(fd_set &writeFds) {
	std::vector<Client>::iterator	it = _clients.begin();

	for (; it < _clients.end(); it++) {
		if(!it->responses.empty())
			FD_SET(it->getSocket(), &writeFds);
	}
}

void		ClientService::sendResponseToClient(unsigned int socket) {
	Client		*cl = __findeClient(socket, FindeSocket()).base();

	if (cl == _clients.end().base()) {
		return ;
	}

	for (size_t	i = 0; i < cl->responses.size(); i++)
		send(socket, cl->responses[i].c_str(), cl->responses[i].size(), 0);
	cl->responses.clear();
}

Response		*ClientService::checkToExecute(ClientRequest *request) {
	std::vector<std::string>	responseArgs;
	Commands::Status			status;

	status = (_methods.*_funcCheck[request->getCommand()])(_clients, request, responseArgs);

	return new Response(responseArgs, request->getUID(), status, request->getCommand());
}

void		ClientService::execute(Response *response) {
	(_methods.*_funcExec[response->getClientCommand()])(_clients, *response);
}


	//
	// EXECUTE
	//


void	ClientService::Methods::message(
	std::vector<Client> &clients,
	const Response &response) {
		if (response.getArguments().size() < 3)
			return ;

		std::vector<Client>::iterator		dstClient = clients.begin();
		Client		*srcClient = __findeClient(clients, response.getUID());

		if (srcClient == clients.end().base())
			return ;

		for (; dstClient != clients.end(); dstClient++) {
			if (dstClient.base() != srcClient
			&& response.getArguments()[0] == dstClient->getChanel()
			&& dstClient->isLogin()) {
				dstClient->addResponse(Message::toMessage(response.getArguments()));
			}
		}
	}

void	ClientService::Methods::privateMessage(
	std::vector<Client> &clients,
	const Response &response) {
		Client		*srcClient = __findeClient(clients, response.getUID());

		if (srcClient == clients.end().base())
			return ;
		if (response.getArguments().size() < 3) {
			srcClient->addResponse(Message::toServerResponse("few arguments", FailType()));
			return ;
		}

		std::vector<std::string>				dstClient;
		__parsPrivMessageArgument(dstClient, response.getArguments()[1]);
		std::vector<Client>::iterator			clientIt;
		std::vector<std::string>::iterator		dstClientIt = dstClient.begin();

		for (; dstClientIt != dstClient.end(); dstClientIt++) {
			clientIt = clients.begin();
			for (; clientIt != clients.end(); clientIt++) {
				if (clientIt->getNickName() == *dstClientIt) {
					if (clientIt->isLogin())
						clientIt->addResponse(Message::toPrivateMessage(response.getArguments()));
					else 
						srcClient->addResponse(Message::toServerResponse("User " + ColorMessage::clientPrefix(*dstClientIt) + " is offline", FailType()));
					break ;
				}
			}
			if (clientIt == clients.end())
				srcClient->addResponse(Message::toServerResponse("User " + ColorMessage::clientPrefix(*dstClientIt) + " not found", FailType()));
		}
	}

void	ClientService::Methods::notice(
	std::vector<Client> &clients,
	const Response &response) {
		Client		*srcClient = __findeClient(clients, response.getUID());

		if (srcClient == clients.end().base())
			return ;
		if (response.getArguments().size() < 3) {
			srcClient->addResponse(Message::toServerResponse("few arguments", FailType()));
			return ;
		}

		std::vector<std::string>				dstClient;
		__parsPrivMessageArgument(dstClient, response.getArguments()[1]);
		std::vector<Client>::iterator			clientIt;
		std::vector<std::string>::iterator		dstClientIt = dstClient.begin();

		for (; dstClientIt != dstClient.end(); dstClientIt++) {
			clientIt = clients.begin();
			for (; clientIt != clients.end(); clientIt++) {
				if (clientIt->getNickName() == *dstClientIt
				&& clientIt->isLogin()) {
					clientIt->addResponse(Message::toPrivateMessage(response.getArguments()));
					break ;
				}
			}
		}
	}

void	ClientService::Methods::nickName(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl == clients.end().base())
			return ;
		if (response.getArguments().size() == 0) {
			cl->addResponse(Message::toServerResponse("few arguments", FailType()));
			return ;
		}

		if (!__checkNameToValid(response.getArguments()[0])) {
			cl->addResponse(Message::toServerResponse("forbidden symbols", FailType()));
			return ;
		}

		std::vector<Client>::iterator	it = clients.begin();

		for (; it != clients.end(); it++) {
			if (it->getNickName() == *(response.getArguments().begin())
			|| it->getLogin() == *(response.getArguments().begin())) {
				cl->addResponse(Message::toServerResponse("user with the same nickname already exists", FailType()));
				return ;
			}
			if (it->getChanel() == *(response.getArguments().begin())) {
				cl->addResponse(Message::toServerResponse("you can't change the nickname that matches the channel name", FailType()));
				return ;
			}
		}
		cl->setNickName(response.getArguments()[0]);
		cl->addResponse(Message::toServerResponse("your nickname has been changed to : " + cl->getNickName(), SuccessType()));
	}

void	ClientService::Methods::join(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl == clients.end().base())
			return ;
		if (response.getArguments().size() == 0) {
			cl->addResponse(Message::toServerResponse("few arguments", FailType()));
			return ;
		}
		std::vector<Client>::iterator		it = clients.begin();
		for (; it != clients.end(); it++) {
			if (response.getArguments()[0] == it->getNickName()
			|| response.getArguments()[0] == it->getLogin()) {
				cl->addResponse(Message::toServerResponse("you can't create a channel that matches a username", FailType()));
				return ;
			}
		}

		__sendChannelMessage(clients, cl, ColorMessage::clientPrefix(cl->getNickName()) + " leave from " + ColorMessage::channelPrefix(cl->getChanel()));
		cl->setChanel(response.getArguments()[0]);
		cl->addResponse(Message::toServerResponse("your channel has been changed to : " + ColorMessage::channelPrefix(cl->getChanel()), SuccessType()));
		std::vector<Client>::iterator	dstClient2 = clients.begin();
		__sendChannelMessage(clients, cl, ColorMessage::clientPrefix(cl->getNickName()) + " joint to " + ColorMessage::channelPrefix(cl->getChanel()));
	}

void	ClientService::Methods::leave(
	std::vector<Client> &clients,
	const Response &response) {
		Client	*cl = __findeClient(clients, response.getUID());

		if (cl == clients.end().base())
			return ;

		if (cl->getChanel() == "Hub")
			cl->addResponse(Message::toServerResponse("you cant't leave form Hub", FailType()));
		else {
			cl->addResponse(Message::toServerResponse("you leave channel : " + cl->getChanel(), SuccessType()));
			std::vector<Client>::iterator	dstClient1 = clients.begin();

			__sendChannelMessage(clients, cl, ColorMessage::clientPrefix(cl->getNickName()) + " leave from " + ColorMessage::channelPrefix(cl->getChanel()));
			cl->setChanel("Hub");
			std::vector<Client>::iterator	dstClient2 = clients.begin();
			__sendChannelMessage(clients, cl, ColorMessage::clientPrefix(cl->getNickName()) + " join to " + ColorMessage::channelPrefix(cl->getChanel()));
		}
	}

void	ClientService::Methods::quit(
	std::vector<Client> &clients,
	const Response &response) {
		Client	*cl = __findeClient(clients, response.getUID());

		if (cl == clients.end().base())
			return ;

		cl->loginOut();
	}

void	ClientService::Methods::who(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl == clients.end().base())
			return ;
		if (response.getArguments().empty()) {
			cl->addResponse(Message::toServerResponse("no such users found", FailType()));
			return ;
		}
		cl->addResponse(Message::toList(response.getArguments()));
	}

void	ClientService::Methods::list(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			if (response.getCommandStatus() == Commands::SUCCESS)
				cl->addResponse(Message::toList(response.getArguments()));
			else 
				cl->addResponse(Message::toServerResponse("no channels", FailType()));
		}
	}

void	ClientService::Methods::help(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base())
			cl->addResponse(Message::toList(response.getArguments()));
	}

void	ClientService::Methods::Register(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base())
			cl->addResponse(Message::toServerResponse("you already login in", FailType()));
	}

void	ClientService::Methods::login(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base())
			cl->addResponse(Message::toServerResponse("you already login in", FailType()));
	}

void	ClientService::Methods::commandNotFound(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base())
			cl->addResponse(Message::toServerResponse("command not found, write \"/HELP\" to check all commands, that you can use", FailType()));
	}

void	ClientService::Methods::oper(
	std::vector<Client> &clients,
	const Response &response) {
		Client	*cl = __findeClient(clients, response.getUID());

		if (cl == clients.end().base())
			return ;
		if (response.getArguments().size() == 0) {
			cl->addResponse(Message::toServerResponse("few arguments", FailType()));
			return ;
		}
		if (response.getCommandStatus() == Commands::SUCCESS) {
			cl->becomePrivileged();
			cl->addResponse(Message::toServerResponse("you become operator server", SuccessType()));
		}
		else
			cl->addResponse(Message::toServerResponse("wrong password", FailType()));
	}

void	ClientService::Methods::kick(
	std::vector<Client> &clients,
	const Response &response) {
		Client	*oper = __findeClient(clients, response.getUID());

		if (oper == clients.end().base())
			return ;
		if (!oper->isPrivileged()) {
			oper->addResponse(Message::toServerResponse("you are not operator", FailType()));
			return ;
		}
		if (response.getArguments().size() == 0) {
			oper->addResponse(Message::toServerResponse("few arguments", FailType()));
			return ;
		}

		std::vector<Client>::iterator	it = clients.begin();

		for (; it != clients.end() && it->getNickName() != response.getArguments()[0]; it++) { }

		if (it == clients.end()) {
			oper->addResponse(Message::toServerResponse("client don't found on server", FailType()));
			return ;
		}
		if (it->isPrivileged()) {
			oper->addResponse(Message::toServerResponse("you can't kick other operator", FailType()));
			return ;
		}
		if (!it->isLogin()) {
			oper->addResponse(Message::toServerResponse("client is offline", FailType()));
			return ;
		}
		if (it->getChanel() != oper->getChanel()) {
			oper->addResponse(Message::toServerResponse("you can't kick a user who is on another channel", FailType()));
			return ;
		}
		if (it->getChanel() == "Hub") {
			oper->addResponse(Message::toServerResponse("you can't kick client form Hub, but you can /KILL him", FailType()));
			return ;
		}
		it->addResponse(Message::toServerResponse("you kicked from : " + ColorMessage::channelPrefix(it->getChanel()), FailType()));
		__sendChannelMessage(clients, it.base(), ColorMessage::clientPrefix(it->getNickName()) + " kicked from channel");
		it->setChanel("Hub");
		__sendChannelMessage(clients, it.base(), ColorMessage::clientPrefix(it->getNickName()) + " join to " + ColorMessage::channelPrefix(it->getChanel()));
	}

void	ClientService::Methods::kill(
	std::vector<Client> &clients,
	const Response &response) { }

void	ClientService::Methods::die(
	std::vector<Client> &clients,
	const Response &response) {
		Client	*oper = __findeClient(clients, response.getUID());

		if (oper == clients.end().base())
			return ;
		if (!oper->isPrivileged()) {
			oper->addResponse(Message::toServerResponse("you are not operator", FailType()));
			return ;
		}
		exit(0);
	}


	//
	//	CHECK TO EXECUTE
	//


Commands::Status	ClientService::Methods::checkMessage(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		const Client	*cl = __findeClient(clients, request->getUID());

		if (cl != clients.end().base()) {
			responseArgs.push_back(cl->getChanel());
			responseArgs.push_back(cl->getNickName());
			responseArgs.insert(responseArgs.end(), request->getArguments().begin(), request->getArguments().end());
		}
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkPrivateMessage(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		const Client	*cl = __findeClient(clients, request->getUID());

		if (cl != clients.end().base()) {
			responseArgs.push_back(cl->getNickName());
			responseArgs.insert(responseArgs.end(), request->getArguments().begin(), request->getArguments().end());
		}
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkNotice(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		const Client	*cl = __findeClient(clients, request->getUID());

		if (cl != clients.end().base()) {
			responseArgs.push_back(cl->getNickName());
			responseArgs.insert(responseArgs.end(), request->getArguments().begin(), request->getArguments().end());
		}
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkNickName(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		responseArgs = request->getArguments();
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkJoin(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		responseArgs = request->getArguments();
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkLeave(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		responseArgs = request->getArguments();
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkQuit(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		responseArgs = request->getArguments();
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkWho(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::const_iterator		it = clients.begin();

		if (request->getArguments().size() == 0) {
			for (; it != clients.end(); it++) {
				responseArgs.push_back("User\t : " + ColorMessage::clientPrefix(it->getLogin()));
				responseArgs.push_back("Nick\t : " + ColorMessage::clientPrefix(it->getNickName()));
				responseArgs.push_back("Channel : " + ColorMessage::channelPrefix(it->getChanel()));
				responseArgs.push_back("Status\t : " + std::string(it->isLogin() ? "\033[38;5;28mis online\033[m" : "\033[38;5;52mis offline\033[m"));
				responseArgs.push_back(" ");
			}
		}
		else {
			for (; it != clients.end(); it++) {
				if (!it->getNickName().compare(0, request->getArguments().begin()->size(), *request->getArguments().begin())) {
					responseArgs.push_back("User\t : " + ColorMessage::clientPrefix(it->getLogin()));
					responseArgs.push_back("Nick\t : " + ColorMessage::clientPrefix(it->getNickName()));
					responseArgs.push_back("Channel : " + ColorMessage::channelPrefix(it->getChanel()));
					responseArgs.push_back("Status\t : " + std::string(it->isLogin() ? "\033[38;5;28mis online\033[m" : "\033[38;5;52mis offline\033[m"));
					responseArgs.push_back(" ");
				}
			}
		}

		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkList(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::const_iterator			it = clients.begin();
		std::vector<std::string>::iterator			itChannel;

		responseArgs.push_back(it->getChanel());
		for (; it != clients.end(); it++) {
			itChannel = responseArgs.begin();
			for (; itChannel != responseArgs.end(); itChannel++) {
				if (*itChannel == it->getChanel())
					break ;
			}
			if (itChannel == responseArgs.end())
				responseArgs.push_back(it->getChanel());
		}

		for (itChannel = responseArgs.begin(); itChannel != responseArgs.end(); itChannel++)
			*itChannel = ColorMessage::channelPrefix(*itChannel);

		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkHelp(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		responseArgs.push_back("/PRIVMSG - sending a private message. Example \"/PRIVMSG <UserNickName> <any message>\" ");
		responseArgs.push_back("/NOTICE - sending a privat message, but without error response");
		responseArgs.push_back("/NICK - change of nickname. Example \"/NICK <__xXx_PeNiS_DeTrOv_xXx__>\" ");
		responseArgs.push_back("/JOIN - change of channel. Example \"/JOIN <any_channel>\" ");
		responseArgs.push_back("/LEAVE - leave channel");
		responseArgs.push_back("/QUIT - quit form server");
		responseArgs.push_back("/WHO - finde User by");
		responseArgs.push_back("/LIST - displays all channels");
		responseArgs.push_back("/HELP - shows all server commands");
		responseArgs.push_back(" ");
		const Client	*cl = __findeClient(clients, request->getUID());
		if (cl != clients.end().base() && cl->isPrivileged()) {
			responseArgs.push_back("/KICK - you can moderate all servers, and kick any user from channel");
			responseArgs.push_back("/KILL - you can remove the user from the network");
			responseArgs.push_back("/DIE - you can shut down server");
			responseArgs.push_back(" ");
		}

		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkRegister(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::const_iterator		it = clients.begin();
		if (request->getArguments().size() < 2)
			return Commands::ERROR;

		if (!__checkNameToValid(request->getArguments()[0]))
			return Commands::FAIL;

		for (; it != clients.end(); it++) { 
			if (request->getArguments()[0] == it->getLogin()
				|| request->getArguments()[0] == it->getNickName()
				|| request->getArguments()[0] == it->getChanel())
				return Commands::FAIL;
		}

		responseArgs = request->getArguments();
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkLogin(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::const_iterator		it = clients.begin();
		if (request->getArguments().size() < 2)
			return Commands::ERROR;

		for (; it != clients.end(); it++) { 
			if (request->getArguments()[0] == it->getLogin()
				&& request->getArguments()[1] == it->getPassword()
				&& !it->isLogin()) {
				responseArgs = request->getArguments();
				return Commands::SUCCESS;
			}
		}

		return Commands::FAIL;
	}

Commands::Status	ClientService::Methods::checkCommandNotFound(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		responseArgs = request->getArguments();
		return Commands::FAIL;
	}

Commands::Status	ClientService::Methods::checkOper(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		responseArgs = request->getArguments();
		if (request->getArguments().size() < 1)
			return Commands::FAIL;
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkKick(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		responseArgs = request->getArguments();
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkKill(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		Client	*oper = __findeClient(clients, request->getUID());

		if (oper == clients.end().base())
			return Commands::FAIL;
		if (!oper->isPrivileged()) {
			oper->addResponse(Message::toServerResponse("you are not operator", FailType()));
			return Commands::FAIL;
		}
		if (request->getArguments().size() == 0) {
			oper->addResponse(Message::toServerResponse("few arguments", FailType()));
			return Commands::FAIL;
		}

		std::vector<Client>::iterator	it = clients.begin();

		for (; it != clients.end() && it->getNickName() != request->getArguments()[0]; it++) { }
		
		if (it == clients.end()) {
			oper->addResponse(Message::toServerResponse("client don't found on server", FailType()));
			return Commands::FAIL;
		}
		if (it->isPrivileged()) {
			oper->addResponse(Message::toServerResponse("you can't kill other operator", FailType()));
			return Commands::FAIL;
		}
		oper->addResponse(Message::toServerResponse(ColorMessage::clientPrefix(it->getNickName()) + " deleted from server", SuccessType()));
		responseArgs.push_back(ft::to_string(it->getSocket()));
		it->loginOut();
		__sendChannelMessage(clients, it.base(), ColorMessage::clientPrefix(it->getNickName()) + " deleted from server");
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkDie(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		responseArgs = request->getArguments();
		return Commands::SUCCESS;
	}


	//
	//	METHODS UTILS
	//


void		ClientService::Methods::__sendChannelMessage(std::vector<Client> &clients, Client *srcClient, const std::string &message) {
	std::vector<Client>::iterator	dstClient1 = clients.begin();

	for (; dstClient1 != clients.end(); dstClient1++) {
		if (dstClient1.base() != srcClient
		&& dstClient1->getChanel() == srcClient->getChanel()
		&& dstClient1->isLogin())
			dstClient1->addResponse(Message::toServerResponse(message, ChannelType()));
	}
}

void		ClientService::Methods::__parsPrivMessageArgument(std::vector<std::string> &dst, const std::string &usrs) {
		std::string::const_iterator		it2 = usrs.begin();
		std::string::const_iterator		it1;

		while (it2 < usrs.end()) {
			it1 = it2;
			for(; *it2 != ',' && it2 < usrs.end(); ++it2) { }
			dst.push_back(std::string(it1, it2));
			for(; *it2 == ',' && it2 < usrs.end(); ++it2) { }
		}
	}

bool		ClientService::Methods::__checkNameToValid(const std::string &name) {
	std::string::const_iterator		it = name.begin();

	for (; it != name.end(); it++) {
		if (!isalpha(*it) && !isdigit(*it))
			return false;
	}
	return true;
}

Client		*ClientService::Methods::__findeClient(std::vector<Client> &clients, const UID &uid) {
	std::vector<Client>::iterator		it1 = clients.begin();
		
	for (; it1 != clients.end(); it1++) {
		if (it1->getUserId() == uid.getUserId()) {
			return it1.base();
		}
	}

	return clients.end().base();
}

const Client		*ClientService::Methods::__findeClient(const std::vector<Client> &clients, const UID &uid) {
	std::vector<Client>::const_iterator		it1 = clients.begin();
		
	for (; it1 != clients.end(); it1++) {
		if (it1->getUserId() == uid.getUserId()) {
			return it1.base();
		}
	}

	return clients.end().base();
}


	//
	//	UTILS
	//


std::vector<Client>::iterator	ClientService::__findeClient(unsigned int userId, FindeUserId) {
	std::vector<Client>::iterator	it = _clients.begin();

	for (; it != _clients.end() && it->getUserId() != userId; it++) { }

	return it;

}
std::vector<Client>::iterator	ClientService::__findeClient(unsigned int socket, FindeSocket) {
	std::vector<Client>::iterator	it = _clients.begin();

	for (; it != _clients.end() && it->getSocket() != socket; it++) { }

	return it;

}

std::vector<Client>::iterator	ClientService::__findeClient(const std::string &nickName) {
	std::vector<Client>::iterator	it = _clients.begin();

	for (; it != _clients.end() && it->getNickName() != nickName; it++) { }

	return it;
}

void	ClientService::__functionPointerInit() {
	_funcExec[Commands::MESSAGE] = &Methods::message;
	_funcExec[Commands::PRIVMSG] = &Methods::privateMessage;
	_funcExec[Commands::NOTICE] = &Methods::notice;
	_funcExec[Commands::NICK] = &Methods::nickName;
	_funcExec[Commands::JOIN] = &Methods::join;
	_funcExec[Commands::LEAVE] = &Methods::leave;
	_funcExec[Commands::QUIT] = &Methods::quit;
	_funcExec[Commands::WHO] = &Methods::who;
	_funcExec[Commands::LIST] = &Methods::list;
	_funcExec[Commands::HELP] = &Methods::help;
	_funcExec[Commands::REGISTR] = &Methods::Register;
	_funcExec[Commands::LOGIN] = &Methods::login;
	_funcExec[Commands::OPER] = &Methods::oper;
	_funcExec[Commands::KICK] = &Methods::kick;
	_funcExec[Commands::KILL] = &Methods::kill;
	_funcExec[Commands::DIE] = &Methods::die;
	_funcExec[Commands::COMMAND_NOT_FOUND] = &Methods::commandNotFound;
	_funcCheck[Commands::MESSAGE] = &Methods::checkMessage;
	_funcCheck[Commands::PRIVMSG] = &Methods::checkPrivateMessage;
	_funcCheck[Commands::NOTICE] = &Methods::checkNotice;
	_funcCheck[Commands::NICK] = &Methods::checkNickName;
	_funcCheck[Commands::JOIN] = &Methods::checkJoin;
	_funcCheck[Commands::LEAVE] = &Methods::checkLeave;
	_funcCheck[Commands::QUIT] = &Methods::checkQuit;
	_funcCheck[Commands::WHO] = &Methods::checkWho;
	_funcCheck[Commands::LIST] = &Methods::checkList;
	_funcCheck[Commands::HELP] = &Methods::checkHelp;
	_funcCheck[Commands::REGISTR] = &Methods::checkRegister;
	_funcCheck[Commands::LOGIN] = &Methods::checkLogin;
	_funcCheck[Commands::OPER] = &Methods::checkOper;
	_funcCheck[Commands::KICK] = &Methods::checkKick;
	_funcCheck[Commands::KILL] = &Methods::checkKill;
	_funcCheck[Commands::DIE] = &Methods::checkDie;
	_funcCheck[Commands::COMMAND_NOT_FOUND] = &Methods::checkCommandNotFound;
}
