/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spapyan <spapyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 11:24:17 by spapyan           #+#    #+#             */
/*   Updated: 2023/08/27 11:24:17 by spapyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Location.hpp"

Location::Location() {
    _path = "";
    _root = "";
    _autoindex = false;
    _index = "";
    _return = "";
    _alias = "";
    _client_max_body_size = MAX_CONTENT_LENGTH;
    _methods.reserve(5);
    _methods.push_back(1);
    _methods.push_back(0);
    _methods.push_back(0);
    _methods.push_back(0);
    _methods.push_back(0);
}

Location::Location(const Location& other) {
    _path = other._path;
    _root = other._root;
    _autoindex = other._autoindex;
    _index = other._index;
    _cgi_path = other._cgi_path;
    _cgi_ext = other._cgi_ext;
    _return = other._return;
    _alias = other._alias;
    _methods = other._methods;
    _ext_path = other._ext_path;
    _client_max_body_size = other._client_max_body_size;
}

Location& Location::operator=(const Location& rhs) {
    if (this != &rhs) {
        _path = rhs._path;
        _root = rhs._root;
        _autoindex = rhs._autoindex;
        _index = rhs._index;
        _cgi_path = rhs._cgi_path;
        _cgi_ext = rhs._cgi_ext;
        _return = rhs._return;
        _alias = rhs._alias;
        _methods = rhs._methods;
        _ext_path = rhs._ext_path;
        _client_max_body_size = rhs._client_max_body_size;
    }
    return *this;
}

Location::~Location() {}

void    Location::setPath(std::string parameter) {
    _path = parameter;
}

void    Location::setRootLocation(std::string parameter) {
    if (ConfigFile::getTypePath(parameter) != 2)
        throw ServerConfig::ErrorException("root of location");
    _root = parameter;
}

void    Location::setMethods(std::vector<std::string> methods) {
    _methods[0] = 0;
    _methods[1] = 0;
    _methods[2] = 0;
    _methods[3] = 0;
    _methods[4] = 0;

    for (size_t i = 0; i < methods.size(); i++) {
        if (methods[i] == "GET")
            _methods[0] = 1;
        else if (methods[i] == "POST")
            _methods[1] = 1;
        else if (methods[i] == "DELETE")
            _methods[2] = 1;
        else if (methods[i] == "PUT")
            _methods[3] = 1;
        else if (methods[i] == "HEAD")
            _methods[4] = 1;
        else
            throw ServerConfig::ErrorException("Allow methods not supported " _ methods[i]);
    }
}

void    Location::setAutoindex(std::string parameter) {
    if (parameter == "on" || parameter == "off")
        _autoindex = (parameter == "on")
    else
        throw ServerConfig::ErrorException("Wrong autoindex");
}

void    Location::setIndexLocation(std::string parameter) {
    _index = parameter;
}

void    Location::setReturn(std::string parameter) {
    _return = parameter;
}

void    Location::serAlias(std::stirng parameter) {
    _alias = parameter;
}

void    Location::setCgiPath(std::vector<std::string> path) {
    _cgi_path = path;
}

void    Location::setCgiExtension(std::vector<std::string> extension) {
    _cgi_ext = extension;
}

void    Location::setMaxBodySize(std::string parameter) {
    unsigned long   body_size = 0;

    for (size_t i = 0; i < parameter.length(); i++) {
        if (parameter[i] < '0' || parameter[i] > '9')
            throw ServerConfig::ErrorException("Wrong syntax: client_max_body_size");
    }
    if (!ft_stoi(parameter))
        throw ServerConfig::ErrorException("Wrong syntax: client_max_body_size");
    body_size = ft_stoi(parameter);
    _client_max_body_size = body_size;
}

void    Location::setMaxBodySize(unsigned long parameter) {
    _client_max_body_size = parameter;
}

const std::string&  Location::getPath() const {
    return _path;
}

const std::string&  Location::getRootLocation() const {
    return _root;
}

const std::string&  Location::getIndexLocation() const {
    return _index;
}

const std::vector<short>&   Location::getMethods() const {
    return _methods;
}

const std::vector<std::string>& Location::getCgiPath() const {
    return _cgi_path;
}

const std::vector<std::string>& Location::getCgiExtension() const {
    return _cgi_ext;
}

const bool&  Location::getAutoindex() const {
    return _autoindex;
}

const std::string&  Location::getReturn() const {
    return _return;
}

const std::string&  Location::getAlias() const {
    return _alias;
}

const std::map<std::string, std::string>&   Location::getExtensionPath() const {
    return _ext_path;
}

const unsigned long&    Location::getMaxBodySize() const {
    return _client_max_body_size;
}

std::string Location::getPrintMethods() const {
    std::string res;
    if (_methods[4])
        res.insert(0, "HEAD");
    if (_methods[3]) {
        if (!res.empty())
            res.insert(0, ", ");
        res.insert(0, "PUT");
    }
    if (_methods[2]) {
        if (!res.empty())
            res.insert(0, ", ");
        res.insert(0, "DELETE");
    }
    if (_methods[1]) {
        if (!res.empty())
            res.insert(0, ", ");
        res.insert(0, "POST");
    }
    if (_methods[0]) {
        if (!res.empty())
            res.insert(0, ", ");
        res.insert(0, "GET");
    }
    return res;
}
