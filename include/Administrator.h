#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <string>

#include "Account.h"
#include "AccountManager.h"
#include "RemovalStatus.h"
#include "Store.h"


class Administrator : public Account {
public:
    Administrator(int id, const std::string& username,
                  const std::string& password);

    std::string getRole() const override;

    void createSeller(int id, const std::string& username,
                      const std::string& password, AccountManager& manager);

    void createAdministrator(int id, const std::string& username,
                             const std::string& password, AccountManager& manager);

    RemovalStatus removeAccount(int accountId, AccountManager& manager);

    Account* findAccount(int accountId, AccountManager& manager);

    void displayAccounts(const AccountManager& manager) const;
    bool approveProduct(int productId,Store& store);
};

#endif