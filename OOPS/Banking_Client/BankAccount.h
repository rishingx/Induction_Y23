#include <fstream>
using namespace std;

class BankAccount {
    public:
        int acc_number;
        int balance;

        BankAccount() {

        }

        BankAccount(int acc_number, int balance, string open_date) {
            this->acc_number = acc_number;
            this->balance = balance;
            this->open_date = open_date; 
        }

        void deposit(int amt) {
            this->balance = this->balance + amt;
        }

        void withdraw(int amt) {
            if (amt > this->balance) {
                throw "Not enough balance!";
            }
            this->balance = this->balance - amt;
        }

        void transfer(int amt, int dest_acc) {
            return;
        }

        void write(string file) const {
            ofstream ofs(file, ios::binary);
            ofs.write(reinterpret_cast<const char*>(&acc_number), sizeof(acc_number));
            ofs.write(reinterpret_cast<const char*>(&balance), sizeof(balance));
            ofs.write(reinterpret_cast<const char*>(&interest_rate), sizeof(interest_rate));
            ofs.write(reinterpret_cast<const char*>(&open_date), sizeof(open_date));
            ofs.write(reinterpret_cast<const char*>(&type), sizeof(type));
            ofs.close();
        }

        void read(string file) {
            ifstream ifs(file, ios::binary);
            ifs.read(reinterpret_cast<char*>(&acc_number), sizeof(acc_number));
            ifs.read(reinterpret_cast<char*>(&balance), sizeof(balance));
            ifs.read(reinterpret_cast<char*>(&interest_rate), sizeof(interest_rate));
            ifs.read(reinterpret_cast<char*>(&open_date), sizeof(open_date));
            ifs.read(reinterpret_cast<char*>(&type), sizeof(type));
            ifs.close();
        }


    protected:
        int interest_rate;
        string open_date;
        string type;
};

class SavingsAccount: public BankAccount {

    public:

        SavingsAccount() {
        }

        SavingsAccount(int acc_number, int balance, string open_date) : BankAccount(acc_number, balance, open_date) {
            this->type = "savings";
            this->interest_rate = interest_rate;
        }

        int getInterestRate() {
            return this->interest_rate;
        }

        void setInterestRate(int new_rate) {
            this->interest_rate = new_rate;
        }

};

class CurrentAccount: public BankAccount {

    public:

        CurrentAccount() {
        }

        CurrentAccount(int acc_number, int balance, string open_date) : BankAccount(acc_number, balance, open_date) {
            this->type = "current";
            this->interest_rate = 0;
        }

        int getInterestRate() {
            return this->interest_rate;
        }

};