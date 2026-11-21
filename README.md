
# MiniStore — Inventory & Sales Management

**A C++17 console-based inventory and sales management application developed by DaemonLabs.**

MiniStore is an object-oriented C++ application that simulates a small marketplace where administrators manage accounts and approve product proposals, sellers manage product listings and inventory, and customers browse available offers and manage shopping carts.

The project was developed to practise and demonstrate object-oriented programming, class relationships, memory management, polymorphism, and modular C++ application development.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [User Roles](#user-roles)
- [Architecture](#architecture)
- [Technologies](#technologies)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Building the Project](#building-the-project)
- [Debugging](#debugging)
- [Application Workflow](#application-workflow)
- [Limitations](#limitations)
- [Future Improvements](#future-improvements)
- [License](#license)
- [Author](#author)

---

## Overview

MiniStore separates products from seller-specific listings.

A product represents an item in the store's catalog, while a seller listing represents a particular seller's offer for that product, including its price and available stock.

This design allows multiple sellers to offer the same product independently, each with their own price and inventory.

The application uses an in-memory architecture. Accounts, products, listings, and shopping carts are managed during execution without a database or persistent storage.

---

## Features

### Account Management

- Customer registration.
- User authentication.
- Three account roles: Administrator, Seller, and Customer.
- Administrator account creation and removal.
- Seller account creation.
- Account identification and role-based menu access.
- Username and password modification at the account-class level.

### Product Management

- Product catalog management.
- Product proposals submitted by sellers.
- Administrator approval of proposed products.
- Product identification through unique IDs.
- Separation of product information from seller-specific offers.

### Seller Management

- Creation of product listings.
- Independent pricing for each seller's listing.
- Stock restocking.
- Listing price updates.
- Removal of personal listings.
- Display of individual seller listings.

### Shopping Cart

- Browse products and seller listings.
- Add products from different sellers.
- Remove individual cart items.
- Remove all items associated with a seller or product.
- Update item quantities.
- Increase or decrease quantities.
- Clear the shopping cart.
- Display cart contents and item subtotals.
- Calculate the total number of items and cart value.
- Check listing availability and stock constraints.

---

## User Roles

### Administrator

Administrators are responsible for managing the application.

Their available operations include:

- Displaying registered accounts.
- Creating seller accounts.
- Creating administrator accounts.
- Removing accounts, subject to self-removal restrictions.
- Displaying catalog products.
- Displaying all seller listings.
- Approving product proposals.

### Seller

Sellers manage their own product offers.

Their available operations include:

- Browsing catalog products.
- Displaying personal listings.
- Creating product listings.
- Proposing new products.
- Updating listing prices.
- Restocking listings.
- Removing personal listings.

### Customer

Customers interact with the marketplace and manage their shopping carts.

Their available operations include:

- Browsing products.
- Browsing seller listings.
- Viewing shopping cart contents.
- Adding items to the cart.
- Removing items.
- Updating quantities.
- Increasing or decreasing quantities.
- Clearing the cart.

---

## Architecture

MiniStore follows an object-oriented design with responsibilities distributed across several classes.

### Account System

`Account` is an abstract base class containing shared account information and authentication functionality.

It is inherited by:

- `Administrator`
- `Seller`
- `Customer`

Each derived class implements its own role identification through polymorphism.

`AccountManager` manages account ownership and operations using:

```cpp
std::vector<std::unique_ptr<Account>>
```

### Product and Listing System

`Product` represents catalog information, including its ID and name.

`SellerListing` represents a seller's offer and contains:

- Product ID.
- Seller ID.
- Price.
- Stock quantity.
- Active/inactive status.

The `Store` class manages:

- Catalog products.
- Seller listings.
- Pending product proposals.

It uses `std::unique_ptr` to manage the lifetime of dynamically allocated products and listings.

### Shopping Cart System

`CartItem` represents an item selected by a customer.

Each cart item is identified by both its product ID and seller ID, allowing the same product to appear in a cart through different sellers.

`ShoppingCart` manages cart operations and calculations.

`Customer` owns a `ShoppingCart` object through composition and delegates cart-related operations to it.

### Other Components

| Component | Responsibility |
|---|---|
| `RemovalStatus` | Represents possible account-removal outcomes. |
| `Store` | Manages products, listings, and product proposals. |
| `AccountManager` | Manages registered accounts. |
| `main.cpp` | Application entry point, menus, input handling, and role-based navigation. |

---

## Technologies

- **Language:** C++17
- **Compiler:** GCC / G++
- **Compiler distribution:** MSYS2 UCRT64
- **Debugger:** GDB
- **IDE / Editor:** Visual Studio Code
- **Build automation:** PowerShell
- **Version control:** Git

### C++ Concepts Used

- Classes and objects.
- Encapsulation.
- Inheritance.
- Abstraction.
- Polymorphism.
- Composition.
- Constructors and destructors.
- Virtual functions and abstract classes.
- Standard Template Library (STL).
- Vectors and algorithms.
- Smart pointers (`std::unique_ptr`).
- Exception handling.
- Forward declarations.
- Header and source file separation.

---

## Project Structure

```text
inventory & sales management/
│
├── .vscode/
│   ├── tasks.json
│   ├── launch.json
│   └── build.ps1
│
├── build/
│   └── inventory & sales management.exe
│
├── include/
│   ├── Account.h
│   ├── AccountManager.h
│   ├── Administrator.h
│   ├── CartItem.h
│   ├── Customer.h
│   ├── Product.h
│   ├── RemovalStatus.h
│   ├── Seller.h
│   ├── SellerListing.h
│   ├── ShoppingCart.h
│   └── Store.h
│
├── src/
│   ├── Account.cpp
│   ├── AccountManager.cpp
│   ├── Administrator.cpp
│   ├── CartItem.cpp
│   ├── Customer.cpp
│   ├── main.cpp
│   ├── Product.cpp
│   ├── Seller.cpp
│   ├── SellerListing.cpp
│   ├── ShoppingCart.cpp
│   └── Store.cpp
│
├── .gitignore
├── LICENSE
└── README.md
```

The `build/` directory and executable files are generated outputs and are excluded from version control.

---

## Requirements

To build and run MiniStore, you need:

- Windows.
- Visual Studio Code.
- Microsoft C/C++ extension for VS Code.
- MSYS2 UCRT64 environment.
- GCC/G++ with C++17 support.
- GDB for debugging.
- PowerShell.

The current build and debugging configurations expect the following executable locations:

```text
C:\msys64\ucrt64\bin\g++.exe
C:\msys64\ucrt64\bin\gcc.exe
C:\msys64\ucrt64\bin\gdb.exe
```

No external libraries or database systems are required.

---

## Building the Project

### Using Visual Studio Code

1. Open the project root directory in Visual Studio Code.
2. Open a C++ source file, preferably `src/main.cpp`.
3. Press `Ctrl + Shift + B`.
4. Select `C++: Build Executable` if necessary.

The build task calls the custom PowerShell script:

```text
.vscode/build.ps1
```

The script automatically discovers the project's source files, selects the appropriate compiler, applies the C++17 standard, and generates the executable.

The resulting executable is placed in:

```text
build/inventory & sales management.exe
```

### Build Modes

The custom build script supports three modes:

| Mode | Description |
|---|---|
| `Auto` | Automatically determines whether to build an executable or object files. |
| `Executable` | Compiles and links the source files into an executable. |
| `Object` | Compiles source files individually into object files. |

The `Auto` mode detects the presence of `main()` to determine the appropriate build operation.

### Compiler Flags

The build script uses compiler options including:

```text
-std=c++17
-Wall
-Wextra
-Wpedantic
-g
```

These enable C++17, common compiler warnings, additional pedantic warnings, and debugging information.

---

## Debugging

MiniStore includes a VS Code debugging configuration using GDB.

To start debugging:

1. Open the project in Visual Studio Code.
2. Set breakpoints where needed.
3. Press `F5`.
4. VS Code automatically executes the `C++: Build Executable` task before launching the debugger.

The debugging configuration is defined in:

```text
.vscode/launch.json
```

The debugger uses:

- GDB through the `cppdbg` debugger interface.
- An external console for application input and output.
- The project root as the working directory.
- Automatic compilation before debugging.

---

## Application Workflow

The application follows this general workflow:

1. The application starts and creates an initial administrator account.
2. The user accesses the main menu.
3. The user can log in, register as a customer, or exit.
4. After authentication, the application displays the menu corresponding to the user's role.
5. Administrators manage accounts, products, listings, and product approvals.
6. Sellers manage their listings, prices, stock, and product proposals.
7. Customers browse available offers and manage their shopping carts.
8. Users can log out and return to the main menu.

---

## Limitations

MiniStore is currently a console-based project and has several limitations.

### Data Persistence

All application data is stored in memory.

Accounts, products, listings, and shopping carts are lost when the application terminates.

There is currently no database or file-based persistence system.

### No Checkout or Payment System

Customers can manage their shopping carts and calculate totals, but the application does not implement:

- Order creation.
- Checkout processing.
- Payment processing.
- Purchase history.
- Automatic stock deduction following a completed purchase.

### Security

Passwords are currently stored directly in memory as plaintext.

A production-ready implementation would require secure password hashing and additional authentication protections.

### User Interface

The application uses a console interface. There is no graphical user interface or web interface.

### Validation and Business Rules

Some validations could be strengthened, including:

- Rejecting non-finite prices such as NaN and infinity.
- Verifying that a seller ID belongs to an existing Seller account when creating a listing.
- Improving consistency checks between cart contents and listing availability.

### Scalability

Several operations use linear searches through vectors.

For larger datasets, more efficient data structures and database-backed operations would be preferable.

---

## Future Improvements

Potential improvements for future versions include:

- Database integration using SQLite or MySQL.
- Persistent account, product, listing, and cart storage.
- Secure password hashing.
- Order management and checkout.
- Payment integration.
- Purchase history.
- Improved inventory and transaction management.
- Graphical user interface.
- More comprehensive validation and error handling.
- Automated unit and integration tests.
- More efficient data structures.

These are possible future extensions, not features currently implemented.

---

## License

MiniStore is distributed under the **DaemonLabs Non-Commercial License (DNCL) 1.0**.

The license permits free personal and educational use, modification, and non-commercial redistribution, provided the original copyright notice and license are retained.

Commercial use, commercial redistribution, or commercial exploitation requires prior written permission from DaemonLabs.

See the [`LICENSE`](LICENSE) file for the complete terms.

---

## Author

**DaemonLabs**

GitHub: [https://github.com/daemonlabs-pixel](https://github.com/daemonlabs-pixel)

---

*Developed with C++17 as an object-oriented programming project by DaemonLabs.*
