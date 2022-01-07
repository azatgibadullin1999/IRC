/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerClientService.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zera <zera@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 10:22:23 by zera              #+#    #+#             */
/*   Updated: 2022/01/06 20:09:05 by zera             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef __SERVERCLIENTSERVICE_HPP__
# define __SERVERCLIENTSERVICE_HPP__


# include <vector>
# include "ServerModels/ServerClient.hpp"


class ServerClientService
{
	public:
		ServerClientService(void);
		~ServerClientService(void);

		

	private:
		std::vector<ServerClient>		_serverClients;
};


#endif