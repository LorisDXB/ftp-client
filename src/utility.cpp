/*
 * < Kawabonga ! >
 * ------------------------------
 *    \   ^__^
 *     \  (oo)\_______
 *        (__)\       )\/\
 *            ||----w |
 *            ||     ||
 * 
 */

#include <string>
#include <iostream>
#include <stdexcept>
#include <stdint.h>
#include <vector>
#include <sstream>
#include <random>
#include <memory>
#include "client.hpp"
#include <filesystem>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


uint16_t stouint16(const std::string &port)
{
	try {
		unsigned long long ret = std::stoull(port);

		return static_cast<uint16_t>(ret);
	} catch (const std::exception &e) {
		throw std::invalid_argument(e.what());
	} 
}

std::vector<std::string> splitString(const std::string &str)
{
	std::vector<std::string> result;
	std::stringstream ss(str);
	std::string tmp;

	while (ss >> tmp) {
		result.push_back(tmp);
	}

	return result;
}

std::vector<std::string> splitStringDelim(const std::string &str, char delim)
{
	std::vector<std::string> ret;
	std::string tmp;

	for (char c : str) {
		if (c == delim && !tmp.empty()) {
			ret.push_back(tmp);
			tmp.clear();
		} else {
			tmp += c;
		}
	}
	ret.push_back(tmp);
	return ret;
}

int generateRandomPort()
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<int> dist6(1024, 65535);

	return dist6(rng);
}

std::string resolvePreviousDir(const std::string &homeDir, const std::string &currDir)
{
    std::filesystem::path homePath(homeDir);
    std::filesystem::path currPath(currDir);

	if (currPath == homePath) {
		return "";
	}
    if (currPath.parent_path() == homePath) {
        return homeDir;
    }

    std::filesystem::path resolvedPath = currPath.parent_path();

    return resolvedPath.string();
}

std::string resolvePath(std::shared_ptr<ftp::Client> &client, const std::string &path)
{
    std::string currDir = client->getCurrDir();
    std::string homeDir = client->getHomeDir();

    if (homeDir.empty()) {
        return "";
    }

    std::filesystem::path homePath(homeDir);
    std::filesystem::path currPath(currDir);
    std::filesystem::path inputPath(path);

    std::filesystem::path resolvedPath;
    if (path == "..") {
        return resolvePreviousDir(homeDir, currDir);
    }

    if (inputPath.is_relative()) {
        resolvedPath = currPath / inputPath;
    } else {
        resolvedPath = homePath / inputPath.relative_path();
    }

    return resolvedPath.string();
}