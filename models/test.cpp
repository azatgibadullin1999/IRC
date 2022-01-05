/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 21:14:13 by root              #+#    #+#             */
/*   Updated: 2022/01/05 16:28:28 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

int		main() {
	Request		r;

	r.createRequest("WHO     WEIIfew     rwer werwewr");
	std::cout << r.whichComand() << std::endl;

	std::vector<std::string>::const_iterator	it = r.getComand().begin();
}