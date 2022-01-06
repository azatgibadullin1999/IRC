/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UID.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/06 17:52:49 by root              #+#    #+#             */
/*   Updated: 2022/01/06 19:26:22 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UID_HPP__
# define __UID_HPP__


# include <string>
# include <stdlib.h>
# include "../services/utils.hpp"


class UID {
	
	private :

		unsigned long	_port;
		unsigned long	_messageId;
		unsigned long	_userId;

	public :

		UID(unsigned long port, unsigned long messageId, unsigned long userId) ;

		UID(const std::string &uid) ;

		~UID() ;

		std::string		toString() const ;

		unsigned long	getPort() const ;

		unsigned long	getMessageId() const ;

		unsigned long	getUserId() const ;

} ;


#endif