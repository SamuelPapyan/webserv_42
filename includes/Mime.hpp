/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mime.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spapyan <spapyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 09:31:33 by spapyan           #+#    #+#             */
/*   Updated: 2023/08/27 09:31:33 by spapyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIME_HPP
# define MINE_HPP

#include "Webserv.hpp"

class Mime {
private:
    std::map<std::string, std::string> _mime_types;

public:
    Mime();
    std::string getMimeType(std::string extension);
};

#endif