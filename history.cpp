#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

extern double balance;
extern const double MAX_BALANCE;
extern struct Card;
extern vector<Card> cards;
extern struct Transaction;
extern vector<Transaction> history;

bool isValidAmount(const string &str);
bool isNumberOnly(const string &str);
void saveTNGToFile();

void reloadEWallet() {
    string input;
    double amount;
    while (true) {
        cout << "Enter reload amount for E-Wallet (min 20, max 3000, up to 2 decimals): ";
        getline(cin, input);
        if (isValidAmount(input)) {
            amount = stod(input);
            if (amount >= 20 && amount <= 3000) break;
        }
        cout << "Invalid amount.\n";
    }
    if (balance + amount > MAX_BALANCE) {
        cout << "Cannot exceed maximum E-Wallet balance RM 3000. Reload cancelled.\n";
        return;
    }
    balance += amount;
    cout << "E-Wallet successfully reloaded. New balance: RM " << fixed << setprecision(2) << balance << "\n";
    history.push_back({"E-Wallet Reload", "Reloaded to E-Wallet", amount});
    saveTNGToFile();
}

void reloadCard() {
    if (cards.empty()) { cout << "No cards available.\n"; return; }

    string input; int cardChoice;
    while(true){
        cout << "\n--- Select Card to Reload ---\n";
        for(size_t i=0;i<cards.size();i++)
            cout << i+1 << ". Serial: " << cards[i].serial << " | Balance: RM " << fixed << setprecision(2) << cards[i].balance << "\n";
        cout << "Enter card number (1-" << cards.size() << "): ";
        getline(cin,input);
        if(isNumberOnly(input)){
            cardChoice = stoi(input);
            if(cardChoice>=1 && cardChoice <= (int)cards.size()) break;
        }
        cout << "Invalid card selection.\n";
    }

    double amount;
    while(true){
        cout << "Enter reload amount for Card (min 20, max 3000, up to 2 decimals): ";
        getline(cin,input);
        if(isValidAmount(input)){
            amount = stod(input);
            if(amount>=20 && amount<=3000) break;
        }
        cout << "Invalid amount.\n";
    }

    if(balance >= amount){
        if(cards[cardChoice-1].balance + amount > MAX_BALANCE){
            cout << "Cannot exceed maximum card balance RM 3000. Reload cancelled.\n";
            return;
        }
        balance -= amount;
        cards[cardChoice-1].balance += amount;
        cout << "Card " << cards[cardChoice-1].serial << " reloaded with RM " 
             << fixed << setprecision(2) << amount 
             << ". Card balance: RM " << fixed << setprecision(2) << cards[cardChoice-1].balance
             << ", E-Wallet balance: RM " << fixed << setprecision(2) << balance << "\n";
        history.push_back({"Card Reload", "Card "+cards[cardChoice-1].serial, amount});
        saveTNGToFile();
    } else {
        cout << "Not enough balance in E-Wallet.\n";
    }
}