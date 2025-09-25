#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
using namespace std;

// ================= Global =================
double balance = 0.0; // E-Wallet starts at 0
const double MAX_BALANCE = 3000.0; // Maximum balance limit

struct Card {
    string serial;
    double balance;
    Card(string s) : serial(s), balance(0.0) {}
};

vector<Card> cards;

struct Transaction {
    string type;    // E-Wallet Reload, Card Reload, Payment, Transfer
    string detail;  // info like card serial, recipient, reason
    double amount;
};

vector<Transaction> history;

// ================= Helpers =================
bool isNumberOnly(const string &str) {
    if (str.empty()) return false;
    for (char c : str) if (!isdigit(c)) return false;
    return true;
}

bool isAlphabetAndSpace(const string &str) {
    if (str.empty()) return false;
    for (char c : str) if (!isalpha(c) && c != ' ') return false;
    return true;
}

bool isPhoneNumber(const string &str) {
    if (str.size() < 10 || str.size() > 11) return false;
    for (char c : str) if (!isdigit(c)) return false; // no '-' allowed
    return true;
}

bool isValidAmount(const string &str) {
    if (str.empty()) return false;
    int dotCount = 0, decimals = 0;
    for (char c : str) {
        if (c == '.') {
            dotCount++;
            if (dotCount > 1) return false;
            decimals = 0;
        } else if (isdigit(c)) {
            if (dotCount == 1) decimals++;
            if (decimals > 2) return false;
        } else return false;
    }
    return true;
}

void showEWalletBalance() {
    cout << "E-Wallet Balance: RM " << fixed << setprecision(2) << balance << "\n";
}

// ================= Save =================
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
        file << history[i].type << " | " << history[i].detail << " | RM " << fixed << setprecision(2) << history[i].amount << "\n";

    file.close();
}

// ================= Reload E-Wallet =================
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

// ================= Reload Card =================
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

// ================= Transfer =================
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
         << " to " << name << " (" << phone << ", 10-11 digits, no '-')"
         << ". Reason: " << desc << "\n";
    history.push_back({"Transfer", "To "+name+" "+phone+" Reason: "+desc, amount});
    saveTNGToFile();
}

// ================= Display Transaction History =================
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

// ================= Main =================
int main(){
    string input; int choice;
    do{
        cout<<"\n===== TNG E-Wallet System =====\n";
        showEWalletBalance();
        cout<<"1. Reload TNG E-Wallet\n2. Card Management\n3. Transport Payment\n4. Retail Payment\n5. Check Transaction History\n6. Transfer\n7. Exit\nEnter choice: ";
        getline(cin,input);
        if(!isNumberOnly(input)){ cout<<"Invalid input.\n"; continue; }
        choice=stoi(input);

        switch(choice){

            case 1: reloadEWallet(); break;

            case 2: {
                int subChoice;
                do{
                    cout<<"\n--- Card Management ---\n";
                    if(!cards.empty()){
                        cout<<"\n--- Existing Cards ---\n";
                        for(size_t i=0;i<cards.size();i++)
                            cout<<i+1<<". Serial: "<<cards[i].serial<<" | Balance: RM "<<fixed<<setprecision(2)<<cards[i].balance<<"\n";
                    } else cout<<"No cards available.\n";

                    cout<<"\n1. Add Card\n2. Delete Card\n3. Reload Card\n4. Back\nEnter choice: ";
                    getline(cin,input);
                    if(!isNumberOnly(input)){ cout<<"Invalid input.\n"; continue; }
                    subChoice=stoi(input);

                    if(subChoice==1){
                        if(cards.size()>=15){ cout<<"Maximum 15 cards.\n"; continue; }
                        while(true){
                            cout<<"Enter 10-digit card serial: "; getline(cin,input);
                            if(!(isNumberOnly(input) && input.size()==10)){ cout<<"Invalid serial.\n"; continue; }
                            bool exists=false;
                            for(auto &c: cards) if(c.serial==input){ exists=true; break; }
                            if(exists){ cout<<"Card serial already exists.\n"; continue; }
                            cards.push_back(Card(input));
                            cout<<"Card added successfully.\n";
                            saveTNGToFile();
                            break;
                        }
                    }
                    else if(subChoice==2){
                        if(cards.empty()){ cout<<"No cards to delete.\n"; continue; }
                        while(true){
                            cout<<"Enter card number to delete (1-"<<cards.size()<<"): "; getline(cin,input);
                            if(isNumberOnly(input)){
                                int idx=stoi(input);
                                if(idx>=1 && idx<=(int)cards.size()){
                                    if(cards[idx-1].balance>0.0){
                                        cout<<"Card has RM "<<fixed<<setprecision(2)<<cards[idx-1].balance<<" balance. Delete anyway? (Y/N): ";
                                        char conf; cin>>conf; cin.ignore(1000,'\n');
                                        if(conf=='Y'||conf=='y'){ 
                                            cout<<"Card deleted.\n"; 
                                            cards.erase(cards.begin()+idx-1); saveTNGToFile(); break;
                                        } else { cout<<"Deletion cancelled.\n"; break; }
                                    } else {
                                        cout<<"Card deleted.\n"; cards.erase(cards.begin()+idx-1); saveTNGToFile(); break;
                                    }
                                }
                            }
                            cout<<"Invalid card number.\n";
                        }
                    }
                    else if(subChoice==3) reloadCard();
                    else if(subChoice==4) break;
                    else cout<<"Invalid choice.\n";
                }while(subChoice!=4);
                break;
            }

            case 3: { // Transport Payment
                if(cards.empty()){ cout<<"No cards available.\n"; break; }
                int vehicleChoice;
                double cost; string vehicleType;
                while(true){
                    cout<<"\nSelect Vehicle Type for Toll Payment:\n1. Car (RM3.50)\n2. 4-wheel Lorry (RM7.00)\n3. >4-wheel Lorry (RM10.50)\n4. Taxi (RM2.30)\n5. Bus (RM3.00)\nEnter choice: ";
                    getline(cin,input);
                    if(isNumberOnly(input)){ vehicleChoice=stoi(input); if(vehicleChoice>=1&&vehicleChoice<=5) break; }
                    cout<<"Invalid input.\n";
                }
                switch(vehicleChoice){
                    case 1: cost=3.50; vehicleType="Car"; break;
                    case 2: cost=7.0; vehicleType="4-wheel Lorry"; break;
                    case 3: cost=10.50; vehicleType="More than 4-wheel Lorry"; break;
                    case 4: cost=2.30; vehicleType="Taxi"; break;
                    case 5: cost=3.0; vehicleType="Bus"; break;
                }

                int cardChoice;
                while(true){
                    cout<<"\n--- Select Card for "<<vehicleType<<" Toll ---\n";
                    for(size_t i=0;i<cards.size();i++)
                        cout<<i+1<<". Serial: "<<cards[i].serial<<" | Balance: RM "<<fixed<<setprecision(2)<<cards[i].balance<<"\n";
                    cout<<"Enter card number: "; getline(cin,input);
                    if(isNumberOnly(input)){ cardChoice=stoi(input); if(cardChoice>=1 && cardChoice<=(int)cards.size()) break; }
                    cout<<"Invalid card selection.\n";
                }

                Card &selectedCard=cards[cardChoice-1];
                if(selectedCard.balance>=cost){
                    selectedCard.balance-=cost;
                    cout<<vehicleType<<" Toll paid from Card "<<selectedCard.serial<<". Deducted RM "<<fixed<<setprecision(2)<<cost<<"\n";
                    history.push_back({"Transport Payment", vehicleType+" using Card "+selectedCard.serial, cost});
                    saveTNGToFile();
                } else { char resp;
                    cout<<"Balance not enough. Reload? (Y/N): "; cin>>resp; cin.ignore(1000,'\n');
                    if(resp=='Y'||resp=='y') reloadCard();
                }
                break;
            }

            case 4: { // Retail Payment
                int method;
                while(true){
                    cout<<"\nSelect Retail Payment Method:\n1. E-Wallet\n2. Card\nEnter choice: ";
                    getline(cin,input);
                    if(isNumberOnly(input)){ method=stoi(input); if(method==1||method==2) break; }
                    cout<<"Invalid input.\n";
                }

                cout<<"Enter retail payment amount (RM, up to 2 decimals): ";
                getline(cin,input);
                if(!isValidAmount(input)){ cout<<"Invalid amount.\n"; break; }
                double cost=stod(input);

                if(method==1){
                    if(balance>=cost){ balance-=cost; 
                        cout<<"Retail payment of RM "<<fixed<<setprecision(2)<<cost<<" paid using E-Wallet.\n";
                        history.push_back({"Retail Payment","Paid using E-Wallet",cost}); saveTNGToFile();
                    } else { char resp; 
                        cout<<"Balance not enough. Reload? (Y/N): "; cin>>resp; cin.ignore(1000,'\n');
                        if(resp=='Y'||resp=='y') reloadEWallet();
                    }
                } else {
                    if(cards.empty()){ cout<<"No cards available.\n"; break; }
                    int cardChoice;
                    while(true){
                        cout<<"\n--- Select Card ---\n";
                        for(size_t i=0;i<cards.size();i++)
                            cout<<i+1<<". Serial: "<<cards[i].serial<<" | Balance: RM "<<fixed<<setprecision(2)<<cards[i].balance<<"\n";
                        cout<<"Enter card number: "; getline(cin,input);
                        if(isNumberOnly(input)){ cardChoice=stoi(input); if(cardChoice>=1 && cardChoice<=(int)cards.size()) break; }
                        cout<<"Invalid card selection.\n";
                    }
                    Card &selectedCard=cards[cardChoice-1];
                    if(selectedCard.balance>=cost){
                        selectedCard.balance-=cost;
                        cout<<"Retail payment of RM "<<fixed<<setprecision(2)<<cost<<" paid from Card "<<selectedCard.serial<<".\n";
                        history.push_back({"Retail Payment","Paid using Card "+selectedCard.serial,cost});
                        saveTNGToFile();
                    } else { char resp;
                        cout<<"Balance not enough. Reload? (Y/N): "; cin>>resp; cin.ignore(1000,'\n');
                        if(resp=='Y'||resp=='y') reloadCard();
                    }
                }
                break;
            }

            case 5: showTransactionHistory(); break;

            case 6: transferMoney(); break;

            case 7: cout<<"Exiting program...\n"; break;

            default: cout<<"Invalid choice.\n";
        }

    }while(choice!=7);

    return 0;
}
