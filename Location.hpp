/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asidqi <asidqi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 11:14:16 by asidqi            #+#    #+#             */
/*   Updated: 2024/02/20 21:22:56 by asidqi           ###   ########.fr       */
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
	std::vector<std::string>	locDirName;
	std::vector<std::string>	methods;
	std::string					default_file;
	std::string					root;
	std::string					alias;
	std::vector<std::string>	cgi_bin;
	std::vector<std::string>	cgi_extension;
	bool						lai;
	int							redirect_status;
	std::string					redirect_to_dir;
};