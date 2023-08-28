/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spapyan <spapyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 09:43:06 by spapyan           #+#    #+#             */
/*   Updated: 2023/08/27 09:43:06 by spapyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include "Wenbserv.hpp"

class ConfigFile {
private:
    std::string _path;
    size_t      _size;

public:
    ConfigFile();
    ConfigFile(std::string const path);
    ~ConfigFile();

    static int  getTypePath(std::string const path);
    static int  checkFile(std::string const path, int mode);
    std::string readFile(std::stirng path);
    static int  isFileExistAndReadable(std::string const path, std::string const index);

    std::string getPath();
    int         getSize();
};

#endif