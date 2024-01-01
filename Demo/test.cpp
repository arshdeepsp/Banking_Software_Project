#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "main_interface.cpp"

using namespace std;

string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

// LARGE INPUT TEST w/ 2000 inputs
int main() {
    string ifile = "testinputs.txt", fname, lname;
    ifstream inputs(ifile);

    vector<Account> accounts;

    if (inputs.is_open()) {
        accounts = readAccountsFromFile("testout.txt");

        int counter = 0;

        while (inputs >> fname) {
            inputs >> lname;

            string password = gen_random(10);

            string name = (fname+" "+lname);
            Account account {name,0,password,500,"No","No"};

            counter++;

            cout << counter << ". ";
            createAccountFromStruct(accounts, "testout.txt", account);
        }

        cout << endl;
        // Table of tested inputs
        displayAccounts(accounts);

        inputs.close();
    }

    return 0;
}