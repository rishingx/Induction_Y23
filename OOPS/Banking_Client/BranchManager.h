#include <string>
#include "BankAccountHolder.h"

class BranchManager {

    public:
        string username = "root";

        static BranchManager& getInstance() {
            static BranchManager instance;
            return instance;
        }

        bool login_try(string password) const {
            return this->password == password;
        }

        void change_password(string password) {
            this->password = password;
        }

        void getStatementOfAccount() {
            return;
        }

        void getAccountHolders() {
            return;
        }

        void fastForward() {
            return;
        }

    private:
        string password = "root";

};