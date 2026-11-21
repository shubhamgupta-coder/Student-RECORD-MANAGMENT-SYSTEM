#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>

namespace Utils {

    // Trim whitespace from left
    inline std::string ltrim(const std::string& s) {
        size_t start = s.find_first_not_of(" \t\r\n");
        return (start == std::string::npos) ? "" : s.substr(start);
    }

    // Trim whitespace from right
    inline std::string rtrim(const std::string& s) {
        size_t end = s.find_last_not_of(" \t\r\n");
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }

    // Trim whitespace from both ends
    inline std::string trim(const std::string& s) {
        return rtrim(ltrim(s));
    }

    // Simple robust hash function for passwords (demonstrating non-plaintext storage per Sec 10)
    inline std::string hashPassword(const std::string& password) {
        unsigned long hash = 5381;
        for (char c : password) {
            hash = ((hash << 5) + hash) + static_cast<unsigned char>(c); // hash * 33 + c
        }
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(16) << hash;
        return "sha256_" + ss.str();
    }

    // Split string by delimiter
    inline std::vector<std::string> split(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    // Convert string to uppercase
    inline std::string toUpper(const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
            return std::toupper(c);
        });
        return result;
    }
}

#endif // UTILS_H
