#include "Account.h"
#include <stdexcept>

Account::Account(int id, const std::string& username,
                 const std::string& password)
    : id(id), username(username), password(password) {

    if (id <= 0 ||
        username.find_first_not_of(" \t\n\r") == std::string::npos ||
        password.find_first_not_of(" \t\n\r") == std::string::npos) {

        throw std::invalid_argument("Invalid account information");
    }
}

int Account::getId() const {
    return id;
}

std::string Account::getUsername() const {
    return username;
}

void Account::setUsername(const std::string& username) {
    if (username.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument("Invalid username");
    }

    if (this->username == username) {
        throw std::invalid_argument("Username must be different");
    }

    this->username = username;
}

void Account::setPassword(const std::string& oldPassword,
                          const std::string& newPassword) {

    if (newPassword.find_first_not_of(" \t\n\r") == std::string::npos) {
        throw std::invalid_argument("Invalid new password");
    }

    if (this->password != oldPassword) {
        throw std::invalid_argument("Incorrect password");
    }

    if (oldPassword == newPassword) {
        throw std::invalid_argument("New password must be different");
    }

    this->password = newPassword;
}

bool Account::authenticate(const std::string& password) const {
    return !password.empty() &&
           this->password == password;
}

std::string Account::toString() const {
    return "ID: " + std::to_string(id) +
           "\nUsername: " + username +
           "\nRole: " + getRole();
}