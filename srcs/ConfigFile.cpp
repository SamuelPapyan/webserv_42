/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigFile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spapyan <spapyan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 09:43:08 by spapyan           #+#    #+#             */
/*   Updated: 2023/08/27 09:43:08 by spapyan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ConfigFile.hpp"

ConfigFile::ConfigFile() : _size(0) { }

ConfigFile::ConfigFile(std::string const path) : _path(path), _size(0) {}

ConfigFile::~ConfigFile() {}

int ConfigFile::getTypePath(std::string const path) {
    struct stat buffer;
    int         result;

    result = stat(path.c_str(), &buffer);
    if (result == 0) {
        if (buffer.st_mode & S_IFREG)
            return 1;
        else if (buffer.st_mode & S_IFDIR)
            return 2;
        else
            return 3;
    }
    else
        return -1;
}

int ConfigFile::checkFile(std::string const path, int mode) {
    return access(path.c_str(), mode);
}

int ConfigFile::isFileExistsAndReadable(std::string const path, std::string const index) {
    if (getTypePath(index) == 1 && checkFile(index, 4) == 0)
        return 0;
    if (getTypePath(path + index) == 1 && checkFile(path + index, 4) == 0)
        return 0;
    return -1;
}

std::string ConfigFile::readFile(std::string path) {
    if (path.empty() || path.length() == 0)
        return NULL;
    std::ifstream   config_file(path.c_str());
    if (!config_file || !confif_file.is_open())
        return NULL;

    std::stringstream   stream_binding;
    stream_binding << confif_file.rdbuf();
    return (stream_binding.str());
}

std::string ConfigFile::getPath() {
    return _path;
}

int ConfigFile::getSize() {
    return _size;
}
