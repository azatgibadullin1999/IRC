/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientService.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 14:24:58 by root              #+#    #+#             */
/*   Updated: 2022/01/18 21:38:06 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientService.hpp"


ClientService::ClientService() : _methods() {
	__functionPointerInit();
}

ClientService::~ClientService() { }

void	ClientService::registrClient(unsigned long socket, Response *response) {
	if (response->getCommandStatus() == Commands::SUCCESS_SEND) {
		_clients.push_back(Client(socket, response->getArguments()[0], response->getArguments()[1]));
		__findeClient(socket, FindeSocket())->addResponse(ColorMessage::serverPrefixSuccess() + "you have successfully registered\n");
	}
	delete response;
}

void	ClientService::loginClient(unsigned long socket, Response *response) {
	if (response->getCommandStatus() == Commands::SUCCESS_NO_SEND) {
		Client		*cl = __findeClient(response->getArguments()[0]).base();

		cl->loginIn(socket);
		cl->addResponse(ColorMessage::serverPrefixSuccess() + "you have successfully logged in\n");
	}
	delete response;
}

void	ClientService::disconnectClient(unsigned long socket) {
	Client	*cl = __findeClient(socket, FindeSocket()).base();

	if (cl != _clients.end().base()) {
		cl->loginOut();
		std::cout << cl->isLogin() << std::endl;
	}
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
	delete response;
}


	//
	// EXECUTE
	//


void	ClientService::Methods::message(
	std::vector<Client> &clients,
	const Response &response) {
		std::vector<Client>::iterator	it = clients.begin();
		Client		*clientSendMessage = __findeClient(clients, response.getUID());

		if (response.getCommandStatus() == Commands::ERROR)
			return ;

		for (; it != clients.end(); it++) {
			if (it.base() != clientSendMessage
			&& response.getArguments()[0] == it->getChanel()
			&& it->isLogin()) {
				it->addResponse(response.toMessage());
			}
		}
	}

void	ClientService::Methods::privateMessage(
	std::vector<Client> &clients,
	const Response &response) {
		std::vector<Client>::iterator	it = clients.begin();
		Client		*clientSendMessage = __findeClient(clients, response.getUID());

		if (response.getCommandStatus() == Commands::ERROR) {
			if (clientSendMessage != clients.end().base())
				clientSendMessage->addResponse(ColorMessage::serverPrefixFail() + "few arguments\n");
			return ;
		}

		if (response.getCommandStatus() == Commands::FAIL) {
			if (clientSendMessage != clients.end().base())
				clientSendMessage->addResponse(ColorMessage::serverPrefixFail() + "user not found\n");
			return ;
		}
			
		for (; it != clients.end(); it++) {
			if (it->getNickName() == response.getArguments()[1]
				&& it->isLogin())
				it->addResponse(response.toPrivateMessage());
		}
	}

void	ClientService::Methods::nickName(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			if (response.getCommandStatus() == Commands::SUCCESS_SEND) {
				cl->setNickName(response.getArguments()[0]);
				cl->addResponse(ColorMessage::serverPrefixSuccess() + "your nickname has been changed to : " + cl->getNickName() + "\n");
			}
			else if (response.getCommandStatus() == Commands::FAIL)
				cl->addResponse(ColorMessage::serverPrefixFail() + "user with the same nickname already exists\n");
			else
				cl->addResponse(ColorMessage::serverPrefixFail() + "few arguments\n");
		}
	}

void	ClientService::Methods::join(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			if (response.getCommandStatus() == Commands::SUCCESS_NO_SEND) {
				cl->setChanel(response.getArguments()[0]);
				cl->addResponse(ColorMessage::serverPrefixSuccess() + "your channel has been changed to : " + cl->getChanel() + '\n');
			}
			else
				cl->addResponse(ColorMessage::serverPrefixFail() + "change channel is faild\n");
		}
	}

void	ClientService::Methods::leave(
	std::vector<Client> &clients,
	const Response &response) {
		Client	*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			if (cl->getChanel() == "Hub")
				cl->addResponse(ColorMessage::serverPrefixFail() + "you cant't leave form Hub\n");
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
			if (response.getCommandStatus() == Commands::SUCCESS_SEND)
				cl->addResponse(response.toList());
			else
				cl->addResponse(ColorMessage::serverPrefixFail() + "no such users found\n");
		}
	}

void	ClientService::Methods::list(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			if (response.getCommandStatus() == Commands::SUCCESS_NO_SEND)
				cl->addResponse(response.toList());
			else 
				cl->addResponse(ColorMessage::serverPrefixFail() + "no channels\n");
		}
	}

void	ClientService::Methods::help(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base())
			cl->addResponse(response.toList());
	}

void	ClientService::Methods::Register(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base())
			cl->addResponse(ColorMessage::serverPrefixFail() + "you already login in\n");
	}

void	ClientService::Methods::login(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base())
			cl->addResponse(ColorMessage::serverPrefixFail() + "you already login in\n");
	}

void	ClientService::Methods::commandNotFound(
	std::vector<Client> &clients,
	const Response &response) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base())
			cl->addResponse(ColorMessage::serverPrefixFail() + "command not found, write \"/HELP\" to check all commands, that you can use\n");
	}

void	ClientService::Methods::oper(
	std::vector<Client> &clients,
	const Response &response) {
		Client	*cl = __findeClient(clients, response.getUID());

		if (cl == clients.end().base())
			return ;
		if ()
	}

void	ClientService::Methods::kick(
	std::vector<Client> &clients,
	const Response &response) {

	}

void	ClientService::Methods::kill(
	std::vector<Client> &clients,
	const Response &response) {

	}

void	ClientService::Methods::die(
	std::vector<Client> &clients,
	const Response &response) {

	}


	//
	//	CHECK TO EXECUTE
	//


Commands::Status	ClientService::Methods::checkMessage(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		if (request->getArguments().size() == 0)
			return Commands::ERROR;
		
		std::cout << request->getUID().toString() << std::endl;

		const Client	*cl = __findeClient(clients, request->getUID());
		if (cl != clients.end().base()) {
			request->setChannelAndNick(cl->getChanel(), cl->getNickName());
		}

		responseArgs = request->getArguments();
		return Commands::SUCCESS_SEND;
	}

Commands::Status	ClientService::Methods::checkPrivateMessage(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::const_iterator		it = clients.begin();

		if (request->getArguments().size() < 2)
			return Commands::ERROR;

		const Client	*cl = __findeClient(clients, request->getUID());
		if (cl != clients.end().base()) {
			request->setNick(cl->getNickName());
		}

		for (; it != clients.end(); it++) {
			if (it->getNickName() == request->getArguments()[1]) {
				responseArgs = request->getArguments();
				return Commands::SUCCESS_SEND;
			}
		}
		return Commands::FAIL;
	}

Commands::Status	ClientService::Methods::checkNickName(
	const std::vector<Client> &clients,
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
		return Commands::SUCCESS_SEND;
	}

Commands::Status	ClientService::Methods::checkJoin(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::const_iterator		it = clients.begin();

		if (request->getArguments().size() == 0)
			return Commands::ERROR;

		responseArgs.push_back(request->getArguments()[0]);

		return Commands::SUCCESS_NO_SEND;
	}

Commands::Status	ClientService::Methods::checkLeave(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		return Commands::SUCCESS_NO_SEND;
	}

Commands::Status	ClientService::Methods::checkQuit(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		return Commands::SUCCESS_NO_SEND;
	}

Commands::Status	ClientService::Methods::checkWho(
	const std::vector<Client> &clients,
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

		if (responseArgs.empty())
			return Commands::FAIL;
		else
			return Commands::SUCCESS_SEND;
	}

Commands::Status	ClientService::Methods::checkList(
	const std::vector<Client> &clients,
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

		return Commands::SUCCESS_NO_SEND;
	}

Commands::Status	ClientService::Methods::checkHelp(
	const std::vector<Client> &clients,
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

		return Commands::SUCCESS_NO_SEND;
	}

Commands::Status	ClientService::Methods::checkRegister(
	const std::vector<Client> &clients,
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
		return Commands::SUCCESS_SEND;
	}

Commands::Status	ClientService::Methods::checkLogin(
	const std::vector<Client> &clients,
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
				return Commands::SUCCESS_NO_SEND;
			}
		}

		return Commands::FAIL;
	}

Commands::Status	ClientService::Methods::checkCommandNotFound(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		return Commands::FAIL;
	}

Commands::Status	ClientService::Methods::checkOper(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		const Client	*cl = __findeClient(clients, request->getUID());

		if (cl == clients.end().base() || request->getArguments().size() < 1)
			return Commands::ERROR;
		
	}

Commands::Status	ClientService::Methods::checkKick(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		const Client	*cl = __findeClient(clients, request->getUID());

		if (cl == clients.end().base())
			return Commands::ERROR;
		else if (!cl->isPrivileged())
			return Commands::FAIL;
		else
			return Commands::SUCCESS_NO_SEND;
	}

Commands::Status	ClientService::Methods::checkKill(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		const Client	*cl = __findeClient(clients, request->getUID());

		if (cl == clients.end().base())
			return Commands::ERROR;
		else if (!cl->isPrivileged())
			return Commands::FAIL;
		else
			return Commands::SUCCESS_NO_SEND;
	}

Commands::Status	ClientService::Methods::checkDie(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		const Client	*cl = __findeClient(clients, request->getUID());

		if (cl == clients.end().base())
			return Commands::ERROR;
		else if (!cl->isPrivileged())
			return Commands::FAIL;
		else
			return Commands::SUCCESS_NO_SEND;
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
	_funcExec[2] = &Methods::nickName;
	_funcExec[3] = &Methods::join;
	_funcExec[4] = &Methods::leave;
	_funcExec[5] = &Methods::quit;
	_funcExec[6] = &Methods::who;
	_funcExec[7] = &Methods::list;
	_funcExec[8] = &Methods::help;
	_funcExec[9] = &Methods::Register;
	_funcExec[10] = &Methods::login;
	_funcExec[11] = &Methods::commandNotFound;
	_funcExec[12] = &Methods::oper;
	_funcExec[13] = &Methods::kick;
	_funcExec[14] = &Methods::kill;
	_funcExec[15] = &Methods::die;
	_funcCheck[0] = &Methods::checkMessage;
	_funcCheck[1] = &Methods::checkPrivateMessage;
	_funcCheck[2] = &Methods::checkNickName;
	_funcCheck[3] = &Methods::checkJoin;
	_funcCheck[4] = &Methods::checkLeave;
	_funcCheck[5] = &Methods::checkQuit;
	_funcCheck[6] = &Methods::checkWho;
	_funcCheck[7] = &Methods::checkList;
	_funcCheck[8] = &Methods::checkHelp;
	_funcCheck[9] = &Methods::checkRegister;
	_funcCheck[10] = &Methods::checkLogin;
	_funcCheck[11] = &Methods::checkCommandNotFound;
	_funcCheck[12] = &Methods::checkOper;
	_funcCheck[12] = &Methods::checkKick;
	_funcCheck[12] = &Methods::checkKill;
	_funcCheck[12] = &Methods::checkDie;
}
