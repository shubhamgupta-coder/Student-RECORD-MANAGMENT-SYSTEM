#ifndef USER_H
#define USER_H

#include <string>
#include "Utils.h"

class User {
private:
    std::string userId;
    std::string username;
    std::string passwordHash;
    std::string role;      // "Admin" or "Staff"
    bool active;
    bool loggedIn;

public:
    User();
    User(const std::string& uId, const std::string& uName, const std::string& pwdHash, const std::string& uRole, bool isActive = true);

    // Getters
    std::string getUserId() const;
    std::string getUsername() const;
    std::string getPasswordHash() const;
    std::string getRole() const;
    bool isActive() const;
    bool isLoggedIn() const;

    // Setters
    void setPassword(const std::string& plainPassword);
    void setRole(const std::string& uRole);
    void setActive(bool state);

    // Responsibilities from design specification
    bool login(const std::string& enteredPassword);
    void logout();
    bool hasPermission(const std::string& permissionRequired) const;

    // Serialization helper
    std::string serialize() const;
    static User deserialize(const std::string& line);
};

#endif // USER_H
