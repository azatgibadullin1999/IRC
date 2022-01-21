/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientService.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: larlena <larlena@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 14:24:58 by root              #+#    #+#             */
/*   Updated: 2022/01/21 16:40:52 by larlena          ###   ########.fr       */
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

		std::cout << response.toString() << std::endl;

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
		std::vector<Client>::iterator	dstClient = clients.begin();
		Client		*srcClient = __findeClient(clients, response.getUID());

		if (srcClient == clients.end().base())
			return ;
		if (response.getArguments().size() < 2)
			srcClient->addResponse(Message::toServerResponse("few arguments", FailType()));

		for (; dstClient != clients.end(); dstClient++) {
			if (dstClient->getNickName() == response.getArguments()[0]) {
				if (dstClient->isLogin())
					dstClient->addResponse(Message::toPrivateMessage(response.getArguments()));
				else 
					srcClient->addResponse(Message::toServerResponse("User is ofline", FailType()));
				return ;
			}
		}
		
		srcClient->addResponse(Message::toServerResponse("User not found", FailType()));
	}

void	ClientService::Methods::notice(
	std::vector<Client> &clients,
	const Response &response) {
		std::vector<Client>::iterator	dstClient = clients.begin();
		Client		*srcClient = __findeClient(clients, response.getUID());

		if (srcClient == clients.end().base())
			return ;
		if (response.getArguments().size() < 2)
			srcClient->addResponse(Message::toServerResponse("few arguments", FailType()));

		for (; dstClient != clients.end(); dstClient++) {
			if (dstClient->getNickName() == response.getArguments()[0]
			&& dstClient->isLogin()) {
				dstClient->addResponse(Message::toPrivateMessage(response.getArguments()));
				return ;
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

		std::vector<Client>::iterator	it = clients.begin();

		for (; it != clients.end(); it++) {
			if (it->getNickName() == *(response.getArguments().begin())) {
				cl->addResponse(Message::toServerResponse("user with the same nickname already exists", FailType()));
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

		cl->setChanel(response.getArguments()[0]);
		cl->addResponse(Message::toServerResponse("your channel has been changed to : " + cl->getChanel(), SuccessType()));
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
			cl->setChanel("Hub");
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
		if (it->isLogin()) {
			oper->addResponse(Message::toServerResponse("client is ofline", FailType()));
			return ;
		}
		if (it->getChanel() == "Hub") {
			oper->addResponse(Message::toServerResponse("you can't kick client form Hub, but you can /KILL him", FailType()));
			return ;
		}
		if (it->getChanel() != oper->getChanel()) {
			oper->addResponse(Message::toServerResponse("you can't kick a user who is on another channel", FailType()));
			return ;
		}
		it->addResponse(Message::toServerResponse("you kicked from : " + it->getChanel(), FailType()));
		oper->addResponse(Message::toServerResponse("you kick : " + it->getNickName() + "; from : " + it->getChanel(), SuccessType()));
		it->setChanel("Hub");
	}

void	ClientService::Methods::kill(
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
		oper->addResponse(Message::toServerResponse("client deleted from server", SuccessType()));
		it->loginOut();
	}

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
			responseArgs.insert(responseArgs.begin(), request->getArguments().begin(), request->getArguments().end());
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
			responseArgs.insert(responseArgs.begin(), request->getArguments().begin(), request->getArguments().end());
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
		responseArgs = request->getArguments();
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
		responseArgs = request->getArguments();
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
