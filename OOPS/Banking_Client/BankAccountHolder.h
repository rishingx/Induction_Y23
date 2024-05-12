#include <string>

#include "BankAccount.h"

using namespace std;

class BankAccountHolder {

    public:
        string username;
        int num_of_acc = 0;

        // default constructor
        BankAccountHolder() {
        }

        // construct object from values stored in a file
        BankAccountHolder(string file) {
            this->read(file);
        }

        // construct object from parameters
        BankAccountHolder(string username, string password, string name) {
            this->username = username;
            this->password = password;
            this->name = name;
        }

        // check for correct password (this is used, as we do not want direct access to the password attribute)
        bool login_try(string password) const {
            return this->password == password;
        }

        // check if an account number belongs to this account holder
        bool is_account(int acc_number) const {
            for (int i = 0; i < num_of_acc; i++) if (acc_list[i] == acc_number) return true;
            return false;
        }

        // change the password (this is a dangerous function, needs strong wrapping)
        void change_password(string password) {
            this->password = password;
        }

        // create a savings or current account for this account holder
        void create_account(int acc_number, int balance, string open_date, string acc_type, string file) {

            if (acc_type == "savings") {
            
                SavingsAccount new_acc = SavingsAccount(acc_number, balance);   // generate object using given parameters
                new_acc.write(file);                                            // write the object attributes to a file
                acc_list[num_of_acc] = acc_number;                              // add account number to list of accounts
                num_of_acc = num_of_acc + 1;                                    // increase number of accounts by 1 

            }

            else if (acc_type == "current") {

                CurrentAccount new_acc = CurrentAccount(acc_number, balance);   // generate object using given parameters
                new_acc.write(file);                                            // write the object attributes to file
                acc_list[num_of_acc] = acc_number;                              // add account number to list of accounts
                num_of_acc = num_of_acc + 1;                                    // increase number of accounts by 1

            }
        }

        // get a an array containing the account numbers of this account holder
        int * get_accounts() {
            return this->acc_list;
        }

        // write the attributes to file
        void write(string file) const {
            ofstream ofs(file, ios::binary);
            ofs.write(reinterpret_cast<const char*>(&username), sizeof(username));
            ofs.write(reinterpret_cast<const char*>(&password), sizeof(password));
            ofs.write(reinterpret_cast<const char*>(&name), sizeof(name));
            ofs.write(reinterpret_cast<const char*>(&num_of_acc), sizeof(num_of_acc));
            ofs.write(reinterpret_cast<const char*>(&acc_list), sizeof(acc_list));
            ofs.close();
        }

        // read the attributes from file
        void read(string file) {
            ifstream ifs(file, ios::binary);
            ifs.read(reinterpret_cast<char*>(&username),    sizeof(username));
            ifs.read(reinterpret_cast<char*>(&password),    sizeof(password));
            ifs.read(reinterpret_cast<char*>(&name),        sizeof(name));
            ifs.read(reinterpret_cast<char*>(&num_of_acc),  sizeof(num_of_acc));
            ifs.read(reinterpret_cast<char*>(&acc_list),    sizeof(acc_list));
            ifs.close();
        }

    private:
        string password;
        string name;
        int acc_list[64];
};