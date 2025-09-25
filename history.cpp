#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
using namespace std;

// Helper function to check if a string contains only digits
bool isNumberOnly(const string& s) {
    return !s.empty() && s.find_first_not_of("0123456789") == string::npos;
}

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

// ===== Person B: Save & History =====
void saveTNGToFile() {
    ofstream file("TNG.txt");
    if (!file) {
        cout << "Error opening TNG.txt for saving.\n";
        return;
    }
    file << "=== E-Wallet Balance ===\n";
    file << "RM " << fixed << setprecision(2) << balance << "\n\n";

    file << "=== Cards ===\n";
    if(cards.empty()) file << "(No cards)\n";
    else for(size_t i=0;i<cards.size();i++)
        file << cards[i].serial << " | RM " << fixed << setprecision(2) << cards[i].balance << "\n";

    file << "\n=== Transaction History ===\n";
    if(history.empty()) file << "(No transactions yet)\n";
    else for(size_t i=0;i<history.size();i++)
        file << history[i].type << " | " << history[i].detail << " | RM " 
             << fixed << setprecision(2) << history[i].amount << "\n";
    file.close();
}

void showTransactionHistory() {
    if(history.empty()){ cout<<"No transactions yet.\n"; return; }

    int cat;
    while(true){
        cout<<"\n--- Transaction History ---\n";
        cout<<"1. E-Wallet Reload\n2. Card Reload\n3. Payments (Transport & Retail)\n4. Transfer\n5. All\nEnter choice: ";
        string input; getline(cin,input);
        if(isNumberOnly(input)){ cat=stoi(input); if(cat>=1 && cat<=5) break; }
        cout<<"Invalid input.\n";
    }

    for(size_t i=0;i<history.size();i++){
        bool show=false;
        if(cat==1 && history[i].type=="E-Wallet Reload") show=true;
        else if(cat==2 && history[i].type=="Card Reload") show=true;
        else if(cat==3 && (history[i].type=="Transport Payment" || history[i].type=="Retail Payment")) show=true;
        else if(cat==4 && history[i].type=="Transfer") show=true;
        else if(cat==5) show=true;

        if(show)
            cout<<i+1<<". "<<history[i].type<<" | "<<history[i].detail<<" | RM "<<fixed<<setprecision(2)<<history[i].amount<<"\n";
    }
}
