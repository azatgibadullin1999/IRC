/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 14:39:00 by root              #+#    #+#             */
/*   Updated: 2022/01/15 21:23:43 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(unsigned long socket, const std::string &nickName, const std::string &password) :
	_socket(socket),
	_nickName(nickName),
	_password(password),
	_userId(socket),
	_chanel("Hub"),
	_idRequest(0),
	_loginStatus(true) { }

Client::~Client(void) {
	__deleteRequests();
}

unsigned long 	Client::getSocket() const {
	return _socket;
}


unsigned long	Client::getUserId() const {
	return _userId;
}

unsigned int	Client::getIdRequest() const {
	return _idRequest;
}

void	Client::addRequest(ClientRequest *request) {
	++_idRequest;
	requests.push_back(request);
}

void	Client::deleteRequest(const UID &uid) {
	std::vector<ClientRequest*>::iterator	rq = findeReqeust(uid);

	if (rq != requests.end()) {
		delete	*rq;
		requests.erase(rq);
	}
}

void	Client::addResponse(const std::string &response) {
	responses.push_back(response);
}

const std::string	&Client::getNickName() const {
	return _nickName;
}

const std::string	&Client::getPassword() const {
	return _password;
}

void				Client::setNickName(const std::string &newNickName) {
	_nickName = newNickName;
}

const std::string	&Client::getChanel() const {
	return _chanel;
}

void				Client::setChanel(const std::string &newChannel) {
	_chanel = newChannel;
}

bool				Client::isLogin() const {
	return _loginStatus;
}

void				Client::loginIn(unsigned long newSocket) {
	_loginStatus = true;
	_socket = newSocket;
}

void				Client::loginOut() {
	_loginStatus = false;
}


std::vector<ClientRequest*>::iterator	Client::findeReqeust(const UID &uid) {
	std::vector<ClientRequest*>::iterator	it = requests.begin();

	for (; it < requests.end() && (*it)->getUID() != uid; it++) { }

	return it;
}

void		Client::__deleteRequests() {
	std::vector<ClientRequest*>::iterator	it = requests.begin();

	for (; it != requests.end(); it++)
		delete *it;
}
