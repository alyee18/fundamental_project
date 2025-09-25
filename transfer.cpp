#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

extern double balance;
extern struct Transaction;
extern vector<Transaction> history;

bool isAlphabetAndSpace(const string &str);
bool isPhoneNumber(const string &str);
bool isValidAmount(const string &str);
void saveTNGToFile();

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
