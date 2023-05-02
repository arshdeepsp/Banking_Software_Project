#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "interface.cpp"

using namespace std;

int main() {
    string fileName = "accounts.txt";
    vector<Account> accounts = readAccountsFromFile(fileName);
    int choice;
    cout << "------------------------- BANK SOFTWARE -------------------------\n\n";
    do {
        cout << "1. Display all accounts" << endl;
        cout << "2. Create account" << endl;
        cout << "3. Delete account" << endl;
        cout << "4. Deposit" << endl;
        cout << "5. Withdraw" << endl;
        cout << "6. Get account balance" << endl;
        cout << "7. Exit\n" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cout << endl;
        while (choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 && choice != 7) {
            cout << "Enter (1-7) choice: ";
            cin >> choice;
            cout << endl;
        }
        switch (choice) {
            case 1:
                displayAccounts(accounts);
                break;
            case 2:
                createAccount(accounts, fileName);
                break;
            case 3:
                deleteAccount(accounts, fileName);
                break;
            case 4: {
                depositBalance(accounts, fileName);
                break;
            }
            case 5: {
                withdrawBalance(accounts, fileName);
                break;
            }
            case 6:
                getAccountBalance(accounts);
                break;
            case 7:
                cout << "Thank you! Goodbye.\n" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again.\n" << endl;
        }
        cout << "-----------------------------------------------------------------\n\n";
    } while (choice != 7);

    return 0;
}
