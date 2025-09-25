#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
using namespace std;

// ===== Shared Globals =====
extern double balance;
extern const double MAX_BALANCE;
struct Card {
    string serial;
    double balance;
    Card(string s) : serial(s), balance(0.0) {}
};
extern vector<Card> cards;
struct Transaction {
    string type;
    string detail;
    double amount;
};
extern vector<Transaction> history;

void saveTNGToFile(); // from personB
bool isAlphabetAndSpace(const string &str); // from personA
bool isPhoneNumber(const string &str); // from personA
bool isValidAmount(const string &str); // from personA

// ===== Person D: Transfer =====
void transferMoney() {
    string name, phone, desc, input;
    double amount;

    while(true){
        cout << "Enter recipient name: "; getline(cin,name);
        if(isAlphabetAndSpace(name)) break;
        cout << "Invalid name.\n";
    }
    while(true){
        cout << "Enter recipient phone number (10-11 digits, no '-'): "; getline(cin,phone);
        if(isPhoneNumber(phone)) break;
        cout << "Invalid phone number.\n";
    }
    cout << "Enter transfer description/reason: "; getline(cin,desc);

    cout << "Enter transfer amount (RM, up to 2 decimals): ";
    getline(cin,input);
    if(!isValidAmount(input)){ cout << "Invalid input.\n"; return; }
    amount = stod(input);
    if(amount <=0 || amount>balance){ cout<<"Invalid amount.\n"; return; }

    balance -= amount;
    cout << "Transferred RM " << fixed << setprecision(2) << amount 
         << " to " << name << " (" << phone << ")"
         << ". Reason: " << desc << "\n";
    history.push_back({"Transfer", "To "+name+" "+phone+" Reason: "+desc, amount});
    saveTNGToFile();
}
