/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 16:18:03 by root              #+#    #+#             */
/*   Updated: 2022/01/12 14:57:00 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(const std::vector<std::string> &requestData,
					const UID &uid,
					const Commands::Status &status,
					const Commands::ClientCommandType &clientCommand) :
						_requestData(requestData),
						_uid(uid),
						_status(status),
						_clientCommand(clientCommand) { }

const UID		&Response::getUID() const {
	return _uid;
}

Commands::Status	Response::getCommandStatus() const {
	return _status;
}

Commands::ClientCommandType		Response::getClientCommand() const {
	return _clientCommand;
}

const std::vector<std::string>	&Response::getArguments() const {
	return _requestData;
}
