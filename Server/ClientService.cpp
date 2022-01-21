/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientService.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larlena <larlena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 14:24:58 by root              #+#    #+#             */
/*   Updated: 2022/01/20 21:06:30 by larlena          ###   ########.fr       */
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
		__findeClient(socket, FindeSocket())->addResponse(ColorMessage::serverPrefixSuccess() + "you have successfully registered\n");
	}
}

void	ClientService::loginClient(unsigned long socket, Response *response) {
	if (response->getCommandStatus() == Commands::SUCCESS) {
		Client		*cl = __findeClient(response->getArguments()[0]).base();

		cl->loginIn(socket);
		cl->addResponse(ColorMessage::serverPrefixSuccess() + "you have successfully logged in\n");
	}
}

void	ClientService::disconnectClient(unsigned long socket) {
	Client	*cl = __findeClient(socket, FindeSocket()).base();

	if (cl != _clients.end().base())
		cl->loginOut();
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

void	ClientService::addRequest(int socket, ClientRequest *request) {
	std::vector<Client>::iterator	it = _clients.begin();

	for (; it != _clients.end(); it++) {
		if (it->getSocket() == socket) {
			it->addRequest(request);
			return ;
		}
	}
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

	Client *cl = __findeClient(response->getUID().getUserId(), FindeUserId()).base();
	if (cl == _clients.end().base())
		cl->deleteRequest(response->getUID());
}


	//
	// EXECUTE
	//


void	ClientService::Methods::message(
	std::vector<Client> &clients,
	const Response &response) {
		return ;
	}

void	ClientService::Methods::privateMessage(
	std::vector<Client> &clients,
	const Response &response) {
		std::vector<Client>::iterator	it = clients.begin();
		Client		*clientSendMessage = __findeClient(clients, response.getUID());

		if (response.getCommandStatus() == Commands::ERROR) {
			if (clientSendMessage != clients.end().base())
				clientSendMessage->addResponse(Message::toServerResponse("few arguments", FailType()));
			return ;
		}

		if (response.getCommandStatus() == Commands::FAIL) {
			if (clientSendMessage != clients.end().base())
				clientSendMessage->addResponse(Message::toServerResponse("user not found", FailType()));
			return ;
		}
	}

void	ClientService::Methods::notice(
	std::vector<Client> &clients,
	const Response &response) {
		Client		*clientSendMessage = __findeClient(clients, response.getUID());
		
		if (response.getCommandStatus() == Commands::ERROR) {
			if (clientSendMessage != clients.end().base())
				clientSendMessage->addResponse(Message::toServerResponse("few arguments", FailType()));
			return ;
		}
	}

void	ClientService::Methods::nickName(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			if (response.getCommandStatus() == Commands::SUCCESS) {
				cl->setNickName(response.getArguments()[0]);
				cl->addResponse(ColorMessage::serverPrefixSuccess() + "your nickname has been changed to : " + cl->getNickName() + "\n");
			}
			else if (response.getCommandStatus() == Commands::FAIL)
				cl->addResponse(Message::toServerResponse("user with the same nickname already exists", FailType()));
			else
				cl->addResponse(Message::toServerResponse("few arguments", FailType()));
		}
	}

void	ClientService::Methods::join(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			if (response.getCommandStatus() == Commands::SUCCESS) {
				cl->setChanel(response.getArguments()[0]);
				cl->addResponse(ColorMessage::serverPrefixSuccess() + "your channel has been changed to : " + cl->getChanel() + '\n');
			}
		}
	}

void	ClientService::Methods::leave(
	std::vector<Client> &clients,
	const Response &response) {
		Client	*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			if (cl->getChanel() == "Hub")
				cl->addResponse(Message::toServerResponse("you cant't leave form Hub", FailType()));
			else {
				cl->addResponse(ColorMessage::serverPrefixSuccess() + "you leave channel : " + cl->getChanel() + '\n');
				cl->setChanel("Hub");
			}
		}
	}

void	ClientService::Methods::quit(
	std::vector<Client> &clients,
	const Response &response) {
		Client	*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			cl->loginOut();
		}
	}

void	ClientService::Methods::who(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			if (response.getCommandStatus() == Commands::SUCCESS)
				cl->addResponse(Message::toList(response.getArguments()));
			else
				cl->addResponse(Message::toServerResponse("no such users found", FailType()));
		}
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
		if (response.getCommandStatus() == Commands::SUCCESS_NO_SEND) {
			cl->becomePrivileged();
			cl->addResponse(ColorMessage::serverPrefixSuccess() + "you become operator server\n");
		}
		else if (response.getCommandStatus() == Commands::FAIL)
			cl->addResponse(Message::toServerResponse("few arguments or wrong password", FailType()));
	}

void	ClientService::Methods::kick(
	std::vector<Client> &clients,
	const Response &response) {
		Client	*cl = __findeClient(clients, response.getUID());

		if (cl == clients.end().base())
			return ;
		if (response.getCommandStatus() == Commands::SUCCESS_NO_SEND) {
			std::vector<Client>::iterator	it = clients.begin();

			for (; it != clients.end() && it->getNickName() != response.getArguments()[0]; it++) { }

			if (it == clients.end() || it->isLogin()) {
				cl->addResponse(Message::toServerResponse("client don't found on server ", FailType()));
			}
			else {
				if (it->getChanel() == "Hub") {
					cl->addResponse(Message::toServerResponse("you can't kick client form Hub, but you can /KILL him", FailType()));
				}
				else {
					it->addResponse(Message::toServerResponse("you kicked from : " + it->getChanel(), FailType()));
					cl->addResponse(Message::toServerResponse("you kick : " + it->getNickName() + "; from : " + it->getChanel(), SuccessType()));
					it->setChanel("Hub");
				}
			}
		}
	}

void	ClientService::Methods::kill(
	std::vector<Client> &clients,
	const Response &response) {
		Client	*cl = __findeClient(clients, response.getUID());

		if (cl == clients.end().base())
			return ;
		if (response.getCommandStatus() == Commands::SUCCESS_NO_SEND) {
			std::vector<Client>::iterator	it = clients.begin();

			for (; it != clients.end() && it->getNickName() != response.getArguments()[0]; it++) { }

			if (it == clients.end()) {
				cl->addResponse(Message::toServerResponse("client don't found on server", FailType()));
			}
			else {
				cl->addResponse(Message::toServerResponse("client deleted from server", SuccessType()));
				it->loginOut();
			}
		}
	}

void	ClientService::Methods::die(
	std::vector<Client> &clients,
	const Response &response) {
		if (response.getCommandStatus() == Commands::SUCCESS_NO_SEND)
			exit(0);
	}


	//
	//	CHECK TO EXECUTE
	//


Commands::Status	ClientService::Methods::checkMessage(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		if (request->getArguments().size() == 0)
			return Commands::ERROR;

		const Client	*cl = __findeClient(clients, request->getUID());
		if (cl != clients.end().base()) {
			request->setChannelAndNick(cl->getChanel(), cl->getNickName());
		}

		std::vector<Client>::iterator		it = clients.begin();

		for (; it != clients.end(); it++) {
			if (it.base() != cl
			&& request->getArguments()[0] == it->getChanel()
			&& it->isLogin()) {
				it->addResponse(Message::toMessage(request->getArguments()));
			}
		}
		responseArgs = request->getArguments();
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkPrivateMessage(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::iterator		it = clients.begin();

		if (request->getArguments().size() < 2)
			return Commands::ERROR;

		const Client	*cl = __findeClient(clients, request->getUID());
		if (cl != clients.end().base()) {
			request->setNick(cl->getNickName());
		}

		for (; it != clients.end(); it++) {
			if (it->getNickName() == request->getArguments()[1]
				&& it->isLogin()) {
				it->addResponse(Message::toPrivateMessage(request->getArguments()));
				return Commands::SUCCESS_NO_SEND;
			}
		}
		return Commands::FAIL;
	}

Commands::Status	ClientService::Methods::checkNotice(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::iterator		it = clients.begin();

		if (request->getArguments().size() < 2)
			return Commands::ERROR;

		const Client	*cl = __findeClient(clients, request->getUID());
		if (cl != clients.end().base()) {
			request->setNick(cl->getNickName());
		}

		for (; it != clients.end(); it++) {
			if (it->getNickName() == request->getArguments()[1]
				&& it->isLogin()) {
				it->addResponse(Message::toPrivateMessage(request->getArguments()));
				return Commands::SUCCESS_NO_SEND;
			}
		}
		return Commands::FAIL;
	}

Commands::Status	ClientService::Methods::checkNickName(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::const_iterator		it = clients.begin();

		if (request->getArguments().size() == 0)
			return Commands::ERROR;

		for (; it != clients.end(); it++) {
			if (it->getNickName() == *request->getArguments().begin())
				return Commands::FAIL;
		}
		responseArgs.push_back(request->getArguments()[0]);
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkJoin(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::const_iterator		it = clients.begin();

		if (request->getArguments().size() == 0)
			return Commands::ERROR;

		responseArgs.push_back(request->getArguments()[0]);

		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkLeave(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkQuit(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
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

		return Commands::SUCCESS;;
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

		for (; it != clients.end(); it++) { 
			if (request->getArguments()[0] == it->getLogin()
				|| request->getArguments()[0] == it->getNickName())
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
		return Commands::FAIL;
	}

Commands::Status	ClientService::Methods::checkOper(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		const Client	*cl = __findeClient(clients, request->getUID());

		if (cl == clients.end().base())
			return Commands::ERROR;
		else if (request->getArguments().size() < 1)
			return Commands::FAIL;
		responseArgs = request->getArguments();
		return Commands::SUCCESS_NO_SEND;
	}

Commands::Status	ClientService::Methods::checkKick(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		const Client	*cl = __findeClient(clients, request->getUID());

		if (cl == clients.end().base() || !cl->isPrivileged())
			return Commands::ERROR;
		else if (request->getArguments().size() < 1)
			return Commands::FAIL;
		responseArgs = request->getArguments();
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkKill(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		const Client	*cl = __findeClient(clients, request->getUID());

		if (cl == clients.end().base() || !cl->isPrivileged())
			return Commands::ERROR;
		else if (request->getArguments().size() < 1)
			return Commands::FAIL;
		responseArgs = request->getArguments();
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkDie(
	std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		const Client	*cl = __findeClient(clients, request->getUID());

		if (cl == clients.end().base() || !cl->isPrivileged())
			return Commands::ERROR;
		else if (request->getArguments().size() < 1)
			return Commands::FAIL;
		return Commands::SUCCESS;
	}


	//
	//	METHODS UTILS
	//


Client		*ClientService::Methods::__findeClient(std::vector<Client> &clients, const UID &uid) {
	std::vector<Client>::iterator		it1 = clients.begin();
		
	for (; it1 != clients.end(); it1++) {
		if (it1->getUserId() == uid.getUserId()) {
			std::vector<ClientRequest*>::const_iterator		it2 = it1->requests.begin();
			
			for (; it2 != it1->requests.end() && (*it2)->getUID() != uid; it2++) { }

			if (it2 == it1->requests.end())
				return clients.end().base();
			return it1.base();
		}
	}

	return clients.end().base();
}

const Client		*ClientService::Methods::__findeClient(const std::vector<Client> &clients, const UID &uid) {
	std::vector<Client>::const_iterator		it1 = clients.begin();
		
	for (; it1 != clients.end(); it1++) {
		if (it1->getUserId() == uid.getUserId()) {
			std::vector<ClientRequest*>::const_iterator		it2 = it1->requests.begin();
			
			for (; it2 != it1->requests.end() && (*it2)->getUID() != uid; it2++) { }
	
			if (it2 == it1->requests.end())
				return clients.end().base();
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
	_funcExec[0] = &Methods::message;
	_funcExec[1] = &Methods::privateMessage;
	_funcExec[2] = &Methods::notice;
	_funcExec[3] = &Methods::nickName;
	_funcExec[4] = &Methods::join;
	_funcExec[5] = &Methods::leave;
	_funcExec[6] = &Methods::quit;
	_funcExec[7] = &Methods::who;
	_funcExec[8] = &Methods::list;
	_funcExec[9] = &Methods::help;
	_funcExec[10] = &Methods::Register;
	_funcExec[11] = &Methods::login;
	_funcExec[12] = &Methods::oper;
	_funcExec[13] = &Methods::kick;
	_funcExec[14] = &Methods::kill;
	_funcExec[15] = &Methods::die;
	_funcExec[16] = &Methods::commandNotFound;
	_funcCheck[0] = &Methods::checkMessage;
	_funcCheck[1] = &Methods::checkPrivateMessage;
	_funcCheck[2] = &Methods::checkNotice;
	_funcCheck[3] = &Methods::checkNickName;
	_funcCheck[4] = &Methods::checkJoin;
	_funcCheck[5] = &Methods::checkLeave;
	_funcCheck[6] = &Methods::checkQuit;
	_funcCheck[7] = &Methods::checkWho;
	_funcCheck[8] = &Methods::checkList;
	_funcCheck[9] = &Methods::checkHelp;
	_funcCheck[10] = &Methods::checkRegister;
	_funcCheck[11] = &Methods::checkLogin;
	_funcCheck[12] = &Methods::checkOper;
	_funcCheck[13] = &Methods::checkKick;
	_funcCheck[14] = &Methods::checkKill;
	_funcCheck[15] = &Methods::checkDie;
	_funcCheck[16] = &Methods::checkCommandNotFound;
}
