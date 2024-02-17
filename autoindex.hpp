/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 22:52:25 by asidqi            #+#    #+#             */
/*   Updated: 2024/02/17 12:08:58 by zouaraqa         ###   ########.fr       */
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
#include <dirent.h>
#include "Location.hpp"

/**
 * @brief when all default pages are not found, and autoindex is on then we generate a file
 * else we return 403 Forbidden
 */
class autoindex
{
private:
	std::string server_root;
	std::string host_name;
	int port;
	std::string linkGen(std::string root, std::string host, int p);
	autoindex();

public:
	autoindex(std::string root, std::string host, int p);
	~autoindex();
	autoindex &operator=(const autoindex &other);
	std::string pageGen();
};
