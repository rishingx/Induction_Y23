#include <fstream>
using namespace std;

class BankAccount {
    public:
        int acc_number;
        int type;

        // default constructor
        BankAccount() {
        }

        // construct object from values stored in a file
        BankAccount(string file) {
            this->read(file);
        }

        // construct object from parameters
        BankAccount(int acc_number, int balance) {
            this->acc_number = acc_number;
            this->balance = balance;
        }

        // deposit the amount passed into paramter: amt
        void deposit(int amt) {
            this->balance = this->balance + amt;
        }

        // withdraw the amount passed into parameter: amt (only if it does not exceed balance)
        void withdraw(int amt) {
            if (amt > this->balance) {
                throw "Not enough balance!";
            }
            this->balance = this->balance - amt;
        }

        // transfer the amount passed into parameter: amt to the account dest_acc
        void transfer(int amt, BankAccount dest_acc) {
            this->withdraw(amt);
            dest_acc.deposit(amt);
        }

        // write the attributes to file
        void write(string file) const {
            ofstream ofs(file, ios::binary);
            ofs.write(reinterpret_cast<const char*> (&acc_number),      sizeof(acc_number));
            ofs.write(reinterpret_cast<const char*> (&balance),         sizeof(balance));
            ofs.write(reinterpret_cast<const char*> (&interest_rate),   sizeof(interest_rate));
            ofs.close();
        }

        // read the attributes from file
        void read(string file) {
            ifstream ifs(file, ios::binary);
            ifs.read(reinterpret_cast<char*>    (&acc_number),      sizeof(acc_number));
            ifs.read(reinterpret_cast<char*>    (&balance),         sizeof(balance));
            ifs.read(reinterpret_cast<char*>    (&interest_rate),   sizeof(interest_rate));
            ifs.close();
        }

    protected:
        int balance;
        int interest_rate;
};

class SavingsAccount: public BankAccount {

    public:

        // default constructor
        SavingsAccount() {
        }

        // construct object from parameters
        SavingsAccount(int acc_number, int balance) : BankAccount(acc_number, balance) {
            this->interest_rate = 0;
        }

        // return the interest rate
        int getInterestRate() {
            return this->interest_rate;
        }

        // set the interest rate passed into parameter new_rate
        void setInterestRate(int new_rate) {
            this->interest_rate = new_rate;
        }

};

class CurrentAccount: public BankAccount {

    public:

        // default constructor
        CurrentAccount() {
        }

        // construct object from parameters
        CurrentAccount(int acc_number, int balance) : BankAccount(acc_number, balance) {
            this->interest_rate = 0;
        }

        // return the interest rate
        int getInterestRate() {
            return this->interest_rate;
        }
};