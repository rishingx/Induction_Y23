// === INCLUDE DIRECTIVES === //
#include <map>
#include <fstream>
#include <iostream>
#include <filesystem>

#include "BranchManager.h"

using namespace std;



// === GLOBAL VARIABLES === //
string HELP_MSG = "\n";         // default help message
string CURR_USER = "";          // username of the current user
string DATE = "17-10-2004";     // current date (for now has my bday)



// === THE ROOT USER === //
BranchManager& ROOT_USER = BranchManager::getInstance();



// === MAP OBJECTS TO GENERATE AN INDEX FROM CLI COMMANDS === //
map <string, int> GLOBAL_COMMANDS;
map <string, int> USER_COMMANDS;
map <string, int> ROOT_COMMANDS;



// === UTILITY FUNCTIONS === //

// get the file path of an user from the username
string get_holder_file_path(string username) {

    string file_path = "AccountHolders/";
    file_path.append(username);
    file_path.append(".bin");

    return file_path;
}

// get the file path of an account from the account number
string get_account_file_path(int acc_number) {

    string file_path = "Accounts/";
    file_path.append(to_string(acc_number));
    file_path.append(".bin");

    return file_path;
}

// take a string input that is not empty
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

// take a positive integer input
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

// generate account number (this should generate a random number, but for now it simply takes an user input)
int generate_account_number() {
    int acc_num = take_non_empty_int("Enter account number: ");
    return acc_num;
}



// === COMMAND MAPS - intializes the map objects defined above === //
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
    USER_COMMANDS   ["crtacc"]      = 3;
    USER_COMMANDS   ["getacc"]      = 4;
    USER_COMMANDS   ["deposit"]     = 5;
    USER_COMMANDS   ["withdraw"]    = 6;
    USER_COMMANDS   ["transfer"]    = 7;

}



// === WRAPPERS FOR GLOBAL COMMANDS === //

// return default help message
void help() {
    cout << HELP_MSG;
}

// quit the cli
void quit() {
    cout << "Exitting Application\n";
    exit(0);
}

// sign up as a new user
void signup() {

    string input_username = take_non_empty_input("Enter username: ");   // input username

    string file_path = get_holder_file_path(input_username);            // generate file path
    bool exist = filesystem::exists(file_path);                         // check if file already exists

    if (exist) {                                                        // if it does
        cout << "User already exists!\n";                               // exit with error message
        return;
    }

    string input_password = take_non_empty_input("Enter password: ");   // input password
    string input_name = take_non_empty_input("Enter name: ");           // input name

    // generate BankAccountHolder object and write it to the file path generated above
    BankAccountHolder new_user = BankAccountHolder(input_username, input_password, input_name);
    new_user.write(file_path);
}

// login as a regular user
void login() {

    string input_username = take_non_empty_input("Enter username: ");   // input username

    string file_path = get_holder_file_path(input_username);            // generate file path
    bool exist = filesystem::exists(file_path);                         // check if file already exists

    if (!exist) {                                                       // if it does not
        cout << "User does not exist!\n";                               // exit with error message
        return;
    }

    string input_password = take_non_empty_input("Enter password: ");   // input password
    BankAccountHolder curr_user = BankAccountHolder(file_path);         // retrieve account holder object
    
    if (curr_user.login_try(input_password)) CURR_USER = curr_user.username;    // if password is correct: login
    else cout << "Invalid password!\n";                                         // else: exit with error message

}

// login as a root user
void root_login() {

    string input_password = take_non_empty_input("Enter root password: ");  // input password
    if (ROOT_USER.login_try(input_password)) CURR_USER = "root";            // if password is correct: login
    else cout << "Invalid password!\n";                                     // else: exit with error message

}



// === WRAPPERS FOR USER COMMANDS === //

// log out of user account (root/regular)
void logout() {
    CURR_USER = "";     // set current user username to empty
}

// change password of a regular user
void chpass() {

    string file_path = get_holder_file_path(CURR_USER);                     // generate current user file path
    string curr_pass = take_non_empty_input("Enter current password: ");    // input password
    
    BankAccountHolder curr_user = BankAccountHolder(file_path);             // retrieve account holder object

    if (curr_user.login_try(curr_pass)) {                                   // if password is correct:

        string new_pass = take_non_empty_input("Enter new password: ");         // input new password
        curr_user.change_password(new_pass);                                    // change the password
        curr_user.write(file_path);                                             // save account holder object

    }
    else cout << "Invalid password!\n";                                     // else: exit with error message

}

// create a new savings of current account
void crtacc() {

    string file_path = get_holder_file_path(CURR_USER);                     // generate current user file path
    BankAccountHolder curr_user = BankAccountHolder(file_path);             // retrieve account holder object

    string acc_type = take_non_empty_input("Enter account type: ");         // input account type

    while((acc_type != "savings") && (acc_type != "current")) {             // keep taking input till account type is valid
        cout << "Enter a valid account type\n";
        acc_type = take_non_empty_input("Enter acocunt type: ");
    }

    int balance = take_non_empty_int("Enter opening balance: ");            // input opening balance

    int acc_number = generate_account_number();                             // generate account number
    string acc_file_path = get_account_file_path(acc_number);               // generate account file path

    curr_user.create_account(acc_number, balance, DATE, acc_type, acc_file_path);   // create account
    curr_user.write(file_path);                                                     // save account holder object

}

// output all the account numbers of the current user
void getacc() {

    string file_path = get_holder_file_path(CURR_USER);                     // generate current user file path
    BankAccountHolder curr_user = BankAccountHolder(file_path);             // retrieve account holder object

    int * acc_list = curr_user.get_accounts();                              // get list of current user's accounts

    for (int i = 0; i < curr_user.num_of_acc; i++) {                        // for each account
        cout << acc_list[i] << endl;                                            // print the account number
    }
}

// deposit into an account of the current user
void deposit() {

    string file_path = get_holder_file_path(CURR_USER);                     // generate current user file path
    BankAccountHolder curr_user = BankAccountHolder(file_path);             // retrieve account holder object

    int acc_number = take_non_empty_int("Enter account number: ");          // input account number

    if (curr_user.is_account(acc_number)) {                                 // if the account number is current user's:
        int amount = take_non_empty_int("Enter the amount: ");                  // input amount
        string acc_file_path = get_account_file_path(acc_number);               // generate account file path

        BankAccount curr_user_acc = BankAccount(acc_file_path);                 // retrieve bank account object
        curr_user_acc.deposit(amount);                                          // deposit amount
        curr_user_acc.write(acc_file_path);                                     // save bank account object
    }

    else cout << "Invalid account number\n";                                // else: exit with error message
}

// withdraw from an account of the current user
void withdraw() {
    string file_path = get_holder_file_path(CURR_USER);                     // generate current user file path
    BankAccountHolder curr_user = BankAccountHolder(file_path);             // retrieve account holder object

    int acc_number = take_non_empty_int("Enter account number: ");          // input account number

    if (curr_user.is_account(acc_number)) {                                 // if the account number is current user's:
        int amount = take_non_empty_int("Enter the amount: ");                  // input amount
        string acc_file_path = get_account_file_path(acc_number);               // generate account file path

        BankAccount curr_user_acc = BankAccount(acc_file_path);                 // retrieve bank account object

        try {                                                                   // try: (used as withdrawl amount might exceed balance)
            curr_user_acc.withdraw(amount);                                         // withdraw amount
            curr_user_acc.write(acc_file_path);                                     // save bank account object
        }
        catch (const char * msg) {                                              // error: withdrawl amount > balance
            cout << msg << "\n";                                                    // exit with error message
        }
    }
    
    else cout << "Invalid account number\n";                                // else: exit with error message
}

// transfer from an account of the current user to any other account
void transfer() {

    string file_path = get_holder_file_path(CURR_USER);                 // generate current user file path
    BankAccountHolder curr_user = BankAccountHolder(file_path);         // retrieve account holder object

    int src_acc_num = take_non_empty_int("Enter source account number: ");          // input source account number
    int dest_acc_num = take_non_empty_int("Enter destination account number: ");    // input destination account number

    if (curr_user.is_account(src_acc_num)) {                            // if the source account number is current user's:
        int amount = take_non_empty_int("Enter the amount: ");              // input amount

        string src_acc_file_path = get_account_file_path(src_acc_num);      // generate source account file path
        string dest_acc_file_path = get_account_file_path(dest_acc_num);    // generate destination acocunt file path

        BankAccount src_acc = BankAccount(src_acc_file_path);               // retrieve source account object
        BankAccount dest_acc = BankAccount(dest_acc_file_path);             // retrieve destination account object

        try {                                                               // try: (used as transfer amout might exceed balance)
            src_acc.transfer(amount, dest_acc);                                 // transfer amount
            src_acc.write(src_acc_file_path);                                   // save source account object
            dest_acc.write(dest_acc_file_path);                                 // save destination account object
        }
        catch (const char * msg) {                                          // error: withdrawl amount > balance
            cout << msg << "\n";                                                // exit with error message
        }

    }
}



// === WRAPPERS FOR ROOT COMMANDS === //

// get details of all account holders (not implemented yet)
void root_getacc() {
    ROOT_USER.getAccountHolders();
}

// get bank statement of an account (not implemented yet)
void root_getstat() {
}

// fast forward all the bank accounts (not implemented yet)
void root_ffwd() {
}



// === CLI PARSER === //

// execute a global level command based on the index returned by map object GLOBAL_COMMANDS
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

// execute a root level command based on the index returned by map object ROOT_COMMANDS
void execute_root_command(int command_num) {

    switch (command_num) {

        case 1:     logout();       break;
        case 2:     root_getacc();  break;
        case 3:     root_getstat(); break;
        case 4:     root_ffwd();    break;

        default:    cout << "Invalid Command!\n"; break;

    }
}

// execute an user level command based on the index returned by map object USER_COMMANDS
void execute_user_command(int command_num) {

    switch (command_num) {

        case 1:     logout();       break;
        case 2:     chpass();       break;
        case 3:     crtacc();        break;
        case 4:     getacc();       break;
        case 5:     deposit();      break;
        case 6:     withdraw();     break;
        case 7:     transfer();     break;
        
        default:    cout << "Invalid Command!\n"; break;

    }
}



// === MAIN FUNCTION === //

int main() {

    // prompts for the cli
    string global_prompt = "bank: ";
    string root_prompt = "root: ";
    string user_prompt = "username: ";

    string command = "";    // variable to store the command entered by the user
    int command_num = 0;    // variable to store the index of the entered command

    setup_commands();       // initalize the command-index maps

    while (true) {

        if (CURR_USER == "") {                                  // global level
            command = take_non_empty_input(global_prompt);          // input command
            execute_global_command(GLOBAL_COMMANDS[command]);       // execute command
        }

        else if (CURR_USER == "root") {                         // root level
            command = take_non_empty_input(root_prompt);            // input command
            execute_root_command(ROOT_COMMANDS[command]);           // execute command
        }

        else {                                                  // user level
            user_prompt = CURR_USER;                                // generate prompt based on current user
            user_prompt.append(": ");

            command = take_non_empty_input(user_prompt);            // input command
            execute_user_command(USER_COMMANDS[command]);           // execute command
        }

    }
}