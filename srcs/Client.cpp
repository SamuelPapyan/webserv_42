/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spapyan <spapyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/20 16:07:24 by spapyan           #+#    #+#             */
/*   Updated: 2023/08/20 16:07:24 by spapyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client() {
    _last_msg_time = time(NULL);
}

Client::Client(const Client& other) {
    if (this != &other) {
        _client_socket = other._client_socket;
        _client_address = other._client_address;
        request = other.request;
        response = other.response;
        server = other.server;
        _last_msg_time = other._last_msg_time;
    }
    return;
}

Client& Client::operator=(const Client& rhs) {
    if (this != &rhs) {
        _client_socket = other._client_socket;
        _client_address = other._client_address;
        request = other.request;
        response = other.response;
        server = other.server;
        _last_msg_time = other._last_msg_time;
    }
    return *this;
}

Client::Client(ServerConfig &server) {
    setServer(server);
    request.setMaxBodySize(server.getClientMaxBodySize());
    _last_msg_time = time(NULL);
}

void    Client::setSocket(int &sock) {
    _client_socket = sock;
}

void    Client::setAddress(sockaddr_in  &addr) {
    _client_address = addr;
}

void    Client::setServer(ServerConfig &server) {
    response.setServer(server);
}

const int   &Client::getSocket() const {
    return _client_socket;
}

const HttpRequest   &Client::getRequest() const {
    return request;
}

const struct sockaddr_in    &Client::getAddress() const {
    return _client_address;
}

const time_t    &Client::getLastTime() const {
    return _last_msg_time;
}

void    Client::buildResponse() {
    response.setRequest(request);
    response.buildResponse();
}

void    Client::updateTime() {
    _last_msg_time = time(NULL);
}

void    Client::clearCilent() {
    response.clear();
    request.clear();
}
