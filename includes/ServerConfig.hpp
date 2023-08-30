/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spapyan <spapyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 10:05:42 by spapyan           #+#    #+#             */
/*   Updated: 2023/08/28 10:05:42 by spapyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

# include "Webserv.hpp"

class Location;

class ServerConfig {
private:
    uint16_t                        _port;
    in_addr_t                       _host;
    std::string                     _server_name;
    std::string                     _root;
    unsigned long                   _client_max_body_size;
    std::string                     _index;
    bool                            _autoindex;
    std::map<short, std::string>    _error_pages;
    std::vector<Location>           _locations;
    struct sockaddr_in              _server_address;
    int                             _listen_fd;

public:
    ServerConfig();
    ~ServerConfig();
    ServerConfig(const ServerConfig &other);
    ServerConfig&   operator=(const ServerConfig& rhs);

    void    initErrorPages(void);
    void    serServerName(std::string server_name);
    void    serHost(std::string parameter);
    void    setRoot(std::string root);
    void    setFd(int);
    void    setPort(std::string parameter);
    void    setErrorPages(std::vector<std::string>& parameter);
    void    setIndex(std::string index);
    void    setLocation(std::string nameLocation, std::vector<std::string> parameter);
    void    setAutoindex(std::string autoindex);

    bool    isValidHost(std::string host) const;
    bool    isValidErrorPages();
    int     isValidLocation(Location& location) const;

    const std::string&                      getServerName();
    const uint16_t&                         getPort();
    const in_addr_t&                        getHost();
    const size_t&                           getClientMaxBodySize();
    const std::vector<Location>&            getLocations();
    const std::string&                      getRoot();
    const std::map<short, std::string>&     getErrorPages();
    const std::string&                      getIndex();
    const bool&                             getAutoindex();
    const std::string&                      getPathErrorPage(short key);
    const std::vector<Location>::iterator   getLocationKey(std::string key);

    static void checkToken(std::string& parameter);
    bool        checkLocation() const;
    void        setupServer();
    int         getFd();

    public:
    class ErrorException : public std::exception {
    private:
        std::string _message;
    public:
        ErrorException(std::string message) throw() {
            _message = "SERVER CONFIG ERROR: " +  message;
        }
        virtual const char* what() const sthrow() {
            return _message.c_str();
        }
        virtual ~ErrorException() throw() {}
    };
};

#endif