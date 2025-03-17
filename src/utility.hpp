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

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <stdint.h>
#include <string>
#include <vector>

uint16_t stouint16(const std::string &port);
std::vector<std::string> splitString(const std::string& str);
int generateRandomPort();
std::string resolvePath(std::shared_ptr<ftp::Client> &client, const std::string &path);
std::vector<std::string> splitStringDelim(const std::string &str, char delim);

#endif // UTILITY_HPP