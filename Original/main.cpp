#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "main_interface.cpp"

using namespace std;

int main() {
    bool loginAsAdmin = false;
    char res;
    int id;
    string password;

    cout << "Enter as admin? (Y/n): ";
    cin >> res;
    while (res != 'Y' && res != 'N' && res != 'y' && res != 'n') {
        cout << "Invalid input (Y/n): ";
        cin >> res;
    }

    if (res == 'Y' || res == 'y') {
        cout << "Enter admin id: ";
        cin >> id;
        cout << "Enter admin passcode: ";
        cin >> password;

        string adminFile = "admins.txt";
        loginAsAdmin = checkAdminStatus(adminFile, id, password);
    }

    string fileName = "accounts.txt";
    vector<Account> accounts = readAccountsFromFile(fileName);
    int choice;
    cout << "------------------------- BANK SOFTWARE -------------------------\n\n";
    do {
        if (loginAsAdmin) cout << "0. Make admin" << endl;
        cout << "1. Display all accounts" << endl;
        cout << "2. Create account" << endl;
        cout << "3. Delete account" << endl;
        cout << "4. Deposit" << endl;
        cout << "5. Withdraw" << endl;
        cout << "6. Transfer" << endl;
        cout << "7. Get account balance" << endl;
        cout << "8. Exit\n" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cout << endl;
        while (choice != 0 && choice != 1 && choice != 2 && choice != 3 && choice != 4 && choice != 5 && choice != 6 && choice != 7 && choice != 8) {
            cout << "Enter (0-8) choice: ";
            cin >> choice;
            cout << endl;
        }
        switch (choice) {
            case 0:
                makeAdmin(fileName, accounts, id);
                break;
            case 1:
                if (loginAsAdmin) displayAccounts(accounts);
                else cout << "Admin access required!\n\n";
                break;
            case 2:
                createAccount(accounts, fileName);
                break;
            case 3:
                deleteAccount(accounts, fileName, id);
                break;
            case 4: {
                depositBalance(accounts, fileName);
                break;
            }
            case 5: {
                withdrawBalance(accounts, fileName);
                break;
            }
            case 6: {
                transfer(accounts, fileName);
                break;
            }
            case 7:
                getAccountBalance(accounts, loginAsAdmin);
                break;
            case 8:
                cout << "Thank you! Goodbye.\n" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again.\n" << endl;
        }
        cout << "-----------------------------------------------------------------\n\n";
    } while (choice != 8);

    return 0;
}
