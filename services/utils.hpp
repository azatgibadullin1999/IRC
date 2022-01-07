/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 22:19:02 by root              #+#    #+#             */
/*   Updated: 2022/01/06 15:05:49 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __UTILS_HPP__
# define __UTILS_HPP__


# include <iostream>
# include <string>


namespace ft {

std::string		&strltrim(std::string &str) ;
std::string		&strrtrim(std::string &str) ;
std::string		&strtrim(std::string &str) ;
std::string		to_string(unsigned long value) ;

}


#endif