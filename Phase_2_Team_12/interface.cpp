#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

// Structure to store account information
struct Account {
    string name;
    int accountNumber;
    string password;
    float balance;
};

// HELPER FUNCTIONS - CHECKERS, FILE, I/O

// Check whether given account number clashes with another.
bool checkAccountNumberClash(vector<Account>& accounts, int numToCheck) {
    for (Account account : accounts) {
        if (numToCheck == account.accountNumber) return true;
    }
    return false;
}

// Function to display all accounts
void displayAccounts(vector<Account> accounts) {
    cout << "Full Name\t\tAccount Number\t\tBalance" << endl;
    cout << "-------------------------------------------------------------" << endl;
    if (accounts.size() == 0) cout << "                       - No Data -                     \n";
    for (Account account : accounts) {
        cout << account.name << "\t\t" << account.accountNumber << "\t\t\t" << account.balance << endl;
    }
    cout << "-------------------------------------------------------------\n" << endl;
    cout << endl;
}

// Function to read account information from file
vector<Account> readAccountsFromFile(string fileName) {
    vector<Account> accounts;
    ifstream inputFile(fileName), passwords("passwords.txt");
    if (inputFile.is_open() && passwords.is_open()) {
        string fname, lname, password;
        Account account;
        while (inputFile >> fname) {
            inputFile >> lname;
            passwords >> password >> password >> password;
            account.name = fname + " " + lname;
            account.password = password;
            inputFile >> account.accountNumber >> account.balance;
            accounts.push_back(account);
        }
        inputFile.close();
    }
    return accounts;
}

// Function to write account information to file
void writeAccountsToFile(string fileName, vector<Account> accounts) {
    ofstream outputFile(fileName), passwords("passwords.txt");
    if (outputFile.is_open()) {
        for (Account account : accounts) {
            outputFile << account.name << " " << account.accountNumber << " " << account.balance << endl;
        }
        outputFile.close();
    }
    if (passwords.is_open()) {
        for (Account account : accounts) {
            passwords << account.name << " " << account.password << endl;
        }
        passwords.close();
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
    return emptyAccount;
}

// ACCOUNT CREATION/DELETION

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

    Account account = {(fname+" "+lname), accountNumber, password, balance};
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
void deleteAccount(vector<Account>& accounts, string fileName) {
    int accountNumber;
    string password;
    cout << "Enter account number: ";
    cin >> accountNumber;
    for (auto it = accounts.begin(); it != accounts.end(); ++it) { // Type inference using "auto" type
        if (it->accountNumber == accountNumber) {
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


// ACCOUNT BALANCE

// Function to get an account's balance
void getAccountBalance(vector<Account>& accounts) {
    int accountNumber;
    string password;
    cout << "Enter account number: ";
    cin >> accountNumber;
    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if (it->accountNumber == accountNumber) {
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
    cout << "Account not found.\n" << endl;
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