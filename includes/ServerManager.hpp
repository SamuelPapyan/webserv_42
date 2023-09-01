/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spapyan <spapyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 09:19:29 by spapyan           #+#    #+#             */
/*   Updated: 2023/08/29 09:19:29 by spapyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "Webserv.hpp"
# include "Client.hpp"
# include "Response.hpp"

class ServerManager {
private:
    std::vector<ServerConfig>   _servers;
    std::map<int, ServerConfig> _servers_map;
    std::map<int, Client>       _clients_map;
    fd_set                      _recv_fd_pool;
    fd_set                      _write_fd_pool;
    int                         _biggest_fd;

    void    acceptNewConnection(ServerConfig&);
    void    checkTimeout();
    void    initializeSets();
    void    readRequest(const int&, Client&);
    void    handleReqBody(Client&);
    void    sendResponse(const int&, Client&);
    void    sendCgiBody(Client&, CgiHandler&);
    void    readCgiResponse(Client&, CgiHandler&);
    void    closeConnection(const int);
    void    assignServer(Client&);
    void    addToSet(const int, fd_set&);
    void    removeFormSet(const int, fd_set&);

public:
    ServerManager();
    ~ServerManager();

    void    setupServers(std::vector<ServerConfig>);
    void    runServers();
};

#endif