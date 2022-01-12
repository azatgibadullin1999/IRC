/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 16:18:03 by root              #+#    #+#             */
/*   Updated: 2022/01/10 18:26:57 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(const std::vector<std::string> &requestData,
					const UID &uid,
					const Commands::Status &status) :
						_requestData(requestData),
						_uid(uid),
						_status(status) { }

const UID		&Response::getUID() const {
	return _uid;
}

Commands::Status	Response::getCommandStatus() const {
	return _status;
}

const std::vector<std::string>	&Response::getArguments() const {
	return _requestData;
}
