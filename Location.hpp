/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asidqi <asidqi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 11:14:16 by asidqi            #+#    #+#             */
/*   Updated: 2024/01/21 11:47:23 by asidqi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <map>
#include <cstring>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

class Location
{
public:
	std::vector<std::string>	path;
	std::vector<std::string>	methods;
	std::string					default_file;
	std::vector<std::string>	cgi_bin;
	std::vector<std::string>	cgi_extension;
};
