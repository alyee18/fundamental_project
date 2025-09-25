#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

// ================= Global =================
extern double balance;
extern const double MAX_BALANCE;

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