#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <map>

using namespace std;

// Structure to store account information
struct Account {
    string name;
    int accountNumber;
    string password;
    float balance;
    string isAdmin;
    string masterAdmin;
};

// PROTOTYPES

// Admin
bool checkAdminStatus(string, string, string);
bool makeAdmin(string, vector<Account>&, string);

// Utility
bool checkAccountNumberClash(vector<Account>&, int);
void displayAccounts(vector<Account>);

// I/O and search
vector<Account> readAccountsFromFile(string);
void writeAccountsToFile(string, vector<Account>);
Account findAccount(vector<Account>, int);

// UMS
void createAccount(vector<Account>&, string);
void createAccountFromData(vector<Account>&, Account, string);
void deleteAccount(vector<Account>&, string, bool);
void deleteAccountFromNumber(vector<Account>&, string, int);

// Transaction
void getAccountBalance(vector<Account>&, bool);
int getBalanceFromId(vector<Account>&, int, string);
bool depositBalanceFromNumber(vector<Account>&, string, int, float);
void depositBalance(vector<Account>&, string);
bool withdrawBalanceFromNumber(vector<Account>&, string, int, string, float);
void withdrawBalance(vector<Account>&, string);
void transfer(vector<Account>&, string);

// ADMIN

bool checkAdminStatus(string filename, int id_in, string password_in) {
    ifstream adminFile(filename);
    if (adminFile.is_open()) {
        int id;
        string password;
        while (adminFile >> id) {
            adminFile >> password;
            if (id == id_in) {
                if (password == password_in) {
                    cout << "Logged in as admin " << id << endl << endl;
                    return true;
                }
                cout << "Incorrect Password, try later\n\n";
                return false;
            }
        }
    }
    return false;
}

bool makeAdmin(string fileName, vector<Account>& accounts, int currentId) {
    Account currentAccount = findAccount(accounts, currentId);

    if (currentAccount.masterAdmin == "No") {
        cout << "Privilege forbidden for non-master admins.\n" << endl;
        return false;
    }

    int accountNumber;
    cout << "Enter account number: ";
    cin >> accountNumber;

    Account account = findAccount(accounts, accountNumber);

    if (account.accountNumber != 0) {
        account.isAdmin = "Yes";
        deleteAccountFromNumber(accounts, fileName, account.accountNumber);
        createAccountFromData(accounts, account, fileName);
        return true;
    } else {
        cout << "Account not found.\n" << endl;
    }

    return false;
}

// UTILITY FUNCTIONS - CHECKERS, FILE, I/O

// Check whether given account number clashes with another.
bool checkAccountNumberClash(vector<Account>& accounts, int numToCheck) {
    for (Account account : accounts) {
        if (numToCheck == account.accountNumber) return true;
    }
    return false;
}

// Function to display all accounts
void displayAccounts(vector<Account> accounts) {
    cout << "Full Name\t\tAccount Number\t\tBalance\t\tAdmin\t\tMaster?" << endl;
    cout << "----------------------------------------------------------------------------------------" << endl;
    if (accounts.size() == 0) cout << "                       - No Data -                     \n";
    for (Account account : accounts) {
        cout << account.name << "\t\t" << account.accountNumber << "\t\t\t" << account.balance << "\t\t" << account.isAdmin << "\t\t" << account.masterAdmin << endl;
    }
    cout << "----------------------------------------------------------------------------------------\n" << endl;
    cout << endl;
}

// Function to read account information from file
vector<Account> readAccountsFromFile(string fileName) {
    vector<Account> accounts;
    ifstream inputFile(fileName), passwords("passwords.txt");
    if (inputFile.is_open() && passwords.is_open()) {
        string fname, lname, password, isAdmin;
        Account account;
        while (inputFile >> fname) {
            inputFile >> lname;
            passwords >> password >> password >> password;
            account.name = fname + " " + lname;
            account.password = password;
            inputFile >> account.accountNumber >> account.balance >> account.isAdmin >> account.masterAdmin;
            accounts.push_back(account);
        }
        inputFile.close();
    }
    return accounts;
}

// Function to write account information to file
void writeAccountsToFile(string fileName, vector<Account> accounts) {
    ofstream outputFile(fileName), passwords("passwords.txt"), adminFile("admins.txt");
    if (outputFile.is_open()) {
        for (Account account : accounts) {
            outputFile << account.name << " " << account.accountNumber << " " << account.balance << " " << account.isAdmin << " " << account.masterAdmin << endl;
        }
        outputFile.close();
    }
    if (passwords.is_open()) {
        for (Account account : accounts) {
            passwords << account.name << " " << account.password << endl;
        }
        passwords.close();
    }
    if (adminFile.is_open()) {
        for (Account account : accounts) {
            if (account.isAdmin == "Yes") adminFile << account.accountNumber << " " << account.password << endl;
        }
        adminFile.close();
    }
}

// Function to find account by account number
Account findAccount(vector<Account> accounts, int accountNumber) {
    for (Account account : accounts) {
        if (account.accountNumber == accountNumber) {
            return account;
        }
    }
    Account emptyAccount;
    emptyAccount.accountNumber = 0;
    return emptyAccount;
}

// USER MANAGEMENT

// Function to create a new account
void createAccount(vector<Account>& accounts, string fileName) {
    string fname, lname, password;
    int accountNumber;
    float balance;
    cout << "Enter full name: ";
    cin >> fname >> lname;
    cout << "Enter initial balance: ";
    cin >> balance;
    cout << "Enter a password: ";
    cin >> password;

    do {
        if (accounts.size() == 9000000) {
            cout << "Bank database is full.\n\n";
            return;
        }
        accountNumber = (rand() % 1000000) + (1 + (rand() % 9))*1000000;
    } while (checkAccountNumberClash(accounts, accountNumber));

    Account account = {(fname+" "+lname), accountNumber, password, balance, "No", "No"};
    accounts.push_back(account);
    writeAccountsToFile(fileName, accounts);
    cout << "Account created successfully.\n" << endl;
}

// Function to create a new account from provided account
void createAccountFromData(vector<Account>& accounts, Account account, string fileName) {
    accounts.push_back(account);
    writeAccountsToFile(fileName, accounts);
}

// Function to delete an account
void deleteAccount(vector<Account>& accounts, string fileName, int currentId) {
    int accountNumber;
    string password;
    cout << "Enter account number: ";
    cin >> accountNumber;

    Account currentAccount = findAccount(accounts, currentId);

    for (auto it = accounts.begin(); it != accounts.end(); ++it) { // Type inference using "auto" type
        if (it->accountNumber == accountNumber) {
            if (currentAccount.isAdmin == "Yes") {
                if (it->isAdmin == "No" || currentAccount.masterAdmin == "Yes") {
                    if (it->masterAdmin == "No") {
                        accounts.erase(it);
                        writeAccountsToFile(fileName, accounts);
                        cout << "Account deleted successfully.\n" << endl;
                        return;
                    } else {
                        cout << "Cannot remove master administrator.\n" << endl;
                        return;
                    }
                } else {
                    cout << "Cannot remove system administrator without master access.\n" << endl;
                    return;
                }
            } else {
                cout << "Enter password: ";
                cin >> password;
                
                if (it->password == password) {
                    accounts.erase(it);
                    writeAccountsToFile(fileName, accounts);
                    cout << "Account deleted successfully.\n" << endl;
                    return;
                }
                
                cout << "Incorrect password.\n" << endl;
                return;
            }
        }
    }
    cout << "Account not found.\n" << endl;
}

// Function to delete an account from provided account number
void deleteAccountFromNumber(vector<Account>& accounts, string fileName, int accountNumber) {
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->accountNumber == accountNumber) {
            accounts.erase(it);
            writeAccountsToFile(fileName, accounts);
            return;
        }
    }
    cout << "Account not found.\n" << endl;
}


// BALANCES AND TRANSACTIONS

// Function to get an account's balance
void getAccountBalance(vector<Account>& accounts, bool isAdmin) {
    int accountNumber;
    string password;
    cout << "Enter account number: ";
    cin >> accountNumber;
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->accountNumber == accountNumber) {
            if (isAdmin) {
                cout << "Account balance: " << it->balance << endl << endl;
                return;
            } else {
                cout << "Enter password: ";
                cin >> password;
                
                if (it->password == password) {
                    cout << "Account balance: " << it->balance << endl << endl;
                    return;
                }
                
                cout << "Incorrect password.\n" << endl;
                return;
            }
        }
    }
    cout << "Account not found.\n" << endl;
}

int getBalanceFromId(vector<Account>& accounts, int accountNumber, string password) {
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->accountNumber == accountNumber) {
            if (it->password == password) {
                return it->balance;
            }
            
            cout << "Incorrect password.\n" << endl;
            return -1;
        }
    }
    cout << "Account not found.\n" << endl;
    return -1;
}

bool depositBalanceFromNumber(vector<Account>& accounts, string fileName, int accountNumber, float amount) {
    Account account = findAccount(accounts, accountNumber);
    if (account.accountNumber != 0) {
        account.balance += amount;
        deleteAccountFromNumber(accounts, fileName, account.accountNumber);
        createAccountFromData(accounts, account, fileName);
        return true;
    } else {
        cout << "Account not found.\n" << endl;
    }
    return false;
}

void depositBalance(vector<Account>& accounts, string fileName) {
    int accountNumber;
    string password;
    float amount;
    cout << "Enter account number: ";
    cin >> accountNumber;
    cout << "Enter password: ";
    cin >> password;
    Account account = findAccount(accounts, accountNumber);
    if (account.password == password) {
        if (account.accountNumber != 0) {
            cout << "Enter amount to deposit: ";
            cin >> amount;
            account.balance += amount;
            deleteAccountFromNumber(accounts, fileName, account.accountNumber);
            createAccountFromData(accounts, account, fileName);
            cout << "Deposit successful.\n" << endl;
        } else {
            cout << "Account not found.\n" << endl;
        }
        return;
    }
    cout << "Incorrect password.\n" << endl;
    return;
}

bool withdrawBalanceFromNumber(vector<Account>& accounts, string fileName, int accountNumber, string password, float amount) {
    Account account = findAccount(accounts, accountNumber);
    if (account.accountNumber != 0) {
        if (account.password == password) {
            if (account.balance >= amount) {
                account.balance -= amount;
                deleteAccountFromNumber(accounts, fileName, account.accountNumber);
                createAccountFromData(accounts, account, fileName);
                cout << "Initiated withdrawal from account " << accountNumber << endl << endl;
                return true;
            } else {
                cout << "Insufficient balance.\n" << endl;
            }
        } else {
            cout << "Incorrect password.\n" << endl;
        }
        return false;
    }
    cout << "Account not found.\n" << endl;
    return false;
}

void withdrawBalance(vector<Account>& accounts, string fileName) {
    int accountNumber;
    string password;
    float amount;

    cout << "Enter account number: ";
    cin >> accountNumber;
    cout << "Enter password: ";
    cin >> password;
    Account account = findAccount(accounts, accountNumber);
    if (account.password == password) {
        if (account.accountNumber != 0) {
            cout << "Enter amount to withdraw: ";
            cin >> amount;
            if (account.balance >= amount) {
                account.balance -= amount;
                deleteAccountFromNumber(accounts, fileName, account.accountNumber);
                createAccountFromData(accounts, account, fileName);
                cout << "Withdrawal successful.\n" << endl;
            } else {
                cout << "Insufficient balance.\n" << endl;
            }
        } else {
            cout << "Account not found.\n" << endl;
        }
        return;
    }
    cout << "Incorrect password.\n" << endl;
    return;
}

void transfer(vector<Account>& accounts, string fileName) {
    int accountFrom, accountTo;
    string password;
    float amount;

    cout << "Enter account number: ";
    cin >> accountFrom;
    cout << "Enter password: ";
    cin >> password;
    cout << "Enter amount to transfer: ";
    cin >> amount;

    bool status = withdrawBalanceFromNumber(accounts, fileName, accountFrom, password, amount);

    if (status) {
        cout << "Enter account number to transfer to: ";
        cin >> accountTo;

        bool success = depositBalanceFromNumber(accounts, fileName, accountTo, amount);

        if (success) {
            cout << "Transfer of " << amount << " successfully completed.\n" << endl;
        } else {
            depositBalanceFromNumber(accounts, fileName, accountFrom, amount);
            cout << "Transfer unsuccessful, try again later.\n" << endl;
        }
    } 
    
    else {
        cout << "Transfer unsuccessful, try again later.\n" << endl;
    }
}