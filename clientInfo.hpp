/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clientInfo.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zouaraqa <zouaraqa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 09:05:40 by zouaraqa          #+#    #+#             */
/*   Updated: 2024/01/27 10:15:05 by zouaraqa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

class clientInfo
{
	private:
		std::string reqFull;
		std::string reqChunk;
		std::string root;
		
	public:
		void	setReqFull(std::string cnt);
		std::string getReqFull() const;
		void	setReqChunk(std::string cnt);
		std::string getReqChunk() const;
		void	reqChunckClear();
		std::string getRoot();
		void	setRoot(std::string str);
};