#ifndef ACCOUNTMANAGER_H
#define ACCOUNTMANAGER_H

#include <string>
#include <vector>
#include <memory>
#include <cstddef>

#include "Account.h"
#include "RemovalStatus.h"

class AccountManager {
private:
    std::vector<std::unique_ptr<Account>> accounts;

public:
    void addAccount(std::unique_ptr<Account> account);
    RemovalStatus removeAccount(int accountId);

    Account* findAccount(int accountId);
    bool isUsernameTaken(const std::string& username, int excludedAccountId) const;

    void displayAccounts() const;
    std::size_t getAccountCount() const;
};

#endif