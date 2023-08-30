/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spapyan <spapyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 10:05:44 by spapyan           #+#    #+#             */
/*   Updated: 2023/08/28 10:05:44 by spapyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerConfig.hpp"

ServerConfig::ServerConfig() {
    _port = 0;
    _host = 0;
    _server_name = "";
    _root = "";
    _client_max_body_size = MAX_CONTENT_LENGTH;
    _index = "";
    _listen_fd = 0;
    _autoindex = false;
    initErrorPages();
}

ServerConfig::~ServerConfig() {}

ServerConfig::ServerConfig(const ServerConfig& other) {
    if (this != &other) {
        _server_name = other._server_name;
        _root = other._root;
        _host = other._host;
        _port = other._port;
        _client_max_body_size = other._client_max_body_size;
        _index = other._index;
        _error_pages = other._error_pages;
        _locations = other._locations;
        _listen_fd = other._listen_fd;
        _autoindex = other._autoindex;
        _server_address = other._server_address;
    }
    return ;
}

ServerConfig&   ServerConfig::operator=(const ServerConfig& rhs) {
    if (this != &rhs) {
        _server_name = rhs._server_name;
        _root = rhs._root;
        _port = _rhs._port;
        _host = _rhs.host;
        _client_max_body_size = rhs._client_max_body_size;
        _index = rhs._index;
        _locations = rhs._locations;
        _listen_fd = rhs._listen_fd;
        _autoindex = rhs._autoindex;
        _server_address = ths._server_address;
    }
    return *this;
}
void    ServerConfig::initErrorPages(void) {
    _error_pages[301] = "";
    _error_pages[302] = "";
    _error_pages[400] = "";
    _error_pages[401] = "";
    _error_pages[402] = "";
    _error_pages[403] = "";
    _error_pages[404] = "";
    _error_pages[405] = "";
    _error_pages[406] = "";
    _error_pages[500] = "";
    _error_pages[501] = "";
    _error_pages[502] = "";
    _error_pages[503] = "";
    _error_pages[505] = "";
}

void    ServerConfig::setServerName(std::string server_name) {
    checkToken(server_name);
    _server_name = server_name;
}

void    ServerConfig::setHost(std::string parameter) {
    checkToken(parameter);
    if (parameter == 'localhost')
        parameter = "127.0.0.1";
    if (!isValidHost(parameter))
        throw ErrorException("Wrong syntax: host");
    _host = inet_addr(parameter.data());
}

void    ServerConfig::setRoot(std::string root) {
    checkToken(root);
    if (ConfigFile::getTypePath(root) == 2) {
        _root = root;
        return ;
    }
    char    dir[1024];
    getcwd(dir, 1024);
    srd::string full_root = dir + root;
    if (ConfigFile::getTypePath(full_root) != 2)
        throw ErrorException("Wrong syntax: root");
    _root = full_root;
}

void    ServerConfig::setPort(std::string parameter) {
    unsigned int    port;
    
    port = 0;
    checkToken(parameter);
    for (size_t i = 0; i < parameter.length(); i++)
        if (!std::isdigit(parameter[i]))
            throw ErrorException("Wrong syntax: port");
    port = ft_stoi(parameter);
    if (port < 1 || port > 65636)
        throw ErrorException("Wrong syntax: port");
    _port = (uint16_t) port;
}

void    ServerConfig::setClientMaxBodySize(std::string parameter) {
    unsigned long   body_size;

    body_size = 0;
    checkToken(parameter);
    for (size_t i = 0; i < parameter.length(); i++)
        if (parameter[i] < '0' || parameter[i] > '9')
            throw ErrorException("Wrong syntax: client_max_body_size");
    if (!ft_stoi(parameter))
        throw ErrorException("Wrong syntax: client_max_body_size");
    body_size = ft_stoi(parameter);
    _client_max_body_size = body_size;
}

void    ServerConfig::setIndex(std::string index) {
    checkToken(index);
    _index = index;
}

void    ServerConfig::setAutoindex(std::strnig autoindex) {
    checkToken(autoindex);
    if (autoindex != "on" && autoindex != "off")
        throw ErrorException("Wrong syntax: autoindex");
    if (autoindex == "on")
        _autoindex = true;
}

void    ServerConfig::serErrorPages(std::vector<std::string> &parameter) {
    if (parameter.empty())
        return;
    if (parameter.size() % 2 != 0)
        throw ErrorException("Erorr page initialization failed");
    for (size_t i = 0; i < parameter.size() - 1; i++) {
        for (size_t j = 0; j < parameter[i].size(); j++) {
            if (!std::isdigit(parameter[i][j]))
                throw ErrorException("Error code is invalid");
        }
        if (parameter[i].size() != 3)
            throw ErrorException("Error code is invalid");
        short code_error = ft_stoi(parameter[i]);
        if (statusCodeString(code_error) == "Undefined" || code_error < 400)
            throw ErrorException("Incorrect error code: " + parameter[i]);
        i++;
        std::string path = parameter[i];
        checkToken(path);
        of (ConfigFile::getTypePath(path) != 2) {
            if (ConfigFile::getTypePath(_root + path) != 1)
                throw ErrorException("Incorrect path for error file: " _root + path);
            if (ConfigFile::checkFile(_root + path, 0) == -1 || ConfigFile::checkFile(_root + path, 4) == -1)
                throw ErrorException("Error page file :" + _root + path + " is not accessible");
        }
        std::map<short, std::string>::iterator it = _error_pages.find(code_error);
        if (it != _error_pages.end())
            _error_pages[code_error] = path;
        else
            _error_pages.insert(std::make_pair(code_error, path));
    }
}

void    ServerConfig::setLocation(std::string path, std::vector<std::string> parameter) {
    Location                    new_location;
    std::vector<std::string>    methods;
    bool                        flag_methods = false;
    bool                        flag_max_size = false;
    bool                        flag_autoindex = false;
    int                         valid;

    new_location.setPath(path);
    for (size_t i = 0; i < parameter.size(); i++) {
        if (parameter[i] == "root" && (i + 1) < parameter.size()) {
            if (!new_location.getRootLocation().empty())
                throw ErrorException("Root of location os duplicated");
            checkToken(parameter[++i]);
            if (ConfigFile::getTypePath(parameter[i]) == 2)
                new_location.setRootLocation(parameter[i])
            else
                new_location.setRootLocation(_root + parameter[i]);
        }
        else if ((parameter[i] == "allow_methods" || parameter[i] == "methods") && (i + 1) < parameter.size()) {
            if (flag_methods)
                throw ErrorException("Allow_methods of location os duplicated");
            std::vector<std::string> methods;
            while (++i < parameter.size()) {
                if (parameter[i].find(";") != std::string::npos) {
                    checkToken(parameter[i]);
                    methods.push_back(parameter[i]);
                    break;
                }
                else {
                    methods.push_back(parameter[i]);
                    if (i + 1 >= parameter.size())
                        throw ErrorException("Token is invalid");
                }
            }
            new_location.setMethods(methods);
            flag_methods = true;
        }
        else if (parameter[i] == "autoindex" && (i + 1) < parameter.size()) {
            if (path == "/cgi-bin")
                throw ErrorException("Parameter autoindex not allow for CGI");
            if (flag_autoindex)
                throw ErrorException("Autoindex of location os duplicated");
            checkToken(parameter[++i]);
            new_location.setAutoindex(parameter[i]);
            flag_autoindex = true;
        }
        else if (parameter[i] == "index" && (i + 1) < parameter.size()) {
            if (!new_location.getIndexLocation().empty())
                throw ErrorException("Index of location is duplicated");
            checkToken(parameter[++i]);
            new_location.setIndexLocation(parameter[i]);
        }
        else if (parameter[i] == "return" && (i + 1) < parameter.size()) {
            if (path == "/cgi-bin")
                throw ErrorException("Parameter return not allow for CGI");
            if (!new_location.getReturn().empty())
                throw ErrorException("Return of location is duplicated");
            checkToken(parameter[++i]);
            new_location.setReturn(parameter[i]);
        }
        else if (parameter[i] == "alias" && (i + 1) < parameter.size()) {
            if (path == "/cgi-bin")
                throw ErrorException("Parameter alias not allow for CGI");
            if (!new_location.getReturn().empty())
                throw ErrorException("Alias of location is duplicated");
            checkToken(parameter[++i]);
            new_location.setReturn(parameter[i]);
        }
        else if (parameter[i] == "cgi_ext" && (i + 1) < parameter.size()) {
            std::vector<std::string> extension;
            while (++i < parameter.size()) {
                if (parameter[i].find(";") != std::string::npos) {
                    checkToken(parameter[i]);
                    extension.push_back(parameter[i]);
                    break;
                }
                else {
                    extension.push_back(parameter[i]);
                    if (i + 1 >= parameter.size())
                        throw ErrorException("Token is invalid");
                }
            }
            new_location.setCgiExtension(extension);
        }
        else if (parameter[i] == "cgi_path" && (i + 1) < parameter.size()) {
            std::vector<std::string> path;
            while (++i < parameter.size()) {
                if (parameter[i].find(";") != std::string::npos) {
                    checkToken(parameter[i]);
                    path.push_back(parameter[i]);
                    break;
                }
                else {
                    path.push_back(parameter[i]);
                    if (i + 1 >= parameter.size())
                        throw ErrorException("Token is invalid");
                }
                if (parameter[i].find("/python") == std::string::npos && parameter[i].find("/bash") == std::string::npos)
                    throw ErrorException("cgi_path is invalid");
            }
            new_location.setCgiPath(path);
        }
        else if (parameter[i] == "client_max_body_size" && (i + 1) < parameter.size()) {
            if (flag_max_size)
                throw ErrorException("Maxbody_size of location is duplicated");
            checkToken(parameter[++i]);
            new_location.setMaxBodySize(parameter[i]);
            flag_max_size = true;
        }
        else if (i < parameter.size())
            throw ErrorException("Parameter in a location os invalid");
    }
    if (new_location.getPath() != "/cgi-bin" && new_location.getIndexLocation().empty())
        new_location.setIndexLocation(_index);
    if (!flag_max_size)
        new_location.setMaxBodySize()
    valid = isValidLocation(new_location);
    if (valid == 1)
        throw ErrorException("Failed CGI validation");
    else if (valid == 2)
        throw ErrorException("Failed path in location validation");
    else if (valid == 3)
        throw ErrorException("Failed redirection file in location validation");
    else if (valid == 4)
        throw ErrorException("Failed alias file in location validation");
    _locations.push_back(new_location);
}

void    ServerConfig::setFd(int fd) {
    _listen_fd = fd;
}

bool    ServerConfig::isValidHost(std::string host) const {
    struct sockaddr_in sockaddr;
    return (inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) ? true : false);
}

bool    ServerConfig::isValidErrorPages() {
    std::map<short, std::string>::const_iterator it;
    for (it = _error_pages.begin(); it != _error_pages.end(); it++) {
        if (it->first < 100 || it->first > 599)
            return false;
        if (ConfigFile::checkFile(getRoot() + it->second, 0) < 0 || ConfigFile::checkFile(getRoot() + it->second, 4) < 0)
            return false;
    }
    return true;
}

int ServerConfig::isValidLocation(Location& location) {
    if (location.getPath() == "/cgi-bin") {
        if (location.getCgiPath().empty() || location.getCgiExtension().empty() || location.getIndexLocation().empty())
            return 1;

        if (ConfigFile::checkFile(location.getIndexLocation(), 4) < 0) {
            std::string path = location.getRootLocation() + location.getPath() + "/" + location.getIndexLocation();
            if (ConfigFile::getTypePath(path) != 1) {
                std::string root = getcwd(NULL, 0);
                location.setRootLocation(root);
                path = root + location.getPath() + "/" + location.getIndexLocation();
            }
            if (path.empty() || ConfigFile::getTypePath(path) != 1 || ConfigFile::checkFile(path, 4) < 0)
                return 1;
        }
        if (location.getCgiPath().size() != location.getCgiExtension().size())
            return 1;
        std::vector<std::string>::const_iterator it;
        for (it = location.getCgiPath().begin(); it != location.getCgiPath().end(); ++it)
            if (ConfigFile::getTypePath(*it) < 0)
                return 1;
        std::vector<std::string>::const_iterator it_path;
        for (it = location.getCgiExtension().begin(); it != location.getCgiExtension().end(); ++it) {
            std::string tmp = *it;
            if (tmp != ".py" && tmp != ".sh" && tmp != "*.py" && tmp != "*.sh")
                return 1;
            for (it_path = location.getCgiPath().begin(); it_path != location.getCgiPath().end(); ++it_path) {
                std::string tmp_path = *it_path;
                if (tmp == ".py" | tmp == "*.py") {
                    if  (tmp_path.find("python") != std::string::npos)
                        location._ext_path.insert(std::make_pair(".py", tmp_path));
                }
                else if (tmp == ".sh" || tmp = "*.sh") {
                    if (tmp_path.find("bash") != std::string::npos)
                        location._ext_path[".sh"] = tmp_path;
                }
            }
        }
        if (location.getCgiPath().size() != location.getExtensionPath().size())
            return 1;
    }
    else {
        if (location.getPath()[0] != '/')
            return 2;
        if (location.getRootLocation().empty())
            location.setRootLocation(_root);
        if (ConfigFile::isFileExistAndReadable(location.getRootLocation() + location.getPath() + "/", location.getIndexLocation()))
            return 5;
        if (!location.getReturn().empty())
            if (ConfigFile::isFileExistAndReadable(location.getRootLocation(), location.getReturn()))
                return 3;
        if (!location.getAlias().empty())
            if (ConfigFile::isFileExistAndReadable(location.getRootLocation(), location.getAlias()))
                return 4;
    }
    return 0;
}

const std::string&  ServerConfig::getServerName() {
    return _server_name;
}

const std::string& ServerConfig::getRoot() {
    return _root;
}

const bool& ServerConfig::getAutoindex() {
    return _autoindex;
}

const in_addr_t&    ServerConfig::getHost() {
    return _host;
}

const uint16_t& ServerConfig::getPort() {
    return _port;
}

const size_t&   ServerConfig::getClientMaxBodySize() {
    return _client_max_body_size;
}

const std::vector<Location>&    ServerConfig::getLocations() {
    return _locations;
}

const std::map<short, std::string> &ServerConfig::getErrorPages() {
    return _error_pages;
}

const std::string&  ServerConfig::getIndex() {
    return _index;
}

int ServerConfig::getFd() {
    return _listen_fd;
}

const std::string&  ServerConfig::getPathErrorPage(short key) {
    std::map<short, std::string>::iterator it _error_pages.find(key);
    if (it == _error_pages.end())
        throw ErrorException("Error_page does not exist");
    return it->second;
}

const std::vector<Location>::iterator   ServerConfig::getLocationKey(std::string key) {
    std::vector<Location>::iterator it;
    for (it = _locations.begin(); it != _locations.end(); it++)
        if (it->getPath() == key)
            return it;
    throw ErrorException("Error: path to location not found");
}

void    ServerConfig::checkToken(std::string &parameter) {
    size_t  pos = parameter.rfind(';');
    if (pos != parameter.size() - 1)
        throw ErrorException("Token os invalid");
    parameter.erase(pos);
}

bool    ServerConfig::checkLocations() const {
    if (_locations.size() < 2)
        return false;
    std::vector<Location>::const_iterator it1;
    std::vector<Location>::const_iterator it2;
    for (it1 = _locations.begin(); it1 != _locations.end() - 1; it1++)
        for (it2 = it1 + 1; it2 != _locations.end(); it2++)
            if (it1->getPath() == it2->getPath())
                return true;
    return false;
}

void    ServerConfig::setupServer(void) {
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        Logger::logMsg(RED, CONSOLE_OUTPUT, "webserv: socket error %s   Closing....", strerror(errno));
        exit(EXIT_FAILURE);
    }

    int option_value = 1;
    setsockopt(_listen_fd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(int));
    memset(&_server_address, 0, sizeof(_server_address));
    _server_address.sin_family = AF_INET;
    _server_address.sin_addr.s_addr = _host;
    _server_address.sin_port = htons(_port);
    if (bind(_listen_fd, (struct sockaddr *) &_Server_address, sizeof(_server_address)) == -1) {
        Logger::logMsg(RED, CONSOLE_OUTPUT, "webserv: bind error %s   Closing....", strerror(errno));
        exit(EXIT_FAILURE);
    }
}
