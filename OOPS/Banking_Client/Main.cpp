// INCLUDE DIRECTIVES
#include <map>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "BranchManager.h"

using namespace std;

// GLOBAL VARIABLES
string HELP_MSG = "bruh\n";
string CURR_USER = "";
string DATE = "17-10-2004";

BranchManager& ROOT_USER = BranchManager::getInstance();

map <string, int> GLOBAL_COMMANDS;
map <string, int> USER_COMMANDS;
map <string, int> ROOT_COMMANDS;

// UTILITY FUNCTIONS

string get_holder_file_path(string username) {

    string file_path = "AccountHolders/";
    file_path.append(username);
    file_path.append(".bin");

    return file_path;
}

string take_non_empty_input(string prompt) {

    string non_empty_input;
    cout << prompt;

    getline(cin, non_empty_input, '\n');

    while (non_empty_input == "") {
        cout << "Input cannot be empty!\n";
        cout << prompt;
        getline(cin, non_empty_input, '\n');
    }

    return non_empty_input;
}

int take_non_empty_int(string prompt) {

    string raw = take_non_empty_input(prompt);
    int number = stoi(raw);

    while (number <= 0) {
        cout << "Input must be a positive integer!\n";
        raw = take_non_empty_input(prompt);
        number = stoi(raw);
    }

    return number;
}

int generate_account_number() {
    int acc_num = take_non_empty_int("Enter account number: ");
    return acc_num;
}

// COMMAND MAPS

void setup_commands() {

    GLOBAL_COMMANDS ["help"]        = 1;
    GLOBAL_COMMANDS ["exit"]        = 2;
    GLOBAL_COMMANDS ["signup"]      = 3;
    GLOBAL_COMMANDS ["login"]       = 4;
    GLOBAL_COMMANDS ["root-login"]  = 5;

    ROOT_COMMANDS   ["logout"]      = 1;
    ROOT_COMMANDS   ["getacc"]      = 2;
    ROOT_COMMANDS   ["getstat"]     = 3;
    ROOT_COMMANDS   ["ffwd"]        = 4;

    USER_COMMANDS   ["logout"]      = 1;
    USER_COMMANDS   ["chpass"]      = 2;
    USER_COMMANDS   ["cracc"]       = 3;
    USER_COMMANDS   ["getacc"]      = 4;
    USER_COMMANDS   ["deposit"]     = 5;
    USER_COMMANDS   ["withdraw"]    = 6;

}

// WRAPPERS FOR GLOBAL COMMANDS

void help() {
    cout << HELP_MSG;
}

void quit() {
    cout << "Exitting Application\n";
    exit(0);
}

void signup() {

    string input_username = take_non_empty_input("Enter username: ");

    string file_path = get_holder_file_path(input_username);
    bool exist = filesystem::exists(file_path);

    if (exist) {
        cout << "User already exists!\n";
        return;
    }

    string input_password = take_non_empty_input("Enter password: ");
    string input_name = take_non_empty_input("Enter name: ");

    BankAccountHolder new_user = BankAccountHolder(input_username, input_password, input_name);
    new_user.write(file_path);
}

void login() {

    string input_username = take_non_empty_input("Enter username: ");

    string file_path = get_holder_file_path(input_username);
    bool exist = filesystem::exists(file_path);

    if (!exist) {
        cout << "User does not exist!\n";
        return;
    }

    BankAccountHolder curr_user = BankAccountHolder();
    curr_user.read(file_path);

    string input_password = take_non_empty_input("Enter password: ");

    if (curr_user.login_try(input_password)) CURR_USER = curr_user.username;
    else cout << "Invalid password!\n";

}

void root_login() {

    string input_password = take_non_empty_input("Enter root password: ");

    if (ROOT_USER.login_try(input_password)) CURR_USER = "root";
    else cout << "Invalid password!\n";

}

// WRAPPERS FOR USER COMMANDS

void logout() {
    CURR_USER = "";
}

void chpass() {

    string file_path = get_holder_file_path(CURR_USER);
    string curr_pass = take_non_empty_input("Enter current password: ");
    
    BankAccountHolder curr_user = BankAccountHolder();
    curr_user.read(file_path);


    if (curr_user.login_try(curr_pass)) {

        string new_pass = take_non_empty_input("Enter new password: ");
        curr_user.change_password(new_pass);
        curr_user.write(file_path);

    }

    else cout << "Invalid password!\n";

}

void cracc() {

    string file_path = get_holder_file_path(CURR_USER);
    BankAccountHolder curr_user = BankAccountHolder();
    curr_user.read(file_path);

    string acc_type = take_non_empty_input("Enter account type: ");

    while((acc_type != "savings") && (acc_type != "current")) {
        cout << "Enter a valid account type\n";
        acc_type = take_non_empty_input("Enter acocunt type: ");
    }

    int balance = take_non_empty_int("Enter opening balance: ");

    curr_user.create_account(generate_account_number(), balance, DATE, acc_type);
    curr_user.write(file_path);

}

void getacc() {

    string file_path = get_holder_file_path(CURR_USER);
    BankAccountHolder curr_user = BankAccountHolder();
    curr_user.read(file_path);

    string acc_type = take_non_empty_input("Enter the account type: ");
    
    if (acc_type == "savings") {
    
        SavingsAccount * curr_user_acc_list = curr_user.get_savings_accounts();

        for (int i = 0; i < curr_user.num_of_savings_acc; i++) {
            cout << (curr_user_acc_list[i]).acc_number << " " << (curr_user_acc_list[i]).balance << "\n";
        }

    }
    
    else if (acc_type == "current") {

        CurrentAccount * curr_user_acc_list = curr_user.get_current_accounts();

        for (int i = 0; i < curr_user.num_of_current_acc; i++) {
            cout << (curr_user_acc_list[i]).acc_number << "\n";
        }

    }

    else cout << "Invalid account type!\n";

}

void deposit() {

    string file_path = get_holder_file_path(CURR_USER);
    BankAccountHolder curr_user = BankAccountHolder();
    curr_user.read(file_path);

    int acc_number = take_non_empty_int("Enter account number: ");

    if (curr_user.is_account(acc_number)) {
        int amount = take_non_empty_int("Enter the amount: ");

        CurrentAccount * curr_user_curr_accs = curr_user.get_current_accounts();
        SavingsAccount * curr_user_save_accs = curr_user.get_savings_accounts();

        for (int i = 0; i < curr_user.num_of_current_acc; i++) {
            if (curr_user_curr_accs[i].acc_number == acc_number) {
                curr_user_curr_accs[i].deposit(amount);
                curr_user.write(file_path);
                return;
            }
        }

        for (int i = 0; i < curr_user.num_of_savings_acc; i++) {
            if (curr_user_save_accs[i].acc_number == acc_number) {
                curr_user_save_accs[i].deposit(amount);
                curr_user.write(file_path);
                return;
            }
        }
    }

    else cout << "Invalid account number\n";
}

void withdraw() {

    string file_path = get_holder_file_path(CURR_USER);
    BankAccountHolder curr_user = BankAccountHolder();
    curr_user.read(file_path);

    int acc_number = take_non_empty_int("Enter account number: ");

    if (curr_user.is_account(acc_number)) {
        int amount = take_non_empty_int("Enter the amount: ");

        CurrentAccount * curr_user_curr_accs = curr_user.get_current_accounts();
        SavingsAccount * curr_user_save_accs = curr_user.get_savings_accounts();

        for (int i = 0; i < curr_user.num_of_current_acc; i++) {
            if (curr_user_curr_accs[i].acc_number == acc_number) {
                try {
                    curr_user_curr_accs[i].withdraw(amount);
                    curr_user.write(file_path);
                }
                catch (const char * msg) {
                    cout << msg << "\n";
                }
                return;
            }
        }

        for (int i = 0; i < curr_user.num_of_savings_acc; i++) {

            if (curr_user_save_accs[i].acc_number == acc_number) {
                try {
                    curr_user_save_accs[i].withdraw(amount);
                    curr_user.write(file_path);
                }
                catch (const char * msg) {
                    cout << msg << "\n";
                }
                return;
            }
        }
    }

    else cout << "Invalid account number\n";
}

// WRAPPERS FOR ROOT COMMANDS

void root_getacc() {

}

void root_getstat() {

}

void root_ffwd() {

}


// CLI PARSER

void execute_global_command(int command_num) {

    switch (command_num) {

        case 1:     help();         break;
        case 2:     quit();         break;
        case 3:     signup();       break;
        case 4:     login();        break;
        case 5:     root_login();   break;
    
        default:    cout << "Invalid Command!\n"; break;

    }
}

void execute_root_command(int command_num) {

    switch (command_num) {

        case 1:     logout();       break;
        case 2:     root_getacc();  break;
        case 3:     root_getstat(); break;
        case 4:     root_ffwd();    break;

        default:    cout << "Invalid Command!\n"; break;

    }

}

void execute_user_command(int command_num) {

    switch (command_num) {

        case 1:     logout();       break;
        case 2:     chpass();       break;
        case 3:     cracc();        break;
        case 4:     getacc();       break;
        case 5:     deposit();      break;
        case 6:     withdraw();     break;
        
        default:    cout << "Invalid Command!\n"; break;

    }

}

// MAIN
int main() {

    string global_prompt = "bank: ";
    string root_prompt = "root: ";
    string user_prompt = "username: ";

    string command = "";
    int command_num = 0;

    setup_commands();


    while (true) {

        if (CURR_USER == "") {
            command = take_non_empty_input(global_prompt);
            execute_global_command(GLOBAL_COMMANDS[command]);
        }

        else if (CURR_USER == "root") {
            command = take_non_empty_input(root_prompt);
            execute_root_command(ROOT_COMMANDS[command]);
        }

        else {
            user_prompt = CURR_USER;
            user_prompt.append(": ");

            command = take_non_empty_input(user_prompt);
            execute_user_command(USER_COMMANDS[command]);
        }

    }
}
