#include <string>
#include <list>
#include "BankAccountHolder.h"

class BranchManager {

    public:
        string username = "root";

        // returns the singleton instance of this class
        static BranchManager& getInstance() {

            // static declaration of the singleton instance
            static BranchManager instance;
            return instance;
        }

        bool login_try(string password) const {
            return this->password == password;
        }

        // return details of all account holders (to be implemented)
        void getAccountHolders() {
        }

        // change the root password (this is a dangerous function, and is not implmented in the main file)
        void change_password(string password) {
            this->password = password;
        }

        // return bank statement of an account
        void getStatementOfAccount() {
            return;
        }

        // fast forwards all bank accounts
        void fastForward() {
            return;
        }

    private:
        string password = "root";
        string acc_holders_dir = "AccountHolders/";
};