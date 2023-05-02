#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "main_interface.cpp"

// LARGE INPUT TEST
int main() {
    string ifile = "testinputs.txt", fname, lname;
    ifstream inputs(ifile);

    vector<Account> accounts;

    if (inputs.is_open()) {
        accounts = readAccountsFromFile("testout.txt");

        int counter = 0;

        while (inputs >> fname) {
            inputs >> lname;

            string name = (fname+" "+lname);
            Account account {name,0,"sample",500,"No","No"};

            counter++;

            cout << counter << ". ";
            createAccountFromStruct(accounts, "testout.txt", account);
        }

        // Table of tested inputs
        displayAccounts(accounts);

        inputs.close();
    }

    return 0;
}