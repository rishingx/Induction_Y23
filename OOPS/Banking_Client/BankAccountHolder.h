#include <string>

#include "BankAccount.h"

using namespace std;

class BankAccountHolder {

    public:
        string username;
        int num_of_savings_acc = 0;
        int num_of_current_acc = 0;

        BankAccountHolder() {
        }

        BankAccountHolder(string username, string password, string name) {
            this->username = username;
            this->password = password;
            this->name = name;
        }

        bool login_try(string password) const {
            return this->password == password;
        }

        bool is_account(int acc_number) const {

            for (int i = 0; i < num_of_current_acc; i++) if (((this->current_acc_list)[i]).acc_number == acc_number) return true;

            for (int i = 0; i < num_of_savings_acc; i++) if (((this->savings_acc_list)[i]).acc_number == acc_number) return true;

            return false;
        }

        void change_password(string password) {
            this->password = password;
        }

        void create_account(int acc_number, int balance, string open_date, string acc_type) {

            if (acc_type == "savings") {
            
                SavingsAccount new_acc = SavingsAccount(acc_number, balance, open_date);
                (this->savings_acc_list)[this->num_of_savings_acc] = new_acc;
                this->num_of_savings_acc = this->num_of_savings_acc + 1;

            }

            else if (acc_type == "current") {

                CurrentAccount new_acc = CurrentAccount(acc_number, balance, open_date);
                (this->current_acc_list)[this->num_of_current_acc] = new_acc;
                this->num_of_current_acc = this->num_of_current_acc + 1;

            }
        }

        CurrentAccount * get_current_accounts() {
            return this->current_acc_list;
        }

        SavingsAccount * get_savings_accounts() {
            return this->savings_acc_list;
        }

        void write(string file) const {
            ofstream ofs(file, ios::binary);
            ofs.write(reinterpret_cast<const char*>(&username), sizeof(username));
            ofs.write(reinterpret_cast<const char*>(&password), sizeof(password));
            ofs.write(reinterpret_cast<const char*>(&name), sizeof(name));
            ofs.write(reinterpret_cast<const char*>(&num_of_current_acc), sizeof(num_of_current_acc));
            ofs.write(reinterpret_cast<const char*>(&num_of_savings_acc), sizeof(num_of_savings_acc));
            ofs.write(reinterpret_cast<const char*>(&current_acc_list), sizeof(current_acc_list));
            ofs.write(reinterpret_cast<const char*>(&savings_acc_list), sizeof(savings_acc_list));
            ofs.close();
        }

        void read(string file) {
            ifstream ifs(file, ios::binary);
            ifs.read(reinterpret_cast<char*>(&username), sizeof(username));
            ifs.read(reinterpret_cast<char*>(&password), sizeof(password));
            ifs.read(reinterpret_cast<char*>(&name), sizeof(name));
            ifs.read(reinterpret_cast<char*>(&num_of_current_acc), sizeof(num_of_current_acc));
            ifs.read(reinterpret_cast<char*>(&num_of_savings_acc), sizeof(num_of_savings_acc));
            ifs.read(reinterpret_cast<char*>(&current_acc_list), sizeof(current_acc_list));
            ifs.read(reinterpret_cast<char*>(&savings_acc_list), sizeof(savings_acc_list));
            ifs.close();
        }

    private:
        string password;
        string name;
        CurrentAccount current_acc_list[16];
        SavingsAccount savings_acc_list[16];
};