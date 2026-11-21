
#include <iostream>
#include <string>
#include <memory>
#include <exception>
#include <limits>
#include <sstream>
#include <utility>

#include "Store.h"
#include "AccountManager.h"
#include "Administrator.h"
#include "Seller.h"
#include "Customer.h"
#include "Product.h"
#include "CartItem.h"
#include "SellerListing.h"
#include "RemovalStatus.h"

// ============================================================
// FUNCTION DECLARATIONS
// ============================================================

// Input helpers
int readInt(const std::string& prompt);
double readDouble(const std::string& prompt);
std::string readText(const std::string& prompt);

// Account management
void createInitialAdministrator(AccountManager& manager);
void registerCustomer(AccountManager& manager);
Account* login(AccountManager& manager);

// Menus
void mainMenu(Store& store, AccountManager& manager);
void administratorMenu(Administrator& admin, Store& store,
                       AccountManager& manager);
void sellerMenu(Seller& seller, Store& store);
void customerMenu(Customer& customer, Store& store,
                  AccountManager& manager);

// ============================================================
// MAIN
// ============================================================

int main() {

    // Initialize the application.
    Store store;
    AccountManager manager;

    try {
        // Create the first Administrator.
        createInitialAdministrator(manager);

        std::cout << "\nMiniStore initialized successfully!\n";

        // Start the application.
        mainMenu(store, manager);
    }
    catch (const std::exception& e) {
        std::cerr << "\nFatal error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}

// ============================================================
// INPUT HELPERS
// ============================================================

// Reads an integer and rejects invalid input.
int readInt(const std::string& prompt) {

    std::string line;

    while (true) {
        std::cout << prompt;

        if (!std::getline(std::cin, line)) {
            throw std::runtime_error("Input stream closed.");
        }

        std::istringstream input(line);
        int value;
        char extra;

        if (input >> value && !(input >> extra)) {
            return value;
        }

        std::cout << "Invalid input. Please enter an integer.\n";
    }
}

// Reads a decimal number.
double readDouble(const std::string& prompt) {

    std::string line;

    while (true) {
        std::cout << prompt;

        if (!std::getline(std::cin, line)) {
            throw std::runtime_error("Input stream closed.");
        }

        std::istringstream input(line);
        double value;
        char extra;

        if (input >> value && !(input >> extra)) {
            return value;
        }

        std::cout << "Invalid input. Please enter a number.\n";
    }
}

// Reads a complete line of text.
std::string readText(const std::string& prompt) {

    std::string value;

    std::cout << prompt;

    if (!std::getline(std::cin, value)) {
        throw std::runtime_error("Input stream closed.");
    }

    return value;
}

// ============================================================
// INITIAL ADMINISTRATOR
// ============================================================

void createInitialAdministrator(AccountManager& manager) {

    bool adminCreated = false;

    std::cout << "\n================================\n";
    std::cout << "     MINISTORE INITIAL SETUP\n";
    std::cout << "================================\n";

    do {

        try {
            std::string username = readText("Enter username: ");
            std::string password = readText("Enter password: ");

            // The first account receives ID 1.
            auto admin = std::make_unique<Administrator>(
                1, username, password
            );

            // Transfer ownership to AccountManager.
            manager.addAccount(std::move(admin));

            adminCreated = true;

            std::cout << "\nAdministrator created successfully!\n";
        }
        catch (const std::exception& e) {

            std::cout << "\nAccount creation failed: "
                      << e.what() << '\n';

            std::cout << "Please try again.\n\n";
        }

    } while (!adminCreated);
}

// ============================================================
// CUSTOMER REGISTRATION
// ============================================================

void registerCustomer(AccountManager& manager) {

    try {

        std::cout << "\n=== Customer Registration ===\n";

        int id = readInt("Enter account ID: ");
        std::string username = readText("Enter username: ");
        std::string password = readText("Enter password: ");

        auto customer = std::make_unique<Customer>(
            id, username, password
        );

        manager.addAccount(std::move(customer));

        std::cout << "\nRegistration successful!\n";
    }
    catch (const std::exception& e) {

        std::cout << "\nRegistration failed: "
                  << e.what() << '\n';
    }
}

// ============================================================
// LOGIN
// ============================================================

Account* login(AccountManager& manager) {

    std::cout << "\n=== Login ===\n";

    int id = readInt("Account ID: ");
    std::string password = readText("Password: ");

    Account* account = manager.findAccount(id);

    if (account == nullptr) {
        std::cout << "Account not found.\n";
        return nullptr;
    }

    if (!account->authenticate(password)) {
        std::cout << "Incorrect password.\n";
        return nullptr;
    }

    std::cout << "\nWelcome, "
              << account->getUsername() << "!\n";

    return account;
}

// ============================================================
// MAIN MENU
// ============================================================

void mainMenu(Store& store, AccountManager& manager) {

    bool running = true;

    while (running) {

        std::cout << "\n================================\n";
        std::cout << "          MINISTORE\n";
        std::cout << "================================\n";
        std::cout << "1. Login\n";
        std::cout << "2. Register as Customer\n";
        std::cout << "0. Exit\n";

        int choice = readInt("Choose an option: ");

        switch (choice) {

            case 1: {

                Account* account = login(manager);

                if (account == nullptr) {
                    break;
                }

                // Direct the account to its corresponding menu.
                if (auto* admin = dynamic_cast<Administrator*>(account)) {
                    administratorMenu(*admin, store, manager);
                }
                else if (auto* seller = dynamic_cast<Seller*>(account)) {
                    sellerMenu(*seller, store);
                }
                else if (auto* customer = dynamic_cast<Customer*>(account)) {
                    customerMenu(*customer, store, manager);
                }

                break;
            }

            case 2:
                registerCustomer(manager);
                break;

            case 0:
                running = false;
                std::cout << "\nThank you for using MiniStore!\n";
                break;

            default:
                std::cout << "Invalid option.\n";
        }
    }
}

// ============================================================
// ADMINISTRATOR MENU
// ============================================================

void administratorMenu(Administrator& admin, Store& store,
                       AccountManager& manager) {

    bool running = true;

    while (running) {

        std::cout << "\n================================\n";
        std::cout << "      ADMINISTRATOR MENU\n";
        std::cout << "================================\n";
        std::cout << "1. Display accounts\n";
        std::cout << "2. Create Seller account\n";
        std::cout << "3. Create Administrator account\n";
        std::cout << "4. Remove account\n";
        std::cout << "5. Display products\n";
        std::cout << "6. Display all listings\n";
        std::cout << "7. Approve product proposal\n";
        std::cout << "0. Logout\n";

        int choice = readInt("Choose an option: ");

        try {

            switch (choice) {

                case 1:
                    admin.displayAccounts(manager);
                    break;

                case 2: {

                    int id = readInt("Seller ID: ");
                    std::string username = readText("Username: ");
                    std::string password = readText("Password: ");

                    admin.createSeller(id, username, password, manager);

                    std::cout << "Seller created successfully.\n";
                    break;
                }

                case 3: {

                    int id = readInt("Administrator ID: ");
                    std::string username = readText("Username: ");
                    std::string password = readText("Password: ");

                    admin.createAdministrator(
                        id, username, password, manager
                    );

                    std::cout << "Administrator created successfully.\n";
                    break;
                }

                case 4: {

                    int id = readInt("Account ID to remove: ");

                    RemovalStatus status = admin.removeAccount(id, manager);

                    switch (status) {

                        case RemovalStatus::SUCCESS:
                            std::cout << "Account removed successfully.\n";
                            break;

                        case RemovalStatus::INVALID_ID:
                            std::cout << "Invalid account ID.\n";
                            break;

                        case RemovalStatus::ACCOUNT_NOT_FOUND:
                            std::cout << "Account not found.\n";
                            break;

                        case RemovalStatus::SELF_REMOVAL:
                            std::cout << "You cannot remove your own account.\n";
                            break;
                    }

                    break;
                }

                case 5:
                    store.displayProducts();
                    break;

                case 6:
                    store.displayListings();
                    break;

                case 7: {

                    int productId = readInt("Product ID to approve: ");

                    if (admin.approveProduct(productId, store)) {
                        std::cout << "Product approved successfully.\n";
                    }
                    else {
                        std::cout << "Product proposal not found.\n";
                    }

                    break;
                }

                case 0:
                    running = false;
                    std::cout << "Administrator logged out.\n";
                    break;

                default:
                    std::cout << "Invalid option.\n";
            }
        }
        catch (const std::exception& e) {

            std::cout << "Operation failed: " << e.what() << '\n';
        }
    }
}

// ============================================================
// SELLER MENU
// ============================================================

void sellerMenu(Seller& seller, Store& store) {

    bool running = true;

    while (running) {

        std::cout << "\n================================\n";
        std::cout << "          SELLER MENU\n";
        std::cout << "================================\n";
        std::cout << "1. Display available products\n";
        std::cout << "2. Display my listings\n";
        std::cout << "3. Create a listing\n";
        std::cout << "4. Propose a new product\n";
        std::cout << "5. Update listing price\n";
        std::cout << "6. Restock a listing\n";
        std::cout << "7. Remove a listing\n";
        std::cout << "0. Logout\n";

        int choice = readInt("Choose an option: ");

        try {

            switch (choice) {

                case 1:
                    store.displayProducts();
                    break;

                case 2:
                    seller.displayListings(store);
                    break;

                case 3: {

                    int productId = readInt("Product ID: ");
                    double price = readDouble("Price: ");

                    seller.createListing(productId, price, store);

                    std::cout << "Listing created successfully.\n";
                    break;
                }

                case 4: {

                    int productId = readInt("New product ID: ");
                    std::string name = readText("Product name: ");

                    auto product = std::make_unique<Product>(
                        productId, name
                    );

                    seller.proposeProduct(std::move(product), store);

                    std::cout << "Product proposal submitted.\n";
                    break;
                }

                case 5: {

                    int productId = readInt("Product ID: ");
                    double price = readDouble("New price: ");

                    seller.updateListingPrice(productId, price, store);

                    std::cout << "Price updated successfully.\n";
                    break;
                }

                case 6: {

                    int productId = readInt("Product ID: ");
                    int quantity = readInt("Quantity to add: ");

                    seller.restockListing(productId, quantity, store);

                    std::cout << "Stock updated successfully.\n";
                    break;
                }

                case 7: {

                    int productId = readInt("Product ID: ");

                    if (seller.removeListing(productId, store)) {
                        std::cout << "Listing removed successfully.\n";
                    }
                    else {
                        std::cout << "Listing not found.\n";
                    }

                    break;
                }

                case 0:
                    running = false;
                    std::cout << "Seller logged out.\n";
                    break;

                default:
                    std::cout << "Invalid option.\n";
            }
        }
        catch (const std::exception& e) {

            std::cout << "Operation failed: " << e.what() << '\n';
        }
    }
}

// ============================================================
// CUSTOMER MENU
// ============================================================

void customerMenu(Customer& customer, Store& store,
                  AccountManager& manager) {

    bool running = true;

    while (running) {

        std::cout << "\n================================\n";
        std::cout << "         CUSTOMER MENU\n";
        std::cout << "================================\n";
        std::cout << "1. Browse products\n";
        std::cout << "2. Browse seller listings\n";
        std::cout << "3. View shopping cart\n";
        std::cout << "4. Add item to cart\n";
        std::cout << "5. Remove item from cart\n";
        std::cout << "6. Update item quantity\n";
        std::cout << "7. Increase item quantity\n";
        std::cout << "8. Decrease item quantity\n";
        std::cout << "9. Clear shopping cart\n";
        std::cout << "0. Logout\n";

        int choice = readInt("Choose an option: ");

        try {

            switch (choice) {

                case 1:
                    store.displayProducts();
                    break;

                case 2:
                    store.displayListings();
                    break;

                case 3:
                    customer.viewCart(store, manager);
                    break;

                case 4: {

                    int productId = readInt("Product ID: ");
                    int sellerId = readInt("Seller ID: ");
                    int quantity = readInt("Quantity: ");

                    CartItem item(productId, sellerId, quantity);

                    customer.addItemToCart(item, store);

                    std::cout << "Item added to cart successfully.\n";
                    break;
                }

                case 5: {

                    int productId = readInt("Product ID: ");
                    int sellerId = readInt("Seller ID: ");

                    if (customer.removeItem(productId, sellerId)) {
                        std::cout << "Item removed successfully.\n";
                    }
                    else {
                        std::cout << "Item not found in your cart.\n";
                    }

                    break;
                }

                case 6: {

                    int productId = readInt("Product ID: ");
                    int sellerId = readInt("Seller ID: ");
                    int quantity = readInt("New quantity (0 to remove): ");

                    if (customer.updateQuantity(
                            productId, sellerId, quantity, store)) {
                        std::cout << "Quantity updated successfully.\n";
                    }
                    else {
                        std::cout << "Item not found in your cart.\n";
                    }

                    break;
                }

                case 7: {

                    int productId = readInt("Product ID: ");
                    int sellerId = readInt("Seller ID: ");
                    int amount = readInt("Quantity to add: ");

                    if (customer.increaseQuantity(
                            productId, sellerId, amount, store)) {
                        std::cout << "Quantity increased successfully.\n";
                    }
                    else {
                        std::cout << "Item not found in your cart.\n";
                    }

                    break;
                }

                case 8: {

                    int productId = readInt("Product ID: ");
                    int sellerId = readInt("Seller ID: ");
                    int amount = readInt("Quantity to remove: ");

                    if (customer.decreaseQuantity(
                            productId, sellerId, amount)) {
                        std::cout << "Quantity decreased successfully.\n";
                    }
                    else {
                        std::cout << "Item not found in your cart.\n";
                    }

                    break;
                }

                case 9:
                    customer.clearCart();
                    break;

                case 0:
                    running = false;
                    std::cout << "Customer logged out.\n";
                    break;

                default:
                    std::cout << "Invalid option.\n";
            }
        }
        catch (const std::exception& e) {

            std::cout << "Operation failed: " << e.what() << '\n';
        }
    }
}
