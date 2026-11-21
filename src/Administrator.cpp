#include "Seller.h"
#include <memory>
#include "Administrator.h"
#include "RemovalStatus.h"

Administrator::Administrator(int id, const std::string& username,
                             const std::string& password)
    : Account(id, username, password) {}


std::string Administrator:: getRole() const{
    return "Administrator";

}


void Administrator::createSeller(int id, const std::string& username,
                      const std::string& password, AccountManager& manager){
    manager.addAccount(std::make_unique <Seller>(id,username,password));

}


void Administrator::createAdministrator(int id, const std::string& username,
                             const std::string& password, AccountManager& manager){
    manager.addAccount(std::make_unique<Administrator>(id, username, password));



}


RemovalStatus Administrator::removeAccount(int accountId, AccountManager& manager){
    if(accountId <=0){
        return RemovalStatus::INVALID_ID;
    }
    if(accountId ==getId()){
        return RemovalStatus::SELF_REMOVAL;
    }
    return manager.removeAccount(accountId);
}

Account* Administrator::findAccount(int accountId,  AccountManager& manager){
   return manager.findAccount(accountId);
}


void Administrator::displayAccounts(const AccountManager& manager) const{
    manager.displayAccounts();

}

bool Administrator::approveProduct(int productId, Store& store){
    return store.approveProduct(productId);
}