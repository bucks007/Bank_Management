#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class User {
private:
    string fullName, fatherName, motherName, username, accountNumber, mobileNumber, email, aadhaar, dob, accountType, ifscCode;
    float balance;
    vector<string> statement;

public:
    User() : balance(0) {}
    
    User(string fName, string faName, string moName, string user, string accNum, 
         string mobile, string email, string aadhaar, string dob, string accountType, string ifsc, float bal = 0)
        : fullName(fName), fatherName(faName), motherName(moName), username(user),
          accountNumber(accNum), mobileNumber(mobile), email(email), aadhaar(aadhaar), dob(dob),
          accountType(accountType), ifscCode(ifsc), balance(bal) {}

    string getUsername() const { return username; }
    string getAccountNumber() const { return accountNumber; }
    float getBalance() const { return balance; }
    string getFullName() const { return fullName; }

    void deposit(float amount) {
        balance += amount;
        statement.push_back("Deposited: " + to_string(amount));
    }

    bool withdraw(float amount) {
        if (amount > balance) return false;
        balance -= amount;
        statement.push_back("Withdrawn: " + to_string(amount));
        return true;
    }

    bool transfer(float amount, User& recipient) {
        if (amount > balance) return false;
        balance -= amount;
        recipient.balance += amount;
        statement.push_back("Transferred: " + to_string(amount) + " to Account " + recipient.accountNumber);
        recipient.statement.push_back("Received: " + to_string(amount) + " from Account " + accountNumber);
        return true;
    }

    void viewStatement() {
        cout << "\n\n\t\t\t\t=============================\n";
        cout << "\t\t\t\t||    ACCOUNT STATEMENT\t||";
        cout << "\n\t\t\t\t=============================\n";
        cout << "\nAccount Statement for " << fullName << " (" << accountNumber << "):\n";
        for (const auto& entry : statement) cout << entry << endl;
        if (statement.empty()) cout << "No transactions yet.\n";
    }

    void saveToFile(ofstream& ofs) {
        ofs << fullName << " " << fatherName << " " << motherName << " " << getUsername() << " " 
            << accountNumber << " " << mobileNumber << " " << email << " " << aadhaar << " " 
            << dob << " " << accountType << " " << ifscCode << " " << balance << endl;
    }

    void loadFromFile(ifstream& ifs) {
        ifs >> fullName >> fatherName >> motherName >> username >> accountNumber
            >> mobileNumber >> email >> aadhaar >> dob >> accountType >> ifscCode >> balance;
    }
};

// Function to generate a unique account number (6 digits)
string generateAccountNumber() {
    static int accountNumber = 100000;
    return to_string(accountNumber++);
}

// Function to generate IFSC code based on branch
string generateIFSC(const string& branch) {
    if (branch == "Amritsar") return "ABC001";
    if (branch == "Delhi") return "DEF002";
    if (branch == "Chandigarh") return "GHI003";
    if (branch == "Patna") return "JKL004";
    if (branch == "Bangalore") return "MNO005";
    return "UNK000";
}

bool isValidMobileNumber(const string& mobileNumber) {
    return mobileNumber.length() == 10 && all_of(mobileNumber.begin(), mobileNumber.end(), ::isdigit);
}

bool isValidAadhaar(const string& aadhaar) {
    return aadhaar.length() == 12 && all_of(aadhaar.begin(), aadhaar.end(), ::isdigit);
}

// Function to create account with a minimum deposit of 1000
void createAccount(User users[], int& userCount) {
    string fullName, fatherName, motherName, username, accountNumber, mobileNumber, email, aadhaar, dob, accountType, branch;
    float initialDeposit;

    cout << "\n\n\t\t\t\t============================\n";
    cout << "\t\t\t\t||     CREATE ACCOUNT  \t||";
    cout << "\n\t\t\t\t============================\n";
    cout << "Enter full name: "; cin.ignore(); getline(cin, fullName);
    cout << "Enter father's name: "; getline(cin, fatherName);
    cout << "Enter mother's name: "; getline(cin, motherName);
    cout << "Enter username: "; cin >> username;
    accountNumber = generateAccountNumber();

    do {
        cout << "Enter mobile number (10 digits): "; cin >> mobileNumber;
    } while (!isValidMobileNumber(mobileNumber));

    cout << "Enter email: "; cin >> email;

    do {
        cout << "Enter Aadhaar number (12 digits): "; cin >> aadhaar;
    } while (!isValidAadhaar(aadhaar));

    cout << "Enter date of birth (YYYY-MM-DD): "; cin >> dob;
    cout << "Enter account type (Savings/Current): "; cin >> accountType;

    cout << "Enter branch name: "; cin.ignore(); getline(cin, branch);
    string ifscCode = generateIFSC(branch);

    cout << "Enter initial deposit (min 1000): "; cin >> initialDeposit;
    if (initialDeposit < 1000) {
        cout << "Minimum deposit is 1000. Account not created.\n";
        return;
    }

    users[userCount++] = User(fullName, fatherName, motherName, username, accountNumber,
                               mobileNumber, email, aadhaar, dob, accountType, ifscCode, initialDeposit);
    
    // Save the user data to a file
    ofstream ofs("UserData.txt", ios::app); // Open file in append mode
    if (ofs.is_open()) {
        users[userCount - 1].saveToFile(ofs); // Save the newly created user data
        ofs.close(); // Close the file
    } else {
        cout << "Error: Unable to open file to save data.\n";
    }
    cout << "\n\t\t\t========================================\n";
    cout << "\t\t\t   Account created successfully!";
    cout << "\n\t\t\t   Account Number: " << accountNumber;
    cout << "\n\t\t\t   IFSC Code: " << ifscCode;
    cout << "\n\t\t\t========================================\n";
}

// Function to login a user
User* login(User users[], int userCount) {
    string username;

    cout << "\n\n\t\t\t\t======================\n";
    cout << "||\t\t\t\t      LOGIN\t\t\t\t||";
    cout << "\n\t\t\t\t======================\n";
    cout << "Enter username: "; cin >> username;

    for (int i = 0; i < userCount; i++) {
        if (users[i].getUsername() == username ) {
            return &users[i];
        }
    }
    cout << "Invalid username.\n";
    return nullptr;........................................
}

// Main function
int main() {
    User users[100];
    int userCount = 0;

    ifstream ifs("UserData.txt");
    if (ifs.is_open()) {
        while (!ifs.eof()) {
            users[userCount].loadFromFile(ifs);
            if (ifs) userCount++;
        }
        ifs.close();
    }
    while (true) {
        cout << "\n\n\t\t\t\t===============================\n";
        cout << "\t\t\t\t||   BANK MANAGEMENT SYSTEM  ||";
        cout << "\n\t\t\t\t===============================\n";
        cout << "\n1. Create Account\n2. Login\n3. Exit\nChoose an option: ";
        int choice; cin >> choice;

        switch (choice) {
            case 1: createAccount(users, userCount); break;
            case 2: {
                User* loggedInUser = login(users, userCount);
                if (loggedInUser) {
                    cout << "\n\t\t\t\t=====================\n";
                    cout << "\t\t\t\t||   LOGIN SUCCESSFUL  ||";
                    cout << "\n\t\t\t\t=====================\n";

                    while (true) {
                        cout << "\n1. Check Balance\n2. Deposit\n3. Withdraw\n4. Transfer\n"
                             << "5. View Statement\n6. Logout\nChoose an option: ";
                        int userChoice; cin >> userChoice;

                        switch (userChoice) {
                            case 1: cout << "Your balance is: " << loggedInUser->getBalance() << endl; break;
                            case 2: {
                                float depositAmount; cout << "Enter amount to deposit: "; cin >> depositAmount;
                                loggedInUser->deposit(depositAmount); break;
                            }
                            case 3: {
                                float withdrawAmount; cout << "Enter amount to withdraw: "; cin >> withdrawAmount;
                                cout << (loggedInUser->withdraw(withdrawAmount) ? "Withdrawal successful." : "Insufficient balance.") << endl;
                                break;
                            }
                            case 4: {
                                string recipientAccount; float transferAmount;
                                cout << "Enter recipient account number: "; cin >> recipientAccount;
                                cout << "Enter amount to transfer: "; cin >> transferAmount;
                                User* recipient = nullptr;
                                for (int i = 0; i < userCount; i++) {
                                    if (users[i].getAccountNumber() == recipientAccount) {
                                        recipient = &users[i]; break;
                                    }
                                }
                                cout << (recipient && loggedInUser->transfer(transferAmount, *recipient) ? "Transfer successful." : "Transfer failed.") << endl;
                                break;
                            }
                            case 5: loggedInUser->viewStatement(); break;
                            case 6: cout << "Logging out...\n"; goto endLogin;
                            default: cout << "Invalid choice. Please try again.\n"; break;
                        }
                    }
                }
                endLogin: break;
            }
            case 3: return 0;
            default: cout << "Invalid choice. Please try again.\n"; break;
        }
    }
    return 0;
}
