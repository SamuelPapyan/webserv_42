/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spapyan <spapyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/01 09:59:37 by spapyan           #+#    #+#             */
/*   Updated: 2023/09/01 09:59:37 by spapyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <iostream>
# include <fcntl.h>
# include <cstring>
# include <string>
# include <unistd.h>
# include <dirent.h>
# include <sstream>
# include <cstdlib>
# include <fstream>
# include <cctype>
# include <ctime>
# include <cstdarg>

# include <map>
# include <set>
# include <vector>
# include <algorithm>
# include <iterator>
# include <list>

# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <unistd.h>
# include <signal.h>

# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/select.h>
# include <arpa/inet.h>

# include "ConfigParser.hpp"
# include "ConfigFile.hpp"
# include "ServerConfig.hpp"
# include "Location.hpp"
# include "HttpRequest.hpp"
# include "CgiHandler.hpp"
# include "Mime.hpp"
# include "Logger.hpp"

# define CONNECTION_TIMEOUT 60
# define MESSAGE_BUFFER 40000

# define MAX_URI_LENGTH 4096
# define MAX_CONTENT_LENGTH 30000000

template<typename T>
std::string toString(const T val) {
    std::stringstream stream;
    stream << val;
    return stream.str();
}

std::string     statusCodeString(short);
std::string     getErrorPage(short);
inet            buildHtmlIndex(std::string&, std::vector<uint8_t>&, size_t&);
inet            ft_stoi(std::string str);
unsigned int    fromHexToDec(const std::string& nb);

#endif