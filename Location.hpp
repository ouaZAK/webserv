/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 11:14:16 by asidqi            #+#    #+#             */
/*   Updated: 2024/01/30 15:50:17 by zouaraqa         ###   ########.fr       */
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
	bool						lai;
};
