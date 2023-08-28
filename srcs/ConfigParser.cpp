/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spapyan <spapyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 10:01:55 by spapyan           #+#    #+#             */
/*   Updated: 2023/08/27 10:01:55 by spapyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigParser.hpp"

ConfigParser::ConfigParser() {
    _nb_server = 0;
}

ConfigParser::~ConfigParser() {}

int ConfigParser::print() {
    std::cout << "----------- Config -----------" << std::endl;
    for (size_t i = 0; i < _servers.size(); i++) {
        std::cout << "Server #" << i + 1 << std::endl;
        std::cout << "Server name: " << _servers[i].getServerName() << std::endl;
        std::cout << "Host: " << _servers[i].getHost() << std::endl;
        std::cout << "Root: " << _servers[i].getRoot() << std::endl;
        std::cout << "Index: " << _servers[i].getIndex() << std::endl;
        std::cout << "Port: " << _servers[i].getPort() << std::endl;
        std::cout << "Max BSize: " << _servers[i].getClientMaxBodySize() << std::endl;
        std::cout << "Error pages: " << _servers[i].getErrorPages().size() << std::endl;
        std::map<short, std::string>::const_iterator it = _servers[i].getErrorPages().begin();
        while (it != _servers[i].getErrorPages().end()) {
            std::cout << it->first << " - " << it->second << std::endl;
            ++it;
        }
        std::cout << "Locations: " << _servers[i].getLocations().size() << std::endl;
        std::vector<Location>::const_iterator itl = _servers[i].getLocations().begin();
        while (itl != _servers[i].getLocations().end()) {
            std::cout << "name location: " << itl->getPath() << std::endl;
            std::cout << "methods: " << itl->getPrintMethods() << std::endl;
            std::cout << "index: " << itl->getIndexLocation() << std::endl;
            if (itl->getCgiPath().empty()) {
                std::cout << "root: " << itl->getRootLocation() << std::endl;
                if (!itl->getReturn().empty())
                    std::cout << "return: " << itl->getReturn() << std::endl;
                if (!itl->getAlias().empty())
                    std::cout << "alias: " << itl->getAlias() << std::endl;
            }
            else {
                std::cout << "cgi root: " << itl->getRootLocation() << std::endl;
                std::cout << "cgi_path: " << itl->getCgiPath().size() << std::endl;
                std::cout << "cgi_ext: " << itl->getCgiExtension().size() << std::endl;
            }
            ++itl;
        }
        itl = _servers[i].getLocations().begin();
        std::cout << "--------------------------" << std::endl;
    }
    return 0;
}

int ConfigParser::createCluster(const std::string &config_file) {
    std::string content;
    ConfigFile  file(config_file);

    if (file.getTypePath(file.getPath()) != 1)
        throw ErrorException("File is invalid");
    if (file.checkFile(file.getPath(), 4) == -1)
        throw ErrorException("File is not accessible");
    content = file.readFile(config_file);
    if (content.empty())
        throw ErrorException("File is empty");
    removeComments(content);
    removeWhiteSpace(content);
    splitServers(content);
    if (_server_config.size() != _nb_server)
        throw ErrorException("Something with size");
    for (size_t i = 0; i < _nb_server; i++) {
        ServerConfig server;
        createServer(_server_config[i], server);
        _servers.push_back(server);
    }
    if (_nb_server > 1)
        checkServers();
    return 0;
}

void    ConfigParser::removeComments(std::string &content) {
    size_t pos;

    pos = content.find('#');
    while (pos != std::string::npos) {
        size_t  pos_end;
        pos_end = content.find('\n', pos);
        content.erase(pos, pos_end - pos);
        pos = content.find('#');
    }
}

void    ConfigParser::removeWhiteSpace(std::string &content) {
    size_t  i = 0;

    while (content[i] && isspace(content[i]))
        i++;
    content = content.substr(i);
    i = content.length - 1;
    while (i > 0 && isspace(content[i]))
        i--;
    content = content.substr(0, i + 1);
}

void    ConfigParser::splitServers(std::string &content) {
    size_t  start = 0;
    size_t  end = 1;

    if (content.find("server", 0) == std::string::npos)
        throw ErrorException("Server did not find");
    while (start != end && start < content.length())
    {
        start = findStartServer(start, content);
        end = findEndServer(start, content);
        if (start == end)
            throw ErrorException("problem with scope");
        _server_config.push_back(content.substr(start, end - start + 1));
        _nb_server++;
        start = end + 1;
    }
}

size_t  ConfigParser::findStartServer(size_t start, std::string &content) {
    size_t  i;

    for (i = start; content[i]; i++) {
        if (content[i] == 's')
            break;
        if (!isspace(content[i]))
            throw ErrorException("Wrong character out of server scope{}");
    }
    if (!content[i])
        return start;
    if (content.compare(i, 6, "server") != 0)
        throw ErrorException("Wrong character out of server scope{}");
    i += 6;
    while ((content[i] && isspace(content[i])))
        i++;
    if (content[i] == '{')
        return i;
    else
        throw ErrorException("Wrong character out of server scope{}");
}

size_t  ConfigParser::findEndServer(size_t start, std::string &content) {
    size_t  i;
    size_t  scope;

    scope = 0;
    for (i = start + 1; content[i]; i++) {
        if (content[i] == '{')
            scope++;
        if (content[i] == '}') {
            if (!scope)
                return i;
            scope--;
        }
    }
    return start;
}

std::vector<std::string> splitParametrs(std::string line, std::string sep) {
    std::vector<std::string>    str;
    std::string::size_type      start, end;

    start = end = 0;
    while (1) {
        end = line.find_first_of(sep, start);
        if (end == std::string::npos)
            break;
        std::string tmp = line.substr(start, end - start);
        str.push_back(tmp);
        start = line.find_first_not_of(sep, end);
        if (start == std::string::npos)
            break;
    }
    return str;
}

void    ConfigParser::createServer(std::string &config, ServerConfig &server) {
    std::vector<std::string>    parameters;
    std::vector<std::string>    error_codes;
    int                         flag_loc = 1;
    bool                        flag_autoindex = false;
    bool                        flag_max_size = false;

    parameters = splitParametrs(config += ' ', std::string(" \n\t"));
    if (parameters.size() < 3)
        throw ErrorException("Failed server validation");
    for (size_t i = 0; i < parameters.size(); i++) {
        if (parameters[i] == "listen" && (i + 1) < parameters.size() && flag_loc) {
            if (server.getPort())
                throw ErrorException("Port is duplicated");
            server.setPort(parameters[++i]);
        }
        else if (parameters[i] == "location" && (i + 1) < parameters.size()) {
            std::string path;
            i++;
            if (parameters[i] == "{" || parameters[i] == "}")
                throw ErrorException("Wrong character in server scope{}");
            path = parameters[i];
            std::vector<std::string> codes;
            if (parameters[++i] != "{")
                throw ErrorException("Wrong character out of server scope{}");
            i++;
            while (i < parameters.size() && parameters[i] != "}")
                codes.push_back(parameters[i++]);
            server.setLocation(path, codes);
            if (i < parameters.size() && parameters[i] != "}")
                throw ErrorException("Wrong character out of server scope{}");
            flag_loc = 0;
        }
        else if (parameters[i] == "host" && (i + 1) < parameters.size() && flag_loc) {
            if(server.getHost())
                throw ErrorException("Host os duplicated");
            server.setHost(parameters[++i]);
        }
        else if (parameters[i] == "root" && (i + 1) < parameters.size() && flag_loc) {
            if (!server.getRoot().empty())
                throw ErrorException("Root is duplicated");
            server.setRoot(parameters[++i]);
        }
        else if (parameters[i] == "error_page" && (i + 1) < parameters.size() && flag_loc) {
            while (++i < parameters.size()) {
                error_codes.push_back(parameters[i]);
                if (parameters[i].find(';') != std::string::npos)
                    break;
                if (i + 1 >= parameters.size())
                    throw ErrorException("Wrong character out of server scope{}");
            }
        }
        else if (parameters[i] == "client_max_body_size" && (i + 1) < parameters.size() && flag_loc) {
            if (flag_max_size)
                throw ErrorException("Client_max_body_size is duplicated");
            server.setClientMaxBodySize(parameters[++i]);
            flag_max_size = true;
        }
        else if (parameters[i] == "server_name" && (i + 1) < parameters.size() && flag_loc) {
            if (!server.getServerName().empty())
                throw ErrorException("Server_name is duplicated");
            server.setServerName(parameters[++i]);
        }
        else if (parameters[i] == "index" && (i + 1) < parameters.size() && flag_loc) {
            if (!server.getIndex().empty())
                throw ErrorException("Index is duplicated");
            server.setIndex(parameters[++i]);
        }
        else if (parameters[i] == "autoindex" && (i + 1) < parameters.size() && flag_loc) {
            if (flag_autoindex)
                throw ErrorException("Autoindex of server is duplicated");
            server.setAutoindex(parameters[++i]);
            flag_autoindex = true;
        }
        else if (parameters[i] != "}" && parameters[i] != "{")
        {
            if (!flag_loc)
                throw ErrorException("Parameters after location");
            else
                throw ErrorException("Unsupported directive");
        }
    }
    if (server.getRoot().empty())
        server.setRoot("/;");
    if (server.getHost() == 0)
        server.setHost("localhost;");
    if (server.getIndex().empty())
        server.setIndex("index.html;");
    if (ConfigFile::isFileExistsAndReadable(server.getRoot(), server.getIndex()))
        throw ErrorException("Index from config file not found or unreadable");
    if (server.checkLocations())
        throw ErrorException("Location is duplicated");
    if (!server.getPort())
        throw ErrorException("Port not found");
    server.setErrorPages(error_codes);
    if (!server.isValidErrorPages())
        throw ErrorException("Incorrect path for error page or number of error");
}

int ConfigParser::stringCompare(std::string str1, std::string str2, size_t pos) {
    size_t  i;

    i = 0;
    while (pos < str1.length() && i < str2.length() && str1[pos] == str2[i]) {
        pos++;
        i++;
    }
    if (i == str2.length() && pos <= str1.length() && (str1.length() == pos || isspace(str1[pos])))
        return 0;
    return 1;
}

void ConfigParser::checkServers() {
    std::vector<ServerConfig>::iterator it1;
    std::vector<ServerConfig>::iterator it2;

    for (it1 = _servers.begin(); it1 != _servers.end() - 1; it1++) {
        for (it2 = it1 + 1; it2 != _servers.end(); it2++) {
            if (it1->getPort() == it2->getPort() && it1->getHost() == it2->getHost() && it1->getServerName() == it2->getServerName())
            throw ErrorException("Failed server validation");
        }
    }
}

std::vector<ServerConfig>   ConfigParser::getServers() {
    return _servers;
}