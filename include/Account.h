#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

class Account {
private:
    const int id;
    std::string username;
    std::string password;

public:
    // Constructor
    Account(int id, const std::string& username,
            const std::string& password);

    // Getters
    int getId() const;
    std::string getUsername() const;

    // Setters
    void setUsername(const std::string& username);
    void setPassword(const std::string& oldPassword,
                     const std::string& newPassword);

    // Authentication
    bool authenticate(const std::string& password) const;

    // Abstract method
    virtual std::string getRole() const = 0;

    // String representation
    std::string toString() const;

    // Virtual destructor
    virtual ~Account() = default;
};

#endif