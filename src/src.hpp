#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>
#include <tuple>
#include <algorithm>

namespace EOSThoughts {
    using namespace eosio;
    using std::string;

    class Thoughts : public contract {
        using contract::contract;

        public:
            Thoughts(account_name _self ):contract( _self ), _users(_self, _self) {}

            [[eosio::action]]
            void create( account_name account, string message, string memo );

            [[eosio::action]]
            void remove( account_name account, uint64_t key, string memo );

            [[eosio::action]]
            void addfollower( account_name account, account_name who, string memo );

            [[eosio::action]]
            void delfollower( account_name account, account_name who, string memo );

            struct [[eosio::table]] thought {
                uint64_t key;
                string text;

                uint64_t primary_key() const { return key; }
                EOSLIB_SERIALIZE(thought, (key)(text))
            };

            struct [[eosio::table]] users {
                account_name owner;
                vector<thought> thoughts;
                vector<account_name> following;

                uint64_t primary_key() const { return owner; }
                EOSLIB_SERIALIZE(users, (owner)(thoughts)(following))
            };
            typedef multi_index<N(users), users> usersindex;
        private:
            usersindex _users;
    };

    EOSIO_ABI(Thoughts, (create)(remove)(addfollower)(delfollower))
}