/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ClientService.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 14:24:58 by root              #+#    #+#             */
/*   Updated: 2022/01/16 13:30:41 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ClientService.hpp"


ClientService::ClientService() : _methods() {
	_funcExec[0] = &Methods::message;
	_funcExec[1] = &Methods::privateMessage;
	_funcExec[2] = &Methods::nickName;
	_funcExec[3] = &Methods::join;
	_funcExec[4] = &Methods::leave;
	_funcExec[5] = &Methods::quit;
	_funcExec[6] = &Methods::who;
	_funcExec[7] = &Methods::list;
	_funcExec[8] = &Methods::help;
	_funcCheck[0] = &Methods::checkMessage;
	_funcCheck[1] = &Methods::checkPrivateMessage;
	_funcCheck[2] = &Methods::checkNickName;
	_funcCheck[3] = &Methods::checkJoin;
	_funcCheck[4] = &Methods::checkLeave;
	_funcCheck[5] = &Methods::checkQuit;
	_funcCheck[6] = &Methods::checkWho;
	_funcCheck[7] = &Methods::checkList;
	_funcCheck[8] = &Methods::checkHelp;
	_funcCheck[9] = &Methods::checkRegistr;
	_funcCheck[10] = &Methods::checkLogin;
}

ClientService::~ClientService() { }

Client	&ClientService::registrClient(unsigned long socket, Response *response) {
	if (response->getCommandStatus() == Commands::SUCCESS)
		_clients.push_back(Client(socket, response->getArguments()[0], response->getArguments()[1]));
}

Client	&ClientService::loginClient(unsigned long socket, Response *response) {
	if (response->getCommandStatus() == Commands::FAIL)
		__findeClient(response->getArguments()[0])->loginIn(socket);
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

// void		ClientService::sendResponse(int socket) {
// 	std::vector<ServerMessage> *toSend = serverClient->getMsgToSend();

// 	while (toSend->size() != 0) {
// 		ServerMessage *rq = *(toSend->rbegin());
// 		send(socket, rq->toString().c_str(), rq->toString().length(), 0);
// 		toSend->erase(--toSend->end());
// 		delete (rq);
// 	}
// }

void		ClientService::sendResponseToClient(unsigned int socket) {
	Client		*cl = __findeClient(socket, FindeSocket()).base();

	if (cl == _clients.end().base()) {
		return ;
	}

	for (size_t	i = 0; i < cl->responses.size(); i++)
		write(1, cl->responses[i].c_str(), cl->responses[i].size());
	cl->responses.clear();
}

Response		*ClientService::checkToExecute(ClientRequest *request) {
	std::vector<std::string>	responseArgs;
	Commands::Status			status;

	status = (_methods.*_funcCheck[request->getCommand()])(_clients, request, responseArgs);

	return new Response(responseArgs, request->getUID(), status, request->getCommand());
}

std::vector<int>	ClientService::execute(Response *response) {
	std::vector<int>	retSockets;

	(_methods.*_funcExec[response->getClientCommand()])(_clients, *response, retSockets);

	return retSockets;
}


	//
	// EXECUTE
	//


void	ClientService::Methods::message(
	std::vector<Client> &clients,
	const Response &response,
	std::vector<int> &retSockets) {
		std::vector<Client>::iterator	it = clients.begin();
		const Client		*clientSendMessage = __findeClient(clients, response.getUID());

		if (response.getCommandStatus() == Commands::ERROR)
			return ;

		for (; it != clients.end(); it++) {
			if (it.base() != clientSendMessage
			&& clientSendMessage->getChanel() == it->getChanel()) {
				it->addResponse(response.toMessage());
				retSockets.push_back(it->getSocket());
			}
		}
	}

void	ClientService::Methods::privateMessage(
	std::vector<Client> &clients,
	const Response &response,
	std::vector<int> &retSockets) {
		std::vector<Client>::iterator	it = clients.begin();

		if (response.getCommandStatus() == Commands::ERROR)
			return ;

		for (; it != clients.end(); it++) {
			if (it->getNickName() == response.getArguments()[1]) {
				it->addResponse(response.toPrivateMessage());
				retSockets.push_back(it->getSocket());
			}
		}
	}

void	ClientService::Methods::nickName(
	std::vector<Client> &clients,
	const Response &response,
	std::vector<int> &retSockets) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			if (response.getCommandStatus() == Commands::SUCCESS) {
				cl->setNickName(response.getArguments()[0]);
				cl->addResponse("[SERVER] your nickname has been changed to : " + cl->getNickName() + "\n");
			}
			else {
				cl->addResponse("[SERVER] change nickname is faild\n");
			}
			retSockets.push_back(cl->getSocket());
		}
	}

void	ClientService::Methods::join(
	std::vector<Client> &clients,
	const Response &response,
	std::vector<int> &retSockets) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			if (response.getCommandStatus() == Commands::SUCCESS) {
				cl->setChanel(response.getArguments()[0]);
				cl->addResponse("[SERVER] your channel has been changed to : " + cl->getChanel() + "\n");
			}
			else {
				cl->addResponse("[SERVER] change channel is faild\n");
			}
			retSockets.push_back(cl->getSocket());
		}
	}

void	ClientService::Methods::leave(
	std::vector<Client> &clients,
	const Response &response,
	std::vector<int> &retSockets) {
		
	}

void	ClientService::Methods::quit(
	std::vector<Client> &clients,
	const Response &response,
	std::vector<int> &retSockets) {
		
	}

void	ClientService::Methods::who(
	std::vector<Client> &clients,
	const Response &response,
	std::vector<int> &retSockets) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			if (response.getCommandStatus() == Commands::SUCCESS) {
				cl->addResponse(response.toList());
			}
			else {
				cl->addResponse("[SERVER] no such users found\n");
			}
			retSockets.push_back(cl->getSocket());
		}
	}

void	ClientService::Methods::list(
	std::vector<Client> &clients,
	const Response &response,
	std::vector<int> &retSockets) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			if (response.getCommandStatus() == Commands::SUCCESS) {
				cl->addResponse(response.toList());
			}
			else {
				cl->addResponse("[SERVER] no channels\n");
			}
			retSockets.push_back(cl->getSocket());
		}
	}

void	ClientService::Methods::help(
	std::vector<Client> &clients,
	const Response &response,
	std::vector<int> &retSockets) {
		Client			*cl = __findeClient(clients, response.getUID());

		if (cl != clients.end().base()) {
			cl->addResponse(response.toList());
			retSockets.push_back(cl->getSocket());
		}
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

		const Client	*cl = __findeClient(clients, request->getUID());
		if (cl != clients.end().base()) {
			request->setChannelAndNick(cl->getChanel(), cl->getNickName());
		}

		responseArgs = request->getArguments();
		return Commands::SUCCESS;
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
			if (it->getNickName() == *(request->getArguments().begin() + 1))
				responseArgs = request->getArguments();
				return Commands::SUCCESS;
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
		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkJoin(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::const_iterator		it = clients.begin();

		if (request->getArguments().size() == 0)
			return Commands::ERROR;

		responseArgs.push_back(request->getArguments()[0]);

		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkLeave(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		return Commands::FAIL;
	}

Commands::Status	ClientService::Methods::checkQuit(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		return Commands::FAIL;
	}

Commands::Status	ClientService::Methods::checkWho(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::const_iterator		it = clients.begin();

		if (request->getArguments().size() == 0)
			return Commands::ERROR;

		for (; it != clients.end(); it++) {
			if (!it->getNickName().compare(0, request->getArguments().begin()->size(), *request->getArguments().begin()))
				responseArgs.push_back(it->getNickName());
		}

		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkList(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::const_iterator		it1 = clients.begin();

		for (; it1 != clients.end(); it1++) {
			responseArgs.push_back(it1->getChanel());
		}

		return Commands::SUCCESS;
	}

Commands::Status	ClientService::Methods::checkHelp(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		responseArgs.push_back("PRIVMSG - sending a private message . Example \"PRIVMSG UserNickName sperma \" ");
		responseArgs.push_back("NICK - change of nickname . Example \"NICK __xXx_PeNiS_DeTrOv_xXx__\" ");
		responseArgs.push_back("JOIN - change of channel . Example \"JOIN any_channel\" ");
		responseArgs.push_back("LEAVE - leave channel");
		responseArgs.push_back("QUIT - quit form server");
		responseArgs.push_back("WHO - finde User by");
		responseArgs.push_back("LIST - displays all channels");
		responseArgs.push_back("HELP - shows all server commands");

		return Commands::FAIL;
	}

Commands::Status	ClientService::Methods::checkRegistr(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::const_iterator		it = clients.begin();
		if (request->getArguments().size() < 2)
			return Commands::ERROR;

		for (; it != clients.end(); it++) { 
			if (request->getArguments()[0] == it->getNickName()) {
				return Commands::FAIL;
			}
		}

		responseArgs = request->getArguments();
		return Commands::SUCCESS;
}

Commands::Status	ClientService::Methods::checkLogin(
	const std::vector<Client> &clients,
	ClientRequest *request,
	std::vector<std::string> &responseArgs) {
		std::vector<Client>::const_iterator		it = clients.begin();
		if (request->getArguments().size() < 2)
			return Commands::ERROR;

		for (; it != clients.end(); it++) { 
			if (request->getArguments()[0] == it->getNickName()
				&& request->getArguments()[1] == it->getPassword()
				&& !it->isLogin()) {
				responseArgs = request->getArguments();
				return Commands::FAIL;
			}
		}

		return Commands::ERROR;
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
