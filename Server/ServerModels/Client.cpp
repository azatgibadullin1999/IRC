/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/07 14:39:00 by root              #+#    #+#             */
/*   Updated: 2022/01/21 01:42:37 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(unsigned long socket, const std::string &login, const std::string &password) :
	_nickName(login),
	_login(login),
	_password(password),
	_chanel("Hub"),
	_socket(socket),
	_userId(socket),
	_idRequest(0),
	_loginStatus(true),
	_privileged(false) { }

Client::~Client(void) {
	__deleteRequests();
}

int		Client::getSocket() const {
	return _socket;
}


unsigned long	Client::getUserId() const {
	return _userId;
}

unsigned int	Client::getIdRequest() {
	return ++_idRequest;
}

void	Client::addRequest(ClientRequest *request) {
	requests.push_back(request);
}

void	Client::deleteRequest(const UID &uid) {
	std::vector<ClientRequest*>::iterator	rq = findeReqeust(uid);

	if (rq != requests.end()) {
		// delete	*rq;
		requests.erase(rq);
	}
}

void	Client::addResponse(const std::string &response) {
	responses.push_back(response);
}

const std::string	&Client::getNickName() const {
	return _nickName;
}

const std::string	&Client::getLogin() const  {
	return _login;
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
	_privileged = false;
	_socket = -1;
}

bool				Client::isPrivileged() const {
	return _privileged;
}

void				Client::becomePrivileged() {
	_privileged = true;
}

std::vector<ClientRequest*>::iterator	Client::findeReqeust(const UID &uid) {
	std::vector<ClientRequest*>::iterator	it = requests.begin();

	for (; it < requests.end() && (*it)->getUID() != uid; it++) { }

	return it;
}

void		Client::__deleteRequests() {
	std::vector<ClientRequest*>::iterator	it = requests.begin();

	// for (; it != requests.end(); it++)
	// 	delete *it;
}
