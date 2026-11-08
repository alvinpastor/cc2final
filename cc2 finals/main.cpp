
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

// Transaction structure
struct Transaction {
    int id;
    string type;
    string category;
    string description;
    string date;
    double amount;
};

// Global variables
vector<Transaction> transactions;

double budget = 0.0;

const string FILE_NAME = "transactions.txt";
const string BUDGET_FILE = "budget.txt";

// Function declarations
int getInt(string message);
double getDouble(string message);
string getText(string message);

void loadData();
void saveData();

int findTransaction(int id);

void addIncome();
void addExpense();
void viewTransactions();
void searchTransaction();
void deleteTransaction();
void setBudget();
void budgetSummary();

void addTransaction(string type);

// Main function
int main() {

    loadData();

    int choice;

    do {

        cout << "\n========================================\n";
        cout << "     PERSONAL BUDGET MANAGEMENT SYSTEM\n";
        cout << "========================================\n";

        cout << "[1] Add Income\n";
        cout << "[2] Add Expense\n";
        cout << "[3] View Transactions\n";
        cout << "[4] Search Transaction\n";
        cout << "[5] Delete Transaction\n";
        cout << "[6] Set Budget\n";
        cout << "[7] Budget Summary\n";
        cout << "[8] Exit\n";

        cout << "========================================\n";

        choice = getInt("Enter your choice: ");

        switch (choice) {

            case 1:
                addIncome();
                break;

            case 2:
                addExpense();
                break;

            case 3:
                viewTransactions();
                break;

            case 4:
                searchTransaction();
                break;

            case 5:
                deleteTransaction();
                break;

            case 6:
                setBudget();
                break;

            case 7:
                budgetSummary();
                break;

            case 8:
                saveData();
                cout << "\nThank you for using the system!\n";
                break;

            default:
                cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 8);

    return 0;
}

// Integer input
int getInt(string message) {

    int value;

    while (true) {

        cout << message;

        if (cin >> value) {

            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );

            return value;
        }

        cout << "Invalid input. Enter a whole number.\n";

        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(), '\n'
        );
    }
}

// Decimal input
double getDouble(string message) {

    double value;

    while (true) {

        cout << message;

        if (cin >> value &&
            value > 0 &&
            value <= 1000000000000.0) {

            cin.ignore(
                numeric_limits<streamsize>::max(), '\n'
            );

            return value;
        }

        cout << "Invalid amount. Enter a positive amount.\n";

        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(), '\n'
        );
    }
}

// Text input
string getText(string message) {

    string value;

    while (true) {

        cout << message;

        getline(cin, value);

        if (!value.empty() &&
            value.find('|') == string::npos) {

            return value;
        }

        cout << "Invalid input. Please try again.\n";
    }
}

// Find transaction using ID
int findTransaction(int id) {

    for (int i = 0; i < transactions.size(); i++) {

        if (transactions[i].id == id) {

            return i;
        }
    }

    return -1;
}

// Add income
void addIncome() {

    addTransaction("Income");
}

// Add expense
void addExpense() {

    addTransaction("Expense");
}

// General transaction function
void addTransaction(string type) {

    Transaction t;

    int highestID = 0;

    for (const Transaction &item : transactions) {

        if (item.id > highestID) {
            highestID = item.id;
        }
    }

    if (highestID == numeric_limits<int>::max()) {
        cout << "Cannot create more transaction IDs.\n";
        return;
    }

    t.id = highestID + 1;

    t.type = type;

    cout << "\n========== ADD " << type << " ==========\n";

    t.category = getText("Enter Category: ");

    t.description = getText("Enter Description: ");

    t.amount = getDouble("Enter Amount (PHP): ");

    t.date = getText("Enter Date (YYYY-MM-DD): ");

    transactions.push_back(t);

    saveData();

    cout << "\n" << type << " successfully recorded!\n";
    cout << "Transaction ID: " << t.id << endl;
}

// View all transactions
void viewTransactions() {

    cout << "\n========== TRANSACTION HISTORY ==========\n";

    if (transactions.empty()) {

        cout << "No transactions available.\n";
        return;
    }

    cout << left
         << setw(6) << "ID"
         << setw(12) << "TYPE"
         << setw(16) << "CATEGORY"
         << setw(15) << "AMOUNT"
         << setw(13) << "DATE"
         << "DESCRIPTION\n";

    cout << string(85, '-') << endl;

    for (const Transaction &t : transactions) {

        cout << left
             << setw(6) << t.id
             << setw(12) << t.type
             << setw(16) << t.category
             << setw(15) << fixed << setprecision(2)
             << t.amount
             << setw(13) << t.date
             << t.description << endl;
    }
}

// Search transaction
void searchTransaction() {

    int id = getInt("\nEnter Transaction ID: ");

    int index = findTransaction(id);

    if (index == -1) {

        cout << "Transaction not found.\n";
        return;
    }

    Transaction t = transactions[index];

    cout << "\n========== TRANSACTION FOUND ==========\n";

    cout << "ID: " << t.id << endl;
    cout << "Type: " << t.type << endl;
    cout << "Category: " << t.category << endl;
    cout << "Description: " << t.description << endl;
    cout << "Amount: PHP " << fixed << setprecision(2)
         << t.amount << endl;
    cout << "Date: " << t.date << endl;
}

// Delete transaction
void deleteTransaction() {

    int id = getInt("\nEnter Transaction ID to delete: ");

    int index = findTransaction(id);

    if (index == -1) {

        cout << "Transaction not found.\n";
        return;
    }

    transactions.erase(transactions.begin() + index);

    saveData();

    cout << "Transaction successfully deleted!\n";
}

// Set spending budget
void setBudget() {

    cout << "\n========== SET BUDGET ==========\n";

    budget = getDouble("Enter your budget (PHP): ");

    saveData();

    cout << "Budget successfully updated!\n";
}

// Budget summary
void budgetSummary() {

    double totalIncome = 0;
    double totalExpenses = 0;

    for (const Transaction &t : transactions) {

        if (t.type == "Income") {

            totalIncome += t.amount;
        }
        else if (t.type == "Expense") {

            totalExpenses += t.amount;
        }
    }

    double balance = totalIncome - totalExpenses;

    double remainingBudget = budget - totalExpenses;

    cout << "\n========== BUDGET SUMMARY ==========\n";

    cout << fixed << setprecision(2);

    cout << "Total Income:       PHP " << totalIncome << endl;

    cout << "Total Expenses:     PHP " << totalExpenses << endl;

    cout << "Current Balance:    PHP " << balance << endl;

    cout << "Spending Budget:    PHP " << budget << endl;

    cout << "Remaining Budget:   PHP " << remainingBudget << endl;

    cout << "====================================\n";

    if (budget == 0) {

        cout << "Please set a budget first.\n";
    }
    else if (remainingBudget < 0) {

        cout << "WARNING: You exceeded your budget!\n";
    }
    else if (remainingBudget == 0) {

        cout << "You have reached your budget limit.\n";
    }
    else {

        cout << "You are still within your budget.\n";
    }
}

// Save data to files
void saveData() {

    ofstream file(FILE_NAME);

    if (!file) {
        cout << "Error saving transactions!\n";
        return;
    }

    for (const Transaction &t : transactions) {

        file << t.id << "|"
             << t.type << "|"
             << t.category << "|"
             << t.description << "|"
             << t.amount << "|"
             << t.date << endl;
    }

    file.close();

    ofstream budgetFile(BUDGET_FILE);

    if (!budgetFile) {
        cout << "Error saving budget!\n";
        return;
    }

    budgetFile << budget;

    budgetFile.close();
}

// Load saved data
void loadData() {

    ifstream file(FILE_NAME);

    string line;

    while (getline(file, line)) {

        Transaction t;

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        size_t p4 = line.find('|', p3 + 1);
        size_t p5 = line.find('|', p4 + 1);

        if (p1 == string::npos ||
            p2 == string::npos ||
            p3 == string::npos ||
            p4 == string::npos ||
            p5 == string::npos) {

            continue;
        }

        try {

            t.id = stoi(line.substr(0, p1));

            t.type = line.substr(p1 + 1, p2 - p1 - 1);

            t.category = line.substr(p2 + 1, p3 - p2 - 1);

            t.description = line.substr(p3 + 1, p4 - p3 - 1);

            t.amount = stod(line.substr(p4 + 1, p5 - p4 - 1));

            t.date = line.substr(p5 + 1);

            if (t.id > 0 &&
                (t.type == "Income" || t.type == "Expense") &&
                t.amount > 0 &&
                findTransaction(t.id) == -1) {

                transactions.push_back(t);
            }

        }
        catch (...) {

            continue;
        }
    }

    file.close();

    ifstream budgetFile(BUDGET_FILE);

    if (budgetFile) {

        double savedBudget;

        if (budgetFile >> savedBudget &&
            savedBudget >= 0 &&
            savedBudget <= 1000000000000.0) {

            budget = savedBudget;
        }

        budgetFile.close();
    }
}
