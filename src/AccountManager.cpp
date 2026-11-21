#include "AccountManager.h"

#include <iostream>
#include <stdexcept>
#include <utility>

void AccountManager::addAccount(std::unique_ptr<Account> account) {
    if (!account) {
        throw std::invalid_argument("Invalid account");
    }

    for (const auto& existingAccount : accounts) {
        if (existingAccount->getId() == account->getId()) {
            throw std::invalid_argument("Account ID already registered");
        }

        if (existingAccount->getUsername() == account->getUsername()) {
            throw std::invalid_argument("Username already taken");
        }
    }

    accounts.push_back(std::move(account));
}


RemovalStatus AccountManager::removeAccount(int accountId) {
    if (accountId <= 0) {
        return RemovalStatus::INVALID_ID;
    }

    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if ((*it)->getId() == accountId) {
            accounts.erase(it);
            return RemovalStatus::SUCCESS;
        }
    }

    return RemovalStatus::ACCOUNT_NOT_FOUND;
}


Account* AccountManager::findAccount(int accountId) {
    if (accountId <= 0) {
        throw std::invalid_argument("Invalid account ID");
    }

    for (const auto& account : accounts) {
        if (account->getId() == accountId) {
            return account.get();
        }
    }

    return nullptr;
}


bool AccountManager::isUsernameTaken(
    const std::string& username, int excludedAccountId) const {

    if (username.find_first_not_of(" \t\n\r") == std::string::npos ||
        excludedAccountId <= 0) {
        throw std::invalid_argument("Invalid username or account ID");
    }

    for (const auto& account : accounts) {
        if (account->getId() != excludedAccountId &&
            account->getUsername() == username) {
            return true;
        }
    }

    return false;
}


void AccountManager::displayAccounts() const {
    if (accounts.empty()) {
        std::cout << "No accounts registered.\n";
        return;
    }

    for (const auto& account : accounts) {
        std::cout << account->toString() << '\n';
        std::cout << "-------------------------\n";
    }
}


std::size_t AccountManager::getAccountCount() const {
    return accounts.size();
}