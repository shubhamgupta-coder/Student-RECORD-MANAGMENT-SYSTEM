#include "../include/User.h"

User::User() : userId(""), username(""), passwordHash(""), role("Staff"), active(true), loggedIn(false) {}

User::User(const std::string& uId, const std::string& uName, const std::string& pwdHash, const std::string& uRole, bool isActive)
    : userId(uId), username(uName), passwordHash(pwdHash), role(uRole), active(isActive), loggedIn(false) {}

std::string User::getUserId() const { return userId; }
std::string User::getUsername() const { return username; }
std::string User::getPasswordHash() const { return passwordHash; }
std::string User::getRole() const { return role; }
bool User::isActive() const { return active; }
bool User::isLoggedIn() const { return loggedIn; }

void User::setPassword(const std::string& plainPassword) {
    this->passwordHash = Utils::hashPassword(plainPassword);
}

void User::setRole(const std::string& uRole) {
    this->role = uRole;
}

void User::setActive(bool state) {
    this->active = state;
}

bool User::login(const std::string& enteredPassword) {
    if (!active) {
        return false;
    }
    std::string hashedInput = Utils::hashPassword(enteredPassword);
    if (hashedInput == passwordHash) {
        loggedIn = true;
        return true;
    }
    return false;
}

void User::logout() {
    loggedIn = false;
}

bool User::hasPermission(const std::string& permissionRequired) const {
    if (!active || !loggedIn) return false;
    if (role == "Admin") return true; // Admin has full access
    if (role == "Staff") {
        if (permissionRequired == "MANAGE_USERS") return false;
        if (permissionRequired == "DELETE_STUDENT") return false;
        return true; // Staff can view, add, search, enter marks, generate reports
    }
    return false;
}

std::string User::serialize() const {
    return userId + "|" + username + "|" + passwordHash + "|" + role + "|" + (active ? "1" : "0");
}

User User::deserialize(const std::string& line) {
    std::vector<std::string> parts = Utils::split(line, '|');
    if (parts.size() >= 5) {
        return User(parts[0], parts[1], parts[2], parts[3], parts[4] == "1");
    }
    return User();
}
